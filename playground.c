#include <stdio.h>
#include <stdlib.h>
#include "hashmap.h"

int main() {
    /* Basic initialization test */
    hashmap *hm = hashmap_init();
    if (hm == NULL) {
        printf("Failed to initialize hashmap\n");
        return 1;
    }
    printf("Hashmap initialized with capacity: %u\n", hashmap_get_capacity(hm));

    /* Basic set and get test */
    if (hashmap_set(hm, "key1", 100) != 0) {
        printf("Failed to set value\n");
        hashmap_free(hm);
        return 1;
    }
    printf("Set key1 = 100\n");

    hashmap_item *item = hashmap_get(hm, "key1");
    if (item == NULL) {
        printf("Failed to get value\n");
        hashmap_free(hm);
        return 1;
    }
    printf("Got value: %s => %d\n", item->key, item->value);

    /* Delete test */
    if (hashmap_delete(hm, "key1") != 0) {
        printf("Failed to delete key\n");
        hashmap_free(hm);
        return 1;
    }
    printf("Deleted key1\n");

    /* Verify deletion */
    item = hashmap_get(hm, "key1");
    if (item != NULL) {
        printf("Key was not properly deleted\n");
        hashmap_free(hm);
        return 1;
    }
    printf("Verified key1 is deleted\n");

    /* Free test */
    hashmap_free(hm);
    printf("Hashmap freed successfully\n");

    printf("All tests passed!\n");
    return 0;
}
