#include <stdio.h>
#include <stdlib.h>

#include "array.h"

const int SIZE = 1;

int main(void)
{
    int **ar = init_array(SIZE);
    set_value(ar, 12, 111);
    int *val = get_value(ar, 12);
    printf("%d\n", *val);
    // printf("Fill...");
    // for (int i = 0; i < SIZE; i++) {
    //     set_value(ar, i, i);
    // }
    // printf("%ld\n", sizeof(ar));
    // printf("done\n");
    // dump_array(ar);
    // int *val = get_value(ar, 2);
    // printf("%d\n", *val);
    // free_array(ar);
    // printf("done\n");
}

int **init_array(int size)
{
    int **ar;
    if ((ar = malloc(size * sizeof(int*))) == NULL) {
        return NULL;
    }

    for (int i = 0; i < size; i++) {
        ar[i] = NULL;
    }
    return ar;
}

void dump_array(int **ar)
{
    for (int i = 0; i < SIZE; i++) {
        int val = 0;
        if (ar[i] != NULL) {
            val = *ar[i];
        }
        printf("%d: %p -> %d\n", i, ar[i], val);
    }
}

void free_array(int **ar)
{
    for (int i = 0; i < SIZE; i++) {
        delete_value(ar, i);
    }
    free(ar);
}

int set_value(int **ar, int index, int val)
{
    int *v = malloc(sizeof(int));
    *v = val;
    ar[index] = v;

    return 0;
}

int *get_value(int **ar, int index)
{
    return ar[index];
}

int delete_value(int **ar, int index)
{
    free(ar[index]);
    ar[index] = NULL;

    return 0;
}
