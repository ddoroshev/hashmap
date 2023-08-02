#include <stdio.h>
#include "hashmap/hashmap.h"
#include "hashmap/hashmap_item.h"

void hashmap_dump(hashmap *hm)
{
    hashmap_item *item;
    for (int i = 0; i < hashmap_len(hm); i++) {
        item = hashmap_values(hm)->items[i];
        if (item == NULL) {
            printf("NULL\n");
        } else {
            printf("%s => %d\n", item->key, item->value);
        }
    }
}
