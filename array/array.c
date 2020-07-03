#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "alloc/alloc.h"
#include "array.h"

struct array {
    int length;
    int item_size;

    void **items;
};

void array_fill_null(array *ar);

array *array_init(int length, int item_size)
{
    array *ar = malloc(sizeof(array));
    if (ar == NULL) {
        return NULL;
    }
    ar->items = malloc(length * sizeof(void*));
    if (ar->items == NULL) {
        free(ar);
        return NULL;
    }
    ar->length = length;
    ar->item_size = item_size;
    array_fill_null(ar);
    return ar;
}

int array_len(array *ar)
{
    return ar->length;
}

void array_fill_null(array *ar)
{
    for (int i = 0; i < ar->length; i++) {
        ar->items[i] = NULL;
    }
}

int array_set(array *ar, int index, void *item)
{
    if (index < 0 || index >= ar->length) {
        return -E_ARRAY_INDEX_OUT_OF_RANGE;
    }
    ar->items[index] = malloc(sizeof(void*));
    if (ar->items[index] == NULL) {
        return -E_ALLOC;
    }
    memcpy(ar->items[index], item, ar->item_size);

    return 0;
}

void *array_get(array *ar, int index)
{
    if (index < 0 || index >= ar->length) {
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

int array_delete(array *ar, int index)
{
    if (index < 0 || index >= ar->length) {
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
