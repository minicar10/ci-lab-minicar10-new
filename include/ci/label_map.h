#ifndef CI_LABEL_MAP_H
#define CI_LABEL_MAP_H
#include "command.h"

/**
 * @brief Represents an entry in the label map.
 *
 * Each entry contains an identifier (label), a corresponding command,
 * and a pointer to the next entry in the chain used for handling collisions.
 */
typedef struct entry {
    char         *id;       // The identifier for this label.
    Command      *command;  // The command associated with this label.
    struct entry *next;     // Pointer to the next entry in the chain.
} Entry;

/**
 * @brief Represents a hash map for managing labels.
 */
typedef struct {
    Entry **entries;   // Array of pointers to entry chains.
    int     capacity;  // The number of buckets in the map.
} LabelMap;

/**
 * @brief Initializes a label map with the specified capacity.
 *
 * @param map Pointer to the `LabelMap` to initialize.
 * @param capacity The number of buckets to allocate for the map.
 * @return true if the map was successfully initialized, false otherwise.
 */
bool label_map_init(LabelMap *map, int capacity);

/**
 * @brief Frees the resources associated with a label map.
 *
 * Releases all memory allocated for the map, including entries and their
 * contents.
 *
 * @param map Pointer to the LabelMap to free.
 */
void label_map_free(LabelMap *map);

/**
 * @brief Inserts a label and its associated command into the map.
 *
 * Adds a new label and its associated command to the label map. If a label
 * with the same ID already exists, its associated command will be replaced.
 *
 * @param map Pointer to the label map.
 * @param id The identifier for the label.
 * @param command Pointer to the `Command` associated with the label.
 * @return true if the label was successfully added, false otherwise.
 */
bool put_label(LabelMap *map, char *id, Command *command);

/**
 * @brief Retrieves a label's entry from the map.
 *
 * Searches the label map for the given ID and returns the associated entry.
 *
 * @param map Pointer to the label map.
 * @param id The identifier for the label to retrieve.
 * @return A pointer to the `Entry` if the label exists, or NULL if not found.
 */
Entry *get_label(LabelMap *map, char *id);

#endif
