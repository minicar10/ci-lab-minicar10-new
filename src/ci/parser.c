#define _GNU_SOURCE
#include "parser.h"
#include "command.h"
#include "command_type.h"
#include "token_type.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static Token    advance(Parser *parser);
static bool     consume(Parser *parser, TokenType type);
static bool     is_at_end(Parser *parser);
static void     skip_nls(Parser *parser);
static bool     consume_newline(Parser *parser);
static Command *create_command(CommandType type);
static bool     is_variable(Token token);
static bool     parse_variable(Token token, int64_t *var_num);
static bool     parse_number(Token token, int64_t *result);
static bool     parse_variable_operand(Parser *parser, Operand *op);
static bool     parse_var_or_imm(Parser *parser, Operand *op, bool *is_immediate);
static Command *parse_cmd(Parser *parser);

void parser_init(Parser *parser, Lexer *lexer, LabelMap *map) {
    if (!parser) {
        return;
    }

    parser->lexer     = lexer;
    parser->had_error = false;
    parser->label_map = map;
    parser->current   = lexer_next_token(parser->lexer);
    parser->next      = lexer_next_token(parser->lexer);
}

/**
 * @brief Advances the parser in the token stream.
 *
 * @param parser A pointer to the parser to read tokens from.
 * @return The token that was just consumed.
 */
static Token advance(Parser *parser) {
    Token ret_token = parser->current;
    if (!is_at_end(parser)) {
        parser->current = parser->next;
        parser->next    = lexer_next_token(parser->lexer);
    }
    return ret_token;
}

/**
 * @brief Determines if the parser reached the end of the token stream.
 *
 * @param parser A pointer to the parser to read tokens from.
 * @return True if the parser is at the end of the token stream, false
 * otherwise.
 */
static bool is_at_end(Parser *parser) {
    return parser->current.type == TOK_EOF;
}

/**
 * @brief Consumes the token if it matches the specified token type.
 *
 * @param parser A pointer to the parser to read tokens from.
 * @param type The type of the token to match.
 * @return True if the token was consumed, false otherwise.
 */
static bool consume(Parser *parser, TokenType type) {
    if (parser->current.type == type) {
        advance(parser);
        return true;
    }

    return false;
}

/**
 * @brief Creates a command of the given type.
 *
 * @param type The type of the command to create.
 * @return A pointer to a command with the requested type.
 *
 * @note It is the responsibility of the caller to free the memory associated
 * with the returned command.
 */
static Command *create_command(CommandType type) {
    Command *cmd = (Command *) calloc(1, sizeof(Command));
    if (!cmd) {
        return NULL;
    }

    cmd->type             = type;
    cmd->next             = NULL;
    cmd->is_a_immediate   = false;
    cmd->is_a_string      = false;
    cmd->is_b_immediate   = false;
    cmd->is_b_string      = false;
    cmd->branch_condition = BRANCH_NONE;

    // Default values for operands
    cmd->val_a.num_val = 0;
    cmd->val_b.num_val = 0;

    return cmd;
}


/**
 * @brief Determines if the given token is a valid variable.
 *
 * A valid (potential) variable is a token that begins with the prefix "x",
 * followed by any other character(s).
 *
 * @param token The token to check.
 * @return True if this token could be a variable, false otherwise.
 */
static bool is_variable(Token token) {
    return token.length >= 2 && token.lexeme[0] == 'x';
}

/**
 * @brief Determines if the given token is a valid base signifier.
 *
 * A valid base signifier is one of d (decimal), x (hex), b (binary) or s (string).
 *
 * @param token The token to check.
 * @return True if this token is a base signifier, false otherwise
 */
static bool is_base(Token token) {
    return token.length == 1 && (token.lexeme[0] == 'd' || token.lexeme[0] == 'x' ||
                                 token.lexeme[0] == 's' || token.lexeme[0] == 'b');
}

/**
 * @brief Parses the given token as a base signifier
 *
 * A base is a single character, either d, s, x, or b.
 *
 * @param parser A pointer to the parser to read tokens from.
 * @param op A pointer to the operand to modify.
 * @return True if the current token was parsed as a base, false otherwise.
 */
static bool parse_base(Parser *parser, Operand *op) {
    // STUDENT TODO: Parse the current token as a base
    return false;
}

/**
 * @brief Parses the given token as a variable.
 *
 * @param token The token to parse.
 * @param var_num a pointer to modify on success.
 * @return True if `var_num` was successfully modified, false otherwise.
 *
 * @note It is assumed that the token already was verified to begin with a valid
 * prefix, "x".
 */
static bool parse_variable(Token token, int64_t *var_num) {
    char   *endptr;
    int64_t tempnum = strtol(token.lexeme + 1, &endptr, 10);

    if ((token.lexeme + token.length) != endptr || tempnum < 0 || tempnum > 31) {
        return false;
    }

    *var_num = tempnum;
    return true;
}

/**
 * @brief Parses the given value as a number.
 *
 * @param token The token to parse.
 * @param result A pointer to the value to modify on success.
 * @return True if `result` was successfully modified, false otherwise.
 */
static bool parse_number(Token token, int64_t *result) {
    const char *parse_start = token.lexeme;
    int         base        = 10;

    if (token.length > 2 && token.lexeme[0] == '0') {
        if (token.lexeme[1] == 'x') {
            parse_start += 2;
            base = 16;
        } else if (token.lexeme[1] == 'b') {
            parse_start += 2;
            base = 2;
        }
    }

    char *endptr;
    *result = strtoll(parse_start, &endptr, base);

    return (token.lexeme + token.length) == endptr;
}

/**
 * @brief Conditionally parses the current token as a number.
 *
 * Note that this won't advance the parser if the token cannot be converted to
 * an integer.
 *
 * @param parser A pointer to the parser to read tokens from.
 * @param op A pointer to the operand to modify.
 * @return True if this token is a number and was was converted successfully,
 * false otherwise.
 */
static bool parse_imm(Parser *parser, Operand *op) {
    Token *token = &parser->current;
    if (token->type != TOK_NUM) {
        return false;
    }

    uint64_t value = 0;
    if (token->length >= 2 && token->lexeme[0] == '0') {
        if (token->lexeme[1] == 'x' || token->lexeme[1] == 'X') {
            for (int i = 2; i < token->length; i++) {
                char c = token->lexeme[i];
                if (c >= '0' && c <= '9') {
                    value = value * 16 + (c - '0');
                } else if (c >= 'a' && c <= 'f') {
                    value = value * 16 + (c - 'a' + 10);
                } else if (c >= 'A' && c <= 'F') {
                    value = value * 16 + (c - 'A' + 10);
                } else {
                    return false;
                }
            }
        } else if (token->lexeme[1] == 'b' || token->lexeme[1] == 'B') {
            for (int i = 2; i < token->length; i++) {
                char c = token->lexeme[i];
                if (c == '0' || c == '1') {
                    value = value * 2 + (c - '0');
                } else {
                    return false;
                }
            }
        } else {
            return false;
        }
    } else {
        for (int i = 0; i < token->length; i++) {
            char c = token->lexeme[i];
            if (c >= '0' && c <= '9') {
                value = value * 10 + (c - '0');
            } else {
                return false;
            }
        }
    }

    op->num_val = value;
    advance(parser);
    return true;
}

/**
 * @brief Parses the next token as a variable.
 *
 * A variable is anything starting with the prefix x and will be of type
 * TOK_IDENT.
 *
 * @param parser A pointer to the parser to read tokens from.
 * @param op A pointer to the operand to modify.
 * @return True if this was parsed as a variable, false otherwise.
 */
static bool parse_variable_operand(Parser *parser, Operand *op) {
    Token *token = &parser->current;

    if (token->type != TOK_IDENT || !is_variable(*token)) {
        return false;
    }

    int64_t var_num = 0;
    if (!parse_variable(*token, &var_num)) {
        return false;
    }

    op->base = var_num; // Set variable index
    advance(parser);    // Consume token
    return true;
}

/**
 * @brief Parses the next token as either a variable or an immediate.
 *
 * A number is considered to be anything beginning with a decimal digit or the
 * prefixes 0b or 0x and will be of type TOK_NUM. A variable is anything
 * starting with the prefix x and will be of type TOK_IDENT.
 *
 * @param parser A pointer to the parser to read tokens from.
 * @param op A pointer to the operand to modify.
 * @param is_immediate A pointer to a boolean to modify upon determining whether
 * the given value is an immediate.
 * @return True if this was parsed as an immediate or a variable, false
 * otherwise.
 */
static bool parse_var_or_imm(Parser *parser, Operand *op, bool *is_immediate) {
    Token *token = &parser->current;

    if (token->type == TOK_IDENT) {
        // Parse as variable
        if (!parse_variable_operand(parser, op)) {
            return false;
        }
        *is_immediate = false;
        return true;
    }

    if (token->type == TOK_NUM) {
        // Parse as immediate
        if (!parse_imm(parser, op)) {
            return false;
        }
        *is_immediate = true;
        return true;
    }

    advance(parser); // Skip invalid token
    return false;
}


/**
 * @brief Skips past tokens that signal the start of a new line
 *
 * Consumes newlines until the end of file is reached.
 * An EOF is not considered to be a "new line" in this context because it is a
 * sentinel token, I.e, there is nothing after it.
 *
 * @param parser A pointer to the parser to read tokens from.
 */
static void skip_nls(Parser *parser) {
    while (consume(parser, TOK_NL))
        ;
}

/**
 * @brief Consumes a single newline
 *
 * @param parser A pointer to the parser to read tokens from.
 * @return True whether a "new line" was consumed, false otherwise.
 *
 * @note An encounter of TOK_EOF should not be considered a failure, as this
 * procedure is designed to "reset" the grammar. In other words, it should be
 * used to ensure that we have a valid ending token after encountering an
 * instruction. Since TOK_EOF signals no more possible instructions, it should
 * effectively play the role of a new line when checking for a valid ending
 * sequence for a command.
 */
static bool consume_newline(Parser *parser) {
    return consume(parser, TOK_NL) || consume(parser, TOK_EOF);
}

/**
 * @brief Prints detailed error messages for debugging and troubleshooting.
 *
 * @param parser A pointer to the parser to read tokens from.
 * @param message A descriptive error message explaining the issue.
 * @param cmd A pointer to the command being parsed, if any.
 */
static void print_error(Parser *parser, const char *message, Command *cmd) {
    printf("Parser encountered an error:\n");

    // Token details
    if (parser->current.type == TOK_EOF) {
        printf("At Token: EOF\n");
    } else {
        printf("At Token: %.*s\n", parser->current.length, parser->current.lexeme);
    }
    printf("Token type: %u\n", (unsigned int) parser->current.type);
    printf("Token length: %d\n", (int) parser->current.length);
    printf("Line: %d:%d\n\n", (int) parser->current.line, (int) parser->current.column);

    printf("Parsed commands up to this point:\n");
    // Set parser's error flag
    parser->had_error = true;
}


/**
 * @brief Parses a singular command.
 *
 * Reads in the token(s) from the lexer that the parser owns and determines the
 * appropriate matching command. Updates the parser->had_error if an error
 * occurs.
 *
 * @param parser A pointer to the parser to read tokens from.
 * @return A pointer to the appropriate command.
 * Returns null if an error occurred or there are no commands to parse.
 *
 * @note The caller is responsible for freeing the memory associated with the
 * returned command.
 */
static Command *parse_cmd(Parser *parser) {
    skip_nls(parser);

    // Halt parsing if an error has already occurred
    if (parser->had_error) {
        return NULL;
    }

    // EOF looking
    if (parser->current.type == TOK_EOF) {
        return NULL;
    }

    switch (parser->current.type) {
        case TOK_ADD: {
            advance(parser);
            Command *cmd = create_command(CMD_ADD);
            if (!cmd) {
                print_error(parser, "Failed to allocate memory for ADD command.", NULL);
                return NULL;
            }

            if (!parse_variable_operand(parser, &cmd->destination)) {
                print_error(parser, "Invalid destination operand for ADD command.", cmd);
                free_command(cmd);
                return NULL;
            }

            bool is_immediate_a = false;
            if (!parse_var_or_imm(parser, &cmd->val_a, &is_immediate_a)) {
                print_error(parser, "Invalid first operand for ADD command.", cmd);
                free_command(cmd);
                return NULL;
            }

            bool is_immediate_b = false;
            if (!parse_var_or_imm(parser, &cmd->val_b, &is_immediate_b)) {
                print_error(parser, "Insufficient arguments for ADD command.", cmd);
                free_command(cmd);
                return NULL;
            }

            cmd->is_a_immediate = is_immediate_a;
            cmd->is_b_immediate = is_immediate_b;

            if (parser->current.type != TOK_NL && parser->current.type != TOK_EOF) {
                print_error(parser, "Unexpected token after ADD command.", cmd);
                free_command(cmd);
                return NULL;
            }

            advance(parser);
            return cmd;
        }

        case TOK_SUB: {
            advance(parser);
            Command *cmd = create_command(CMD_SUB);
            if (!cmd) {
                print_error(parser, "Failed to allocate memory for SUB command.", NULL);
                return NULL;
            }

            if (!parse_variable_operand(parser, &cmd->destination)) {
                print_error(parser, "Invalid destination operand for SUB command.", cmd);
                free_command(cmd);
                return NULL;
            }

            bool is_immediate_a = false;
            if (!parse_var_or_imm(parser, &cmd->val_a, &is_immediate_a)) {
                print_error(parser, "Invalid first operand for SUB command.", cmd);
                free_command(cmd);
                return NULL;
            }

            bool is_immediate_b = false;
            if (!parse_var_or_imm(parser, &cmd->val_b, &is_immediate_b)) {
                print_error(parser, "Invalid second operand for SUB command.", cmd);
                free_command(cmd);
                return NULL;
            }

            cmd->is_a_immediate = is_immediate_a;
            cmd->is_b_immediate = is_immediate_b;

            if (parser->current.type != TOK_NL && parser->current.type != TOK_EOF) {
                print_error(parser, "Unexpected token after SUB command.", cmd);
                free_command(cmd);
                return NULL;
            }

            advance(parser);
            return cmd;
        }

        case TOK_MOV: {
            advance(parser);

            Command *cmd = create_command(CMD_MOV);
            if (!cmd) {
                print_error(parser, "Failed to allocate memory for MOV command.", NULL);
                return NULL;
            }

            if (!parse_variable_operand(parser, &cmd->destination)) {
                print_error(parser, "Invalid destination operand for MOV command.", cmd);
                free_command(cmd);
                return NULL;
            }

            bool is_immediate = false;
            if (!parse_var_or_imm(parser, &cmd->val_a, &is_immediate)) {
                print_error(parser, "Invalid source operand for MOV command.", cmd);
                free_command(cmd);
                return NULL;
            }

            cmd->is_a_immediate = is_immediate;

            if (parser->current.type != TOK_NL && parser->current.type != TOK_EOF) {
                print_error(parser, "Unexpected token after MOV command.", cmd);
                free_command(cmd);
                return NULL;
            }

            advance(parser);
            return cmd;
        }

        case TOK_CMP: {
            advance(parser);
            Command *cmd = create_command(CMD_CMP);
            if (!cmd) {
                print_error(parser, "Failed to allocate memory for CMP command.", NULL);
                return NULL;
            }

            bool is_immediate_a = false;
            if (!parse_var_or_imm(parser, &cmd->val_a, &is_immediate_a)) {
                print_error(parser, "Invalid first operand for CMP command.", cmd);
                free_command(cmd);
                return NULL;
            }

            bool is_immediate_b = false;
            if (!parse_var_or_imm(parser, &cmd->val_b, &is_immediate_b)) {
                print_error(parser, "Invalid second operand for CMP command.", cmd);
                free_command(cmd);
                return NULL;
            }

            cmd->is_a_immediate = is_immediate_a;
            cmd->is_b_immediate = is_immediate_b;

            if (parser->current.type != TOK_NL && parser->current.type != TOK_EOF) {
                print_error(parser, "Unexpected token after CMP command.", cmd);
                free_command(cmd);
                return NULL;
            }

            advance(parser);
            return cmd;
        }

        case TOK_CMP_U: {
            advance(parser);
            Command *cmd = create_command(CMD_CMP_U);
            if (!cmd) {
                print_error(parser, "Failed to allocate memory for CMP_U command.", NULL);
                return NULL;
            }

            bool is_immediate_a = false;
            if (!parse_var_or_imm(parser, &cmd->val_a, &is_immediate_a)) {
                print_error(parser, "Invalid first operand for CMP_U command.", cmd);
                free_command(cmd);
                return NULL;
            }

            bool is_immediate_b = false;
            if (!parse_var_or_imm(parser, &cmd->val_b, &is_immediate_b)) {
                print_error(parser, "Invalid second operand for CMP_U command.", cmd);
                free_command(cmd);
                return NULL;
            }

            cmd->is_a_immediate = is_immediate_a;
            cmd->is_b_immediate = is_immediate_b;

            if (parser->current.type != TOK_NL && parser->current.type != TOK_EOF) {
                print_error(parser, "Unexpected token after CMP_U command.", cmd);
                free_command(cmd);
                return NULL;
            }

            advance(parser);
            return cmd;
        }

        case TOK_PRINT: {
            advance(parser);
            Command *cmd = create_command(CMD_PRINT);
            if (!cmd) {
                print_error(parser, "Failed to allocate memory for PRINT command.", NULL);
                return NULL;
            }

            if (!is_base(parser->current)) {
                print_error(parser, "Invalid base for PRINT command.", cmd);
                free_command(cmd);
                return NULL;
            }

            cmd->val_a.str_val = strdup(parser->current.lexeme);
            advance(parser);

            bool is_immediate = false;
            if (!parse_var_or_imm(parser, &cmd->val_b, &is_immediate)) {
                print_error(parser, "Invalid operand for PRINT command.", cmd);
                free_command(cmd);
                return NULL;
            }

            cmd->is_b_immediate = is_immediate;

            if (parser->current.type != TOK_NL && parser->current.type != TOK_EOF) {
                print_error(parser, "Unexpected token after PRINT command.", cmd);
                free_command(cmd);
                return NULL;
            }

            advance(parser);
            return cmd;
        }

        default:
            print_error(parser, "Unrecognized command.", NULL);
            while (parser->current.type != TOK_NL && parser->current.type != TOK_EOF) {
                advance(parser);
            }
            return NULL;
    }
}

/**
 * @brief Parses commands into a linked list.
 *
 * @param parser A pointer to the parser to read tokens from.
 * @return A pointer to the head of the command list, or NULL if no commands
 * were parsed.
 */
Command *parse_commands(Parser *parser) {
    Command *head = NULL;
    Command *tail = NULL;

    while (!is_at_end(parser)) {
        Command *cmd = parse_cmd(parser);

        if (cmd) {
            if (!head) {
                head = cmd;
            } else {
                tail->next = cmd;
            }
            tail = cmd;
        }

        if (parser->had_error) {
            while (parser->current.type != TOK_NL && parser->current.type != TOK_EOF) {
                advance(parser);
            }
            parser->had_error = false;
        }
    }
    print_commands(head);
    return head;
}

