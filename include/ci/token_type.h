#ifndef CI_TOKEN_TYPE_H
#define CI_TOKEN_TYPE_H

/**
 * @brief A listing of valid token types that could be outputted by the lexer
 *
 * Provides a listing of tokens that could be emitted during lexical analysis.
 */
typedef enum {
    TOK_ADD,         // add
    TOK_AND,         // and
    TOK_ASR,         // asr
    TOK_BRANCH,      // b, unconditional branch
    TOK_BRANCH_EQ,   // b.eq
    TOK_BRANCH_GE,   // b.ge
    TOK_BRANCH_GT,   // b.gt
    TOK_BRANCH_LE,   // b.le
    TOK_BRANCH_LT,   // b.lt
    TOK_BRANCH_NEQ,  // b.neq
    TOK_CALL,        // call
    TOK_CMP,         // cmp
    TOK_CMP_U,       // cmp_u
    TOK_COLON,       // :
    TOK_EOF,         // End of file
    TOK_EOR,         // eor
    TOK_ERR,         // Error token
    TOK_IDENT,       // X, B, etc.
    TOK_LOAD,        // load
    TOK_LSL,         // lsl
    TOK_LSR,         // lsr
    TOK_MOV,         // mov
    TOK_NL,          // \n
    TOK_NUM,         // 0b10101 or 12345 or 0xabcdef
    TOK_ORR,         // orr
    TOK_PRINT,       // print
    TOK_PUT,         // put
    TOK_RET,         // ret
    TOK_STORE,       // store
    TOK_STR,         // "string"
    TOK_SUB,         // sub
} TokenType;

#endif
