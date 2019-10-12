#include <stdio.h>
#include <stdlib.h>

#include "array.h"

const int SIZE = 10*1024*1024;

void run() {
    printf("Initing...\n");
    array *ar = init_array(SIZE);
    printf("Filling...\n");
    for (int i = 0; i < ar->length; i++) {
        set_value(ar, i, i);
    }
    printf("Freeing...\n");
    free_array(ar);
    printf("done\n");
}

int main(void)
{
    for (int i = 0; i < 10; i++) {
        run();
        printf("iter %d", i);
        getchar();
    }
}

array *init_array(int length)
{
    array *ar = malloc(sizeof(array));
    if (ar == NULL) {
        return NULL;
    }

    if ((ar->values = malloc(length * sizeof(int*))) == NULL) {
        free(ar);
        return NULL;
    }
    for (int i = 0; i < length; i++) {
        ar->values[i] = NULL;
    }

    ar->length = length;
    return ar;
}

void dump_array(array *ar)
{
    for (int i = 0; i < ar->length; i++) {
        int val = 0;
        if (ar->values[i] != NULL) {
            val = *ar->values[i];
        }
        printf("%d: %p -> %d\n", i, ar->values[i], val);
    }
}

void free_array(array *ar)
{
    for (int i = 0; i < ar->length; i++) {
        delete_value(ar, i);
    }
    free(ar->values);
    free(ar);
}

int set_value(array *ar, int index, int val)
{
    if (index >= ar->length) {
        return -1;
    }
    int *v = malloc(sizeof(int));
    *v = val;
    ar->values[index] = v;

    return 0;
}

int *get_value(array *ar, int index)
{
    if (index >= ar->length) {
        return NULL;
    }
    return ar->values[index];
}

int delete_value(array *ar, int index)
{
    if (index >= ar->length) {
        return -1;
    }
    free(ar->values[index]);
    ar->values[index] = NULL;

    return 0;
}
