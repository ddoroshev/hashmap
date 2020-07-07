#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "alloc/alloc.h"
#include "array.h"

struct array {
    uint32_t length;
    uint32_t item_size;

    void **items;
};

array *array_init(uint32_t length, uint32_t item_size)
{
    array *ar = calloc(1, sizeof(array));
    if (ar == NULL) {
        return NULL;
    }
    ar->items = calloc(length, item_size + sizeof(void*));
    if (ar->items == NULL) {
        free(ar);
        return NULL;
    }
    ar->length = length;
    ar->item_size = item_size;
    return ar;
}

uint32_t array_len(array *ar)
{
    return ar->length;
}

int array_set(array *ar, uint32_t index, void *item)
{
    if (index < 0 || index >= ar->length) {
        return -E_ARRAY_INDEX_OUT_OF_RANGE;
    }
    ar->items[index] = calloc(1, sizeof(void*));
    if (ar->items[index] == NULL) {
        return -E_ALLOC;
    }
    memcpy(ar->items[index], item, ar->item_size);

    return 0;
}

void *array_get(array *ar, uint32_t index)
{
    if (index >= ar->length) {
        return NULL;
    }
    return ar->items[index];
}

void array_free(array *ar)
{
    for (int i = ar->length - 1; i >= 0; i--) {
        array_delete(ar, i);
    }
    free(ar->items);
    ar->items = NULL;
    free(ar);
}

int array_delete(array *ar, uint32_t index)
{
    if (index >= ar->length) {
        return -E_ARRAY_INDEX_OUT_OF_RANGE;
    }
    free(ar->items[index]);
    ar->items[index] = NULL;
    ar->length--;
    return 0;
}

int array_empty(array *ar)
{
    return ar->items == NULL;
}
