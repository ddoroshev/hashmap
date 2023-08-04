#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "alloc.h"
#include "array.h"

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

int array_set(array *ar, uint32_t index, void *item)
{
    if (index >= ar->length) {
        return -E_ARRAY_INDEX_OUT_OF_RANGE;
    }
    if(ar->items[index] != NULL) {
        free(ar->items[index]);
    }
    ar->items[index] = calloc(1, ar->item_size);
    if (ar->items[index] == NULL) {
        return -E_ALLOC;
    }
    memcpy(ar->items[index], item, ar->item_size);

    return 0;
}

void array_free(array *ar)
{
    for (int i = ar->length - 1; i >= 0; i--) {
        free(ar->items[i]);
    }
    free(ar->items);
    ar->items = NULL;
    free(ar);
}
