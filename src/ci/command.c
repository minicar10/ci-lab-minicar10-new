#include "command.h"
#include <inttypes.h>
#include <stdio.h>

void free_command(Command *command) {
    // STUDENT TODO: Free the command and all its children
}

void print_command(Command *cmd) {
    printf("Command type: %u\n", cmd->type);
    printf("Destination: %" PRId64 "\n", cmd->destination.num_val);
    printf("Operands:\n");
    printf("A:\n");
    print_command_op(cmd->val_a, cmd->is_a_immediate, cmd->is_a_string);
    printf("\n");
    printf("B:\n");
    print_command_op(cmd->val_b, cmd->is_b_immediate, cmd->is_b_string);
    printf("\n");
    printf("Branch condition: %d\n", cmd->branch_condition);
    printf("\n\n");
}

void print_command_op(Operand op, bool is_imm, bool is_str) {
    printf("Is immediate: %d\n", is_imm);
    printf("Is a string: %d\n", is_str);
    printf("Value: ");
    if (!is_str) {
        printf("%" PRId64 "", op.num_val);
    } else {
        printf("%s", op.str_val);
    }

    printf("\n");
}

void print_commands(Command *cmd) {
    if (!cmd) {
        printf("No commands found.\n");
    }

    while (cmd) {
        print_command(cmd);
        cmd = cmd->next;
        if (cmd) {
            printf("\n");
        }
    }
}
