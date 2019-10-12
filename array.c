#include <stdio.h>
#include <stdlib.h>

#include "array.h"

array *array_init(int length)
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

void array_dump(array *ar)
{
    for (int i = 0; i < ar->length; i++) {
        int val = 0;
        if (ar->values[i] != NULL) {
            val = *ar->values[i];
        }
        printf("%d: %p -> %d\n", i, ar->values[i], val);
    }
}

void array_free(array *ar)
{
    for (int i = 0; i < ar->length; i++) {
        array_delete_value(ar, i);
    }
    free(ar->values);
    free(ar);
}

int array_set_value(array *ar, int index, int val)
{
    if (index >= ar->length) {
        return -1;
    }
    int *v = malloc(sizeof(int));
    *v = val;
    ar->values[index] = v;

    return 0;
}

int *array_get_value(array *ar, int index)
{
    if (index >= ar->length) {
        return NULL;
    }
    return ar->values[index];
}

int array_delete_value(array *ar, int index)
{
    if (index >= ar->length) {
        return -1;
    }
    free(ar->values[index]);
    ar->values[index] = NULL;

    return 0;
}
