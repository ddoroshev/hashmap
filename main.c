#include <stdio.h>

#include "hashmap.h"

int main(void)
{
    int i;
    hashmap *hm = hashmap_init();
    char *keys[HASHMAP_BASE_SIZE] = {
        "ab", "bc", "cd", "de",
        "ef", "fg", "gh", "hi"
    };

    printf("Hashes:\n");
    for (i = 0; i < HASHMAP_BASE_SIZE; i++) {
        unsigned long h = hash(keys[i]);
        printf("%s -> %lu -> %i\n", keys[i], h, (int)(h & (HASHMAP_BASE_SIZE - 1)));
        hashmap_set(hm, keys[i], i);
    }

    for (i = 0; i < HASHMAP_BASE_SIZE; i++) {
        hashmap_item *pval = hashmap_get(hm, keys[i]);
        if (pval == NULL) {
            printf("[%s]: NOT EXIST\n", keys[i]);
        } else {
            printf("[%s]: %d\n", keys[i], pval->value);
        }
    }
    printf("\n");
    hashmap_dump(hm);
    printf("\n");

    hashmap_delete(hm, "cd");
    hashmap_dump(hm);

    hashmap_free(hm);
    return 0;
}
