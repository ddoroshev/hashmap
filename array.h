#pragma once

int **init_array(int size);
void free_array(int **ar);
void dump_array(int **ar);

int set_value(int **ar, int index, int val);
int *get_value(int **ar, int index);
int delete_value(int **ar, int index);
