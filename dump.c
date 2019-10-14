#include <stdio.h>
#include "array.h"
#include "hashmap.h"
#include "hashmap_item.h"

void array_dump(array *ar)
{
    for (int i = 0; i < ar->length; i++) {
        hashmap_item item = {"", 0};
        if (ar->items[i] != NULL) {
            item = *(hashmap_item*)ar->items[i];
        }
        printf("%d: %p -> %s %d\n", i, ar->items[i], item.key, item.value);
    }
}

void hashmap_dump(hashmap *hm)
{
    hashmap_item *item;
    for (int i = 0; i < hm->values->length; i++) {
        item = array_get_item(hm->values, i);
        if (item == NULL) {
            printf("NULL\n");
        } else {
            printf("%s => %d\n", item->key, item->value);
        }
    }
}
