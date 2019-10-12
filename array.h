#pragma once

#define E_ARRAY_INDEX_OUT_OF_RANGE 1

typedef struct {
    int length;
    int **values;
} array;

array *array_init(int length);
void array_free(array *ar);
void array_dump(array *ar);

int array_set_value(array *ar, int index, int val);
int *array_get_value(array *ar, int index);
int array_delete_value(array *ar, int index);
