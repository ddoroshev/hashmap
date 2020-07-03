#include <stdio.h>
#include "array/array.h"
#include "hashmap/hashmap.h"
#include "hashmap/hashmap_item.h"

void array_dump(array *ar)
{
    for (int i = 0; i < array_len(ar); i++) {
        hashmap_item item = {"", 0};
        if (array_get(ar, i) != NULL) {
            item = *(hashmap_item*)array_get(ar, i);
        }
        printf("%d: %p -> %s %d\n", i, array_get(ar, i), item.key, item.value);
    }
}

void hashmap_dump(hashmap *hm)
{
    hashmap_item *item;
    for (int i = 0; i < hashmap_len(hm); i++) {
        item = array_get(hashmap_values(hm), i);
        if (item == NULL) {
            printf("NULL\n");
        } else {
            printf("%s => %d\n", item->key, item->value);
        }
    }
}
