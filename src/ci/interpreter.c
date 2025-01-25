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
            case CMD_ADD: {
                int64_t val_a = current->is_a_immediate ? current->val_a.num_val : intr->variables[(int) current->val_a.base];
                int64_t val_b = current->is_b_immediate ? current->val_b.num_val : intr->variables[(int) current->val_b.base];
                intr->variables[(int) current->destination.base] = val_a + val_b;

                current = current->next;
                break;
            }

            case CMD_SUB: {
                int64_t val_a = current->is_a_immediate ? current->val_a.num_val : intr->variables[(int) current->val_a.base];
                int64_t val_b = current->is_b_immediate? current->val_b.num_val : intr->variables[(int) current->val_b.base];
                intr->variables[(int) current->destination.base] = val_a - val_b;

                current = current->next;
                break;
            }

            case CMD_MOV: {
                int64_t value = current->is_a_immediate ? current->val_a.num_val: intr->variables[(int) current->val_a.base];
                intr->variables[(int) current->destination.base] = value;

                current = current->next;
                break;
            }

            case CMD_CMP: {
                int64_t val_a = current->is_a_immediate ? current->val_a.num_val : intr->variables[(int) current->val_a.base];
                int64_t val_b = current->is_b_immediate ? current->val_b.num_val : intr->variables[(int) current->val_b.base];

                intr->is_greater = (val_a > val_b);
                intr->is_equal   = (val_a == val_b);
                intr->is_less    = (val_a < val_b);

                current = current->next;
                break;
            }

            case CMD_CMP_U: {
                uint64_t val_a = current->is_a_immediate ? (uint64_t) current->val_a.num_val : (uint64_t) intr->variables[(int) current->val_a.base];
                uint64_t val_b = current->is_b_immediate ? (uint64_t) current->val_b.num_val : (uint64_t) intr->variables[(int) current->val_b.base];

                intr->is_greater = (val_a > val_b);
                intr->is_equal   = (val_a == val_b);
                intr->is_less    = (val_a < val_b);

                current = current->next;
                break;
            }

            case CMD_PRINT: {
                const char *base = current->val_a.str_val;
                int64_t value = current->is_b_immediate ? current->val_b.num_val : intr->variables[(int) current->val_b.base];

                if (base[0] == 'd') {
                    printf("%" PRId64 "\n", value); 
                } else if (base[0] == 'x') {
                    printf("%" PRIx64 "\n", (uint64_t) value); 
                } else if (base[0] == 'b') {
                    for (int i = 63; i >= 0; i--) {
                        printf("%c", (value & ((int64_t) 1 << i)) ? '1' : '0');
                    }
                    printf("\n");
                } else {
                    intr->had_error = true;
                }

                current = current->next;
                break;
            }

            default:
                intr->had_error = true;
                current = current->next;
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
    if (is_im) {
        return op->num_val;  // Return the immediate value
    } else {
        return intr->variables[op->num_val];  // Return the value from the variable
    }
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