#ifndef CI_LEXER_H
#define CI_LEXER_H
#include "token.h"

/**
 * @brief Represents the state of a lexical analyzer.
 */
typedef struct {
    const char *start_position;  // Pointer to the start of the current token.

    const char *current_position;  // Pointer to the current position within the
                                   // source string, I.e, the character that is
                                   // about to be consumed.

    int current_line;  // The current line number in the source string.

    int current_column;  // The current column number in the source string.
} Lexer;

/**
 * @brief Initializes the given lexer with the passed in string.
 *
 * @param lex The input stream to initialize.
 * @param text A pointer to the string to lex.
 */
void lexer_init(Lexer *lex, const char *text);

/**
 * @brief Yields the next token in the input stream.
 *
 * @param lex A pointer to the lexer, the input stream.
 * @return The next lexed token within the stream.
 */
Token lexer_next_token(Lexer *lex);

/**
 * @brief Prints the lexed tokens, consuming the input stream.
 *
 * If called, the given lexer must be reinitialized if one wishes to re-lex the
 * tokens.
 *
 * @param lex A pointer to the lexer, the input stream.
 */
void print_lexed_tokens(Lexer *lex);

#endif
