#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int length;
    int item_size;

    void **items;
} array;

typedef struct {
    char *key;
    int value;
} item;

array *array_init(int length, int item_size)
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

int main(void)
{
    int length = 10;
    array *ar = array_init(length, sizeof(item));
    printf("%d\n", ar->length);

    return 0;
}
