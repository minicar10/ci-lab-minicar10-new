#include "lexer.h"

#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "token_type.h"

static const char *BAD_BASE_MSG = "Either no or invalid digit in the specified base";

/**
 * @brief Structure representing a keyword and its corresponding token type.
 */
typedef struct {
    const char *name;   /** The keyword string */
    int         length; /** The length of the keyword */
    TokenType   type;   /** The token type to return if matched */
} Keyword;

/**
 * @brief Array of reserved keywords.
 */
static const Keyword keywords[] = {
    {"add", 3, TOK_ADD},         {"and", 3, TOK_AND},        {"asr", 3, TOK_ASR},
    {"b", 1, TOK_BRANCH},        {"b.eq", 4, TOK_BRANCH_EQ}, {"b.gt", 4, TOK_BRANCH_GT},
    {"b.ge", 4, TOK_BRANCH_GE},  {"b.lt", 4, TOK_BRANCH_LT}, {"b.le", 4, TOK_BRANCH_LE},
    {"b.ne", 4, TOK_BRANCH_NEQ}, {"call", 4, TOK_CALL},      {"cmp", 3, TOK_CMP},
    {"cmp_u", 5, TOK_CMP_U},     {"eor", 3, TOK_EOR},        {"load", 4, TOK_LOAD},
    {"lsl", 3, TOK_LSL},         {"lsr", 3, TOK_LSR},        {"mov", 3, TOK_MOV},
    {"orr", 3, TOK_ORR},         {"print", 5, TOK_PRINT},    {"put", 3, TOK_PUT},
    {"ret", 3, TOK_RET},         {"store", 5, TOK_STORE},    {"sub", 3, TOK_SUB},
};

// Calculate on the fly so you only have to modify the array
static const int num_keywords = sizeof(keywords) / sizeof(keywords[0]);

static char advance(Lexer *lex);
static bool is_at_end(Lexer *lex);
static char peek(Lexer *lex);
static char peek_next(Lexer *lex);
static void skip_whitespace(Lexer *lex);

static Token make_token(Lexer *lex, TokenType tok_type);
static Token error_token(Lexer *lex, const char *message);

static Token     make_ident(Lexer *lex);
static TokenType ident_type(Lexer *lex);
static Token     make_number(Lexer *lex, char first_digit);
static Token     make_binary(Lexer *lex);
static Token     make_hex(Lexer *lex);
static Token     make_string(Lexer *lex);

static bool is_alpha(char c);
static bool is_digit(char c);
static bool is_hex(char c);
static bool is_binary(char c);

void lexer_init(Lexer *lex, const char *text) {
    if (!lex) {
        return;
    }

    lex->start_position   = text;
    lex->current_position = text;
    lex->current_line     = 1;
    lex->current_column   = 1;
}

/**
 * @brief Advances the lexer by one character in the given text.
 *
 * @param lex A pointer to the lexer, the input stream.
 * @return The character that was consumed.
 */
static char advance(Lexer *lex) {
    lex->current_position++;
    lex->current_column++;
    return lex->current_position[-1];
}

/**
 * @brief Determines if the lexer is at the end of the given string.
 *
 * @param lex A pointer to the lexer, the input stream.
 * @return True if the lexer is at the end, false otherwise.
 */
static bool is_at_end(Lexer *lex) {
    return *lex->current_position == '\0';
}

/**
 * @brief Creates a token with the specified type.
 *
 * @param lex A pointer to the lexer, the input stream.
 * @param tok_type The type of the token.
 * @return The token with the indicated type.
 */
static Token make_token(Lexer *lex, TokenType tok_type) {
    Token token;
    int   tok_len = (int) (lex->current_position - lex->start_position);
    token_init(&token, tok_type, lex->start_position, tok_len, lex->current_line,
               lex->current_column - tok_len);
    return token;
}

/**
 * @brief Creates an error token with the given message.
 *
 * @param lex A pointer to the lexer, the input stream.
 * @param message The error message this token should contain.
 * @return The error token.
 */
static Token error_token(Lexer *lex, const char *message) {
    Token token;
    token_init(&token, TOK_ERR, message, strlen(message), lex->current_line,
               lex->current_column - 1);
    return token;
}

/**
 * @brief Skips over the whitespace in the input stream.
 *
 * Whitespace characters are considered to be space (' '), tab ('\t'), comma
 * (,), and carriage return ('\r').
 *
 * @param lex A pointer to the lexer, the input stream.
 */
static void skip_whitespace(Lexer *lex) {
    for (;;) {
        char c = peek(lex);
        switch (c) {
            case ' ':
            case ',':
            case '\r':
            case '\t':
                advance(lex);
                break;
            case '/':
                if (peek_next(lex) == '/') {
                    // Go to end of line
                    while (peek(lex) != '\n' && !is_at_end(lex)) {
                        advance(lex);
                    }
                } else {
                    // Found *only* one slash
                    return;
                }
            default:
                return;
        }
    }
}

Token lexer_next_token(Lexer *lex) {
    skip_whitespace(lex);
    lex->start_position = lex->current_position;
    if (is_at_end(lex)) {
        return make_token(lex, TOK_EOF);
    }

    char c = advance(lex);
    if (is_alpha(c)) {
        return make_ident(lex);
    } else if (is_digit(c)) {
        return make_number(lex, c);
    } else if (c == '\n' || c == ';') {
        Token t = make_token(lex, TOK_NL);
        if (c == '\n') {
            lex->current_line++;
            lex->current_column = 1;
        }
        return t;
    } else if (c == ':') {
        return make_token(lex, TOK_COLON);
    } else if (c == '"') {
        return make_string(lex);
    }

    return error_token(lex, "Unexpected character");
}

/**
 * @brief Peeks at the current character in the input stream without consuming
 * it.
 *
 * @param lex A pointer to the lexer, the input stream.
 * @return The peeked character.
 */
static char peek(Lexer *lex) {
    return *lex->current_position;
}

/**
 * @brief Peeks at the next character in the input stream without consuming it.
 *
 * @param lex A pointer to the lexer, the input stream.
 * @return The peeked character.
 */
static char peek_next(Lexer *lex) {
    if (is_at_end(lex)) {
        return '\0';
    }
    return lex->current_position[1];
}

/**
 * @brief Determines if the given character is a valid alphabetic character, a-z
 * A-Z . _.
 *
 * @param c The character to check.
 * @return True if this character is alphabetic, false otherwise.
 */
static bool is_alpha(char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_' || c == '.';
}

/**
 * @brief Determines if the given character is a valid digit.
 *
 * @param c The character to check.
 * @return True if this character is a valid digit, false otherwise.
 */
static bool is_digit(char c) {
    return c >= '0' && c <= '9';
}

/**
 * @brief Determines whether the given character is a valid hexadecimal digit,
 * I.e, 0-9 a-f.
 *
 * @param c The character to check.
 * @return True if this character is hexadecimal, false otherwise.
 */
static bool is_hex(char c) {
    return is_digit(c) || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F');
}

/**
 * @brief Determines if the given character is binary, either 0 or 1.
 *
 * @param c The character to check.
 * @return True if this character is binary, false otherwise.
 */
static bool is_binary(char c) {
    return c == '0' || c == '1';
}

/**
 * @brief Creates an identifier token from the input stream.
 *
 * @param lex A pointer to the lexer, the input stream.
 * @return The token corresponding to the matched identifier.
 *
 * @note Recognizes whether the token is a reserved keyword or not.
 * Returns an appropriate type if this is the case.
 */
static Token make_ident(Lexer *lex) {
    while (is_alpha(peek(lex)) || is_digit(peek(lex))) {
        advance(lex);
    }

    return make_token(lex, ident_type(lex));
}

/**
 * @brief Determines whether the given identifier (word) is reserved.
 *
 * @param lex A pointer to the lexer, the input stream.
 * @return The appropriate token if the word is reserved, `TOK_IDENT` otherwise.
 */
static TokenType ident_type(Lexer *lex) {
    int token_length = lex->current_position - lex->start_position;

    for (int i = 0; i < num_keywords; i++) {
        if (token_length == keywords[i].length &&
            memcmp(lex->start_position, keywords[i].name, token_length) == 0) {
            return keywords[i].type;
        }
    }

    return TOK_IDENT;
}

/**
 * @brief Creates a numeric token from the input stream.
 *
 * This function parses a number from the lexer stream starting with a given
 * digit. It supports decimal, binary (`0b` prefix), and hexadecimal (`0x`
 * prefix) numbers.
 *
 * @param lex A pointer to the lexer, the input stream.
 * @param first_digit The already read in digit.
 * @return The token representing the parsed number.
 *
 * @note Returns an error token if there are no digits after the specified base.
 */
static Token make_number(Lexer *lex, char first_digit) {
    bool z = first_digit == '0';
    bool b = peek(lex) == 'b';
    bool h = peek(lex) == 'x';

    if (z && (b || h)) {
        advance(lex);
        return (b) ? make_binary(lex) : make_hex(lex);
    }

    char c = peek(lex);
    while (is_digit(c)) {
        advance(lex);
        c = peek(lex);
    }

    return make_token(lex, TOK_NUM);
}

/**
 * @brief Parses a binary number from the input stream.
 *
 * It is assumed that the number has already been checked for the prefix 0b.
 *
 * @param lex A pointer to the lexer, the input stream.
 * @return The token representing this binary number.
 *
 * @note Returns an error token if there are no digits.
 */
static Token make_binary(Lexer *lex) {
    char c = peek(lex);

    // Handle empty binary
    if (!is_binary(c)) {
        return error_token(lex, BAD_BASE_MSG);
    }

    while (is_binary(c)) {
        advance(lex);
        c = peek(lex);
    }

    return make_token(lex, TOK_NUM);
}

/**
 * @brief Parses a hexadecimal number from the input stream.
 *
 * It is assumed that the number has already been checked for the prefix 0x.
 *
 * @param lex A pointer to the lexer, the input stream.
 * @return The token representing this hexadecimal number.
 *
 * @note Returns an error token if there are no digits.
 */
static Token make_hex(Lexer *lex) {
    char c = peek(lex);

    // Handle empty hex
    if (!is_hex(c)) {
        return error_token(lex, BAD_BASE_MSG);
    }

    while (is_hex(c)) {
        advance(lex);
        c = peek(lex);
    }

    return make_token(lex, TOK_NUM);
}

/**
 * @brief Parses a string from the input stream
 *
 * @param lex A pointer to the lexer, the input stream.
 * @return A token representing this string, excluding the quotes
 */
static Token make_string(Lexer *lex) {
    while (peek(lex) != '"' && !is_at_end(lex)) {
        if (peek(lex) == '\n') {
            lex->current_line++;
            lex->current_column = 1;
        }

        advance(lex);
    }

    // We do a hack here to avoid storing the quotes
    lex->start_position++;
    Token t = make_token(lex, TOK_STR);
    advance(lex);
    return t;
}

void print_lexed_tokens(Lexer *lex) {
    bool should_stop = false;
    while (!should_stop) {
        Token t = lexer_next_token(lex);
        print_token(t);
        should_stop = t.type == TOK_ERR || t.type == TOK_EOF;
        if (!should_stop) {
            printf("\n");
        }
    }
}
