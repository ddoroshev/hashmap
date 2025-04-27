#include <stdio.h>
#include "hashmap.h"

void hashmap_dump(hashmap *hm)
{
    hashmap_item *item;
    for (uint32_t i = 0; i < hm->capacity; i++) {
        item = hm->items[i];
        if (item == NULL) {
            printf("NULL\n");
        } else {
            printf("%s => %d\n", item->key, item->value);
        }
    }
}
