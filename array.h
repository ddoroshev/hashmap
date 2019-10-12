#pragma once

typedef struct {
    int length;
    int **values;
} array;

array *init_array(int length);
void free_array(array *ar);
void dump_array(array *ar);

int set_value(array *ar, int index, int val);
int *get_value(array *ar, int index);
int delete_value(array *ar, int index);
