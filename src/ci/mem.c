#include "mem.h"
#include <stdio.h>
#include <string.h>

static uint8_t mem[MEM_CAPACITY];

static bool validate_bytes(size_t bytes);

/**
 * @brief Verifies that the given amount of `bytes` is valid to load.
 *
 * Valid amounts are 1, 2, 4, and 8.
 *
 * @param bytes The amount to verify.
 * @return True if the amount is valid, false otherwise.
 */
static bool validate_bytes(size_t bytes) {
    return bytes == 1 || bytes == 2 || bytes == 4 || bytes == 8;
}

bool mem_load(uint8_t *destination, size_t offset, size_t bytes) {
    if (!validate_bytes(bytes) || !destination || offset + bytes > MEM_CAPACITY) {
        return false;
    }

    memcpy(destination, &mem[offset], bytes);
    return true;
}

bool mem_store(uint8_t *source, size_t offset, size_t bytes) {
    if (!validate_bytes(bytes) || !source || offset + bytes > MEM_CAPACITY) {
        return false;
    }

    memcpy(&mem[offset], source, bytes);
    return true;
}

void mem_print(void) {
    printf("Memory state:\n");

    // Calculate minimum hex digits needed based on capacity
    int    addr_width = 1;
    size_t temp       = MEM_CAPACITY - 1;
    while (temp >>= 4) {
        addr_width++;
    }

    size_t first_modified = 0;
    while (first_modified < MEM_CAPACITY && mem[first_modified] == 0) {
        first_modified++;
    }

    if (first_modified == MEM_CAPACITY) {
        printf("Unmodified\n");
        return;
    }

    size_t last_modified = MEM_CAPACITY - 1;
    while (last_modified > first_modified && mem[last_modified] == 0) {
        last_modified--;
    }

    size_t display_start = first_modified & ~0xF;
    size_t display_end   = (last_modified + 16) & ~0xF;
    if (display_end > MEM_CAPACITY)
        display_end = MEM_CAPACITY;

    printf("0x%0*zx-0x%0*zx:\n", addr_width, display_start, addr_width, display_end - 1);

    for (size_t j = display_start; j < display_end; j += 16) {
        printf("    0x%0*zx: ", addr_width, j);
        for (size_t k = 0; k < 16 && j + k < display_end; k++) {
            printf("%02x", mem[j + k]);
            if ((k + 1) % 4 == 0) {
                printf(" ");
            }
        }
        printf("\n");
    }
}