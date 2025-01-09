#ifndef CI_TOKEN_H
#define CI_TOKEN_H
#include "token_type.h"

/**
 * @brief Represents a lexical token produced by the lexer.
 */
typedef struct {
    TokenType   type;    // The type of this token.
    const char *lexeme;  // Pointer to the start of this token in the source text.
    int         length;  // The length of the token.
    int         line;    // The line number where this token is located (1-based).
    int         column;  // The column number where this token starts (1-based).
} Token;

/**
 * @brief Initializes a `Token` structure.
 *
 * @param tok Pointer to the `Token` to initialize.
 * @param tok_type The type of the token.
 * @param lexeme Pointer to the start of the token text.
 * @param lexeme_length The length of the token text.
 * @param line The line number where the token starts.
 * @param column The column number where the token starts.
 */
void token_init(Token *tok, TokenType tok_type, const char *lexeme, int lexeme_length, int line,
                int column);

/**
 * @brief Prints a `Token` for debugging purposes.
 *
 * Outputs the token's type, lexeme, and positional information in a
 * human-readable format.
 *
 * @param tok The token to print.
 */
void print_token(Token tok);

#endif
