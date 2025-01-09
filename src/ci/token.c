#include "token.h"
#include <stdio.h>

void token_init(Token *tok, TokenType tok_type, const char *lexeme, int lexeme_length, int line,
                int column) {
    if (!tok) {
        return;
    }

    tok->type   = tok_type;
    tok->lexeme = lexeme;
    tok->length = lexeme_length;
    tok->line   = line;
    tok->column = column;
}

void print_token(Token tok) {
    printf("Token: ");
    if (tok.type == TOK_EOF) {
        printf("EOF");
    } else if (tok.type == TOK_NL) {
        printf("Newline");
    } else {
        printf("%.*s", tok.length, tok.lexeme);
    }
    printf("\n");

    printf("Token type: %u\n", tok.type);
    printf("Token length: %d\n", tok.length);
    printf("Line: %d:%d\n", tok.line, tok.column);
}
