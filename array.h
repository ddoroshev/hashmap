#pragma once

#include <stdint.h>

#define E_ARRAY_INDEX_OUT_OF_RANGE 1
#define E_ALLOC 2

typedef struct array {
    uint32_t length;
    uint32_t item_size;

    void **items;
} array;

array *array_init(uint32_t length, uint32_t item_size);
int array_set(array *ar, uint32_t index, void *item);
void array_free(array *ar);
