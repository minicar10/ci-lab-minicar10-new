#ifndef CI_CMD_ARGS_CONFIG_H
#define CI_CMD_ARGS_CONFIG_H
#include <stdbool.h>

typedef struct {
    bool  print_lex;     // Lex; do not parse
    bool  print_parse;   // Print result of parsing. Implicitly performs lexing
    bool  repl;          // Set when no arguments are supplied
    char *in_filename;   // What are we running?
    char *out_filename;  // File to output to
} CmdArgsConfig;

void config_free(CmdArgsConfig *conf);
bool parse_cmd_args(CmdArgsConfig *conf, char **args, int arg_count);

#endif
