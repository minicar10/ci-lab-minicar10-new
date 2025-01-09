#include "cmd_args_config.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void config_free(CmdArgsConfig *conf) {
    if (!conf) {
        return;
    }

    free(conf->in_filename);
    free(conf->out_filename);
    conf->in_filename  = NULL;
    conf->out_filename = NULL;
}

bool parse_cmd_args(CmdArgsConfig *conf, char **args, int arg_count) {
    if (!conf) {
        return true;  // No config, no problem
    }

    if (arg_count == 0) {
        conf->repl = true;
    }

    for (int i = 0; i < arg_count; i++) {
        if (strncmp(args[i], "-l", 2) == 0) {
            conf->print_lex = true;
        } else if (strncmp(args[i], "-p", 2) == 0) {
            conf->print_parse = true;
        } else if (strncmp(args[i], "-i", 2) == 0) {
            i++;
            if (i >= arg_count) {
                printf("Filename not specified\n");
                return false;
            }

            conf->in_filename = calloc(strlen(args[i]) + 1, sizeof(char));
            if (!conf->in_filename) {
                printf("Failed to allocate space for filename\n");
                return false;
            }

            strcpy(conf->in_filename, args[i]);
        } else if (strncmp(args[i], "-o", 2) == 0) {
            i++;
            if (i >= arg_count) {
                printf("Filename not specified\n");
                return false;
            }

            conf->out_filename = calloc(strlen(args[i]) + 1, sizeof(char));
            if (!conf->out_filename) {
                printf("Failed to allocate space for filename\n");
                return false;
            }

            strcpy(conf->out_filename, args[i]);
        }
    }

    return true;
}
