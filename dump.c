#include <stdio.h>
#include "array.h"
#include "hashmap.h"
#include "hashmap_item.h"

void array_dump(array *ar)
{
    for (int i = 0; i < array_len(ar); i++) {
        hashmap_item item = {"", 0};
        if (array_get_item(ar, i) != NULL) {
            item = *(hashmap_item*)array_get_item(ar, i);
        }
        printf("%d: %p -> %s %d\n", i, array_get_item(ar, i), item.key, item.value);
    }
}

void hashmap_dump(hashmap *hm)
{
    hashmap_item *item;
    for (int i = 0; i < array_len(hm->values); i++) {
        item = array_get_item(hm->values, i);
        if (item == NULL) {
            printf("NULL\n");
        } else {
            printf("%s => %d\n", item->key, item->value);
        }
    }
}
