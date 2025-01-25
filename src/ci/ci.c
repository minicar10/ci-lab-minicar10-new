#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cmd_args_config.h"
#include "command.h"
#include "interpreter.h"
#include "label_map.h"
#include "lexer.h"
#include "mem.h"
#include "parser.h"
#include "token.h"
#include "token_type.h"
#include <ctype.h>

#define CAPACITY 50

static int   run_interpreter(CmdArgsConfig *conf);
static char *run_repl(void);
static char *read_file(const char *path);
static int   run_file(const char *src, bool print_lex, bool print_parse);

int main(int argc, char **argv) {
    CmdArgsConfig conf = {false, false, false, NULL, NULL};
    if (!parse_cmd_args(&conf, argv + 1, argc - 1)) {
        printf("Aborting\n");
        config_free(&conf);
        return 1;
    }
    FILE *file = NULL;
    if (conf.out_filename != NULL) {
        file = freopen(conf.out_filename, "w", stdout);
        if (file == NULL) {
            perror("Failed to redirect stdout");
            return 1;
        }
    }

    int status = run_interpreter(&conf);
    config_free(&conf);
    if (file) {
        fclose(file);
    }
    return status;
}

static int run_interpreter(CmdArgsConfig *conf) {
    char *src;
    int   status;

    if (conf->repl) {
        src = run_repl();
        if (!src) {
            return -1;
        }
    } else {
        if (conf->in_filename == NULL) {
            printf("No file specified.\n");
            return -1;
        }
        src = read_file(conf->in_filename);
        if (!src) {
            return -1;
        }
    }
    status = run_file(src, conf->print_lex, conf->print_parse);
    free(src);
    return status;
}

static char *run_repl(void) {
    // Allocate initial buffer
    char *buffer = (char *) malloc(CAPACITY * sizeof(char));
    if (!buffer) {
        printf("Could not allocate memory for REPL buffer\n");
        return NULL;
    }

    size_t total_size   = CAPACITY;
    size_t current_size = 0;
    char   line[256];
    bool   continue_input = true;

    printf("Enter commands:\n");

    while (continue_input) {
        printf("CI> ");
        if (!fgets(line, sizeof(line), stdin)) {
            break;
        }

        size_t line_len = strlen(line);

        // Check if we need to resize the buffer
        if (current_size + line_len >= total_size) {
            total_size *= 2;
            char *new_buffer = (char *) realloc(buffer, total_size);
            if (!new_buffer) {
                printf("Could not allocate more memory for REPL buffer\n");
                free(buffer);
                return NULL;
            }
            buffer = new_buffer;
        }

        strcpy(buffer + current_size, line);
        current_size += line_len;

        // Check if line ends with semicolon
        bool has_semicolon = false;
        for (int i = line_len - 1; i >= 0; i--) {
            if (line[i] == ';') {
                has_semicolon = true;
                break;
            }
            if (!isspace(line[i])) {
                break;
            }
        }

        if (!has_semicolon) {
            continue_input = false;
        }
    }

    return buffer;
}

static char *read_file(const char *path) {
    FILE *file = fopen(path, "rb");
    if (!file) {
        printf("Failed to open file %s\n", path);
        return NULL;
    }

    fseek(file, 0L, SEEK_END);
    size_t filesize = ftell(file);
    rewind(file);

    char *buffer = (char *) calloc(filesize + 1, sizeof(char));
    if (!buffer) {
        printf("Could not allocate enough space for %s\n", path);
        return NULL;
    }

    size_t bytes_read = fread(buffer, sizeof(char), filesize, file);
    if (bytes_read < filesize) {
        printf("Could not read %s\n", path);
    }

    fclose(file);
    return buffer;
}

static int run_file(const char *src, bool print_lex, bool print_parse) {
    Lexer l;
    lexer_init(&l, src);
    if (print_lex) {
        print_lexed_tokens(&l);
        // Reset so we can parse
        lexer_init(&l, src);
    }

    LabelMap lbm;
    if (!label_map_init(&lbm, 100)) {
        printf("Unable to allocate label hashmap. Aborting\n");
        return -1;
    }

    Parser p;
    parser_init(&p, &l, &lbm);
    Command *commands = parse_commands(&p);
    if (print_parse) {
        print_commands(commands);
    }

    if (p.had_error) {
        printf("Parser encountered an error:\n");
        printf("At ");
        print_token(p.current);
        printf("\nParsed commands up to this point:\n");
        print_commands(commands);
        free_command(commands);
        label_map_free(&lbm);
        return -1;
    }

    Interpreter i;
    interpreter_init(&i, &lbm);
    interpret(&i, commands);
    print_interpreter_state(&i);
    mem_print();

    free_command(commands);
    label_map_free(&lbm);

    return (i.had_error) ? -1 : 0;
}
