#ifndef CI_INTERPRETER_H
#define CI_INTERPRETER_H
#include "command.h"
#include "label_map.h"

#define NUM_VARIABLES 32  // Maximum number of defined variables.

/**
 * @brief Represents a single entry in the interpreter's call stack.
 */
typedef struct st_entry {
    Command         *command;                   // The command stored in this stack entry.
    int64_t          variables[NUM_VARIABLES];  // Variables in this stack frame.
    struct st_entry *next;                      // Pointer to the next stack entry.
} StackEntry;

/**
 * @brief Represents the state of the interpreter during execution.
 */
typedef struct {
    int64_t variables[NUM_VARIABLES];  // Array of variables used in the
                                       // interpreter.
    bool had_error;                    // Flag indicating if an error occurred during
                                       // interpretation.
    LabelMap *label_map;               // Pointer to the map of labels for branch resolution.
    bool      is_greater;              //  Flag indicating the result of the last comparison
                                       //  (greater).
    bool        is_less;               // Flag indicating the result of the last comparison (less).
    bool        is_equal;              // Flag indicating the result of the last comparison (equal).
    StackEntry *the_stack;             // Pointer to the top of the interpreter's stack.
} Interpreter;

/**
 * @brief Initializes the interpreter state.
 *
 * @param intr Pointer to the `Interpreter` to initialize.
 * @param map Pointer to the `LabelMap` used for jump resolution.
 */
void interpreter_init(Interpreter *intr, LabelMap *map);

/**
 * @brief Executes a list of commands using the interpreter.
 *
 * @param intr Pointer to the `Interpreter` that will execute the commands.
 * @param commands Pointer to the first `Command` in the list of commands to
 * interpret.
 */
void interpret(Interpreter *intr, Command *commands);

/**
 * @brief Prints the current state of the interpreter.
 *
 * Outputs the interpreter's variables, flags, stack state, and other relevant
 * information in a human-readable format.
 *
 * @param intr Pointer to the `Interpreter` whose state is to be printed.
 */
void print_interpreter_state(Interpreter *intr);

#endif
