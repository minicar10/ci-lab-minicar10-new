#ifndef CI_MEMORY_H
#define CI_MEMORY_H
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define MEM_CAPACITY 1024  // Maximum capacity of available memory.

/**
 * @brief Loads the value from memory into the given destination.
 *
 * @param destination The buffer to load values into.
 * @param offset The offset in memory where to start loading from.
 * @param bytes The amount of bytes to load starting from the given offset.
 * @return True if the value could be loaded, false otherwise.
 */
bool mem_load(uint8_t *destination, size_t offset, size_t bytes);

/**
 * @brief Stores the given value at the specified memory address.
 *
 * @param source The buffer to read the value from.
 * @param offset The offset in memory where to start storing.
 * @param bytes The amount of bytes to store starting at `offset`.
 * @return True if the value was stored, false otherwise.
 */
bool mem_store(uint8_t *source, size_t offset, size_t bytes);

/**
 * @brief Prints the memory state to the console
 */
void mem_print(void);

#endif
