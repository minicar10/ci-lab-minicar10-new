#ifndef CI_COMMAND_H
#define CI_COMMAND_H
#include <stdbool.h>
#include <stdint.h>
#include "command_type.h"

/**
 * @brief Enum representing different branching conditions for commands.
 */
typedef enum {
    BRANCH_NONE = -1,
    BRANCH_ALWAYS,
    BRANCH_EQUAL,
    BRANCH_NOT_EQUAL,
    BRANCH_GREATER,
    BRANCH_LESS,
    BRANCH_GREATER_EQUAL,
    BRANCH_LESS_EQUAL,
} BranchCondition;

/**
 * @brief Union representing an operand, which can be an integer or a string.
 */
typedef union {
    int64_t num_val;
    char   *str_val;
    char    base;
} Operand;

/**
 * @brief Represents a command with operands, branching conditions, and
 * metadata.
 */
typedef struct cmd {
    CommandType type;                  // The type of the command.
    struct cmd *next;                  // Pointer to the next command in the sequence.
    Operand     destination;           // The destination variable to which this command will
                                       // write to (should always be an integer; string
                                       // variant unused).
    Operand         val_a;             // The first operand.
    Operand         val_b;             // The second operand.
    bool            is_a_immediate;    // Indicates if the first operand is an immediate.
    bool            is_b_immediate;    // Indicates if the second operand is immediate.
    bool            is_a_string;       // Indicates if the first operand is a string.
    bool            is_b_string;       // Indicates if the second operand is a string.
    BranchCondition branch_condition;  // The branching condition for the command.
} Command;

/**
 * @brief Frees memory associated with a command.
 *
 * @param command Pointer to the `Command` to be freed.
 */
void free_command(Command *command);

/**
 * @brief Prints the details of a command.
 *
 * Outputs information about the command, including its type, operands,
 * destination, and branching condition, in a human-readable format.
 *
 * @param cmd Pointer to the `Command` to print.
 */
void print_command(Command *cmd);

/**
 * @brief Prints the details of a single operand.
 *
 * Outputs the value of the operand and whether it is immediate or a string,
 * in a human-readable format.
 *
 * @param op The operand to print.
 * @param is_imm `true` if the operand is immediate, `false` otherwise.
 * @param is_str `true` if the operand is a string, `false` otherwise.
 */
void print_command_op(Operand op, bool is_imm, bool is_str);

/**
 * @brief Prints a list of commands.
 *
 * Outputs all commands in the given list, one by one, in a human-readable
 * format.
 *
 * @param cmd Pointer to the first `Command` in the list.
 */
void print_commands(Command *cmd);

#endif
