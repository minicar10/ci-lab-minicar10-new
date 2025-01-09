#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "label_map.h"

static void          free_entry(Entry *e);
static void          free_entries(Entry *e);
static unsigned long hash_function(char *s);
static Entry        *entry_init(char *id, Command *command);

bool label_map_init(LabelMap *map, int capacity) {
    // STUDENT TODO: Initialize the fields of the label map
    return true;
}

/**
 * @brief Frees the resources for a `singular` entry.
 *
 * @param e The pointer to the entry to free.
 */
static void free_entry(Entry *e) {
    // STUDENT TODO: Free a *singular* entry
    // Do not free children; see below
}

/**
 * @brief Frees the given list of entries.
 *
 * @param e A pointer to the first entry to free.
 */
static void free_entries(Entry *e) {
    // STUDENT TODO: Free an entry and its children
}

void label_map_free(LabelMap *map) {
    // STUDENT TODO: Free the entries associated with this hashmap
    // Do not free the pointer itself, as you do not know whether it was allocated on the heap
}

/**
 * @brief Returns a hash of the specified id.
 *
 * @param s The string to hash.
 * @return The hash of `s`
 */
static unsigned long hash_function(char *s) {
    unsigned long i = 0;
    for (int j = 0; s[j]; j++) {
        i += s[j];
    }

    return i;
}

/**
 * @brief Initializes the given entry's state.
 *
 * @param id The id associated with this entry.
 * @param command The command associated with this entry.
 * @return True if the entry was initialized successfully, false otherwise.
 */
static Entry *entry_init(char *id, Command *command) {
    // STUDENT TODO: Initialize the entry and its fields
    return NULL;
}

bool put_label(LabelMap *map, char *id, Command *command) {
    // STUDENT TODO: Put the given command into the bucket associated with this label
    // It is okay for the command to be null
    return false;
}

Entry *get_label(LabelMap *map, char *id) {
    // STUDENT TODO: Retrieve the value associated with this label
    return NULL;
}
