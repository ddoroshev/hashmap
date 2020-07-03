#pragma once

#define E_ARRAY_INDEX_OUT_OF_RANGE 1
#define E_ALLOC 2

typedef struct array array;

array *array_init(int length, int size);
int array_len(array *ar);
void array_free(array *ar);
void array_dump(array *ar);

int array_set(array *ar, int index, void *item);
void *array_get(array *ar, int index);
int array_delete(array *ar, int index);
