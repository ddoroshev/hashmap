#pragma once

#define E_ARRAY_INDEX_OUT_OF_RANGE 1
#define E_ALLOC 2

typedef struct {
    int length;
    int item_size;

    void **items;
} array;

array *array_init(int length, int size);
void array_fill_null(array *ar);
void array_free(array *ar);
void array_dump(array *ar);

int array_set_item(array *ar, int index, void *item);
void *array_get_item(array *ar, int index);
int array_delete_item(array *ar, int index);
