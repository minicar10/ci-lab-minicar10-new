#include "interpreter.h"
#include <inttypes.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

#include "command_type.h"
#include "mem.h"

static bool    cond_holds(Interpreter *intr, BranchCondition cond);
static int64_t fetch_number_value(Interpreter *intr, Operand *op, bool is_im);
static bool    print_base(Interpreter *intr, Command *cmd);

void interpreter_init(Interpreter *intr, LabelMap *map) {
    if (!intr) {
        return;
    }

    intr->had_error  = false;
    intr->label_map  = map;
    intr->is_greater = false;
    intr->is_equal   = false;
    intr->is_less    = false;
    intr->the_stack  = NULL;

    for (size_t i = 0; i < NUM_VARIABLES; i++) {
        intr->variables[i] = 0;
    }
}

void interpret(Interpreter *intr, Command *commands) {
    if (!intr || !commands) {
        return;
    }

    Command *current = commands;
    while (current && !intr->had_error) {
        switch (current->type) {
            // STUDENT TODO: process the commands and take actions as appropriate
            default:
                break;
        }
    }

    // Week 4: free the stack at the end
}

void print_interpreter_state(Interpreter *intr) {
    if (!intr) {
        return;
    }

    printf("Error: %d\n", intr->had_error);
    printf("Flags:\n");
    printf("Is greater: %d\n", intr->is_greater);
    printf("Is equal: %d\n", intr->is_equal);
    printf("Is less: %d\n", intr->is_less);

    printf("\n");

    printf("Variable values:\n");
    for (size_t i = 0; i < NUM_VARIABLES; i++) {
        printf("x%zu: %" PRId64 "", i, intr->variables[i]);

        if (i < NUM_VARIABLES - 1) {
            printf(", ");
        }

        if ((i + 1) % 8 == 0) {
            printf("\n");
        }
    }

    printf("\n");
}

/**
 * @brief Fetches the appropriate value from the given operand.
 *
 * @param intr The pointer to the interpreter holding variable state.
 * @param op The operand used to fetch the value.
 * @param is_im A boolean representing whether this value is an immediate or
 * must be read in from the interpreter state.
 * @return The fetched value.
 */
static int64_t fetch_number_value(Interpreter *intr, Operand *op, bool is_im) {
    // STUDENT TODO: Fetch either a variable from the interpreter's state or directly output a value
    return -1;
}

/**
 * @brief Determines whether a given branch condition holds.
 *
 * @param intr The pointer to the interpreter holding the result of the
 * comparison.
 * @param cond The condition to check.
 * @return True if the given condition holds, false otherwise.
 */
static bool cond_holds(Interpreter *intr, BranchCondition cond) {
    // STUDENT TODO: Determine whether a given condition holds using the interpreter's state
    return false;
}

/**
 * @brief Prints the given command's value in a specified base.
 *
 * @param intr The pointer to the interpreter holding variable state.
 * @param cmd The command being processed.
 * @return True whether the print was successful, false otherwise.
 */
static bool print_base(Interpreter *intr, Command *cmd) {
    // STUDENT TODO: Print the given value respecting the appropriate base
    return false;
}
