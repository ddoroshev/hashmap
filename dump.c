#include <stdio.h>
#include "hashmap.h"

void hashmap_dump(hashmap *hm)
{
    hashmap_item *item;
    for (int i = 0; i < hashmap_get_length(hm); i++) {
        item = hm->items[i];
        if (item == NULL) {
            printf("NULL\n");
        } else {
            printf("%s => %d\n", item->key, item->value);
        }
    }
}
