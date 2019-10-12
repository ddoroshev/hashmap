#include <stdio.h>

#include "hashmap.h"

int main(void)
{
    int i;
    hashmap *hm = hashmap_init(HASHMAP_BASE_SIZE);
    int keys[HASHMAP_BASE_SIZE] = {
        156, 167, 234, 343, 75, 884, 500, 590,
        /* 0   1    2    3   4    5    6    7 */
    };

    for (i = 0; i < HASHMAP_BASE_SIZE; i++) {
        hashmap_set(hm, keys[i], &i);
    }

    for (i = 0; i < HASHMAP_BASE_SIZE; i++) {
        int *pval = hashmap_get(hm, keys[i]);
        if (pval == NULL) {
            printf("[%d]: NOT EXIST\n", keys[i]);
        } else {
            printf("[%d]: %d\n", keys[i], *pval);
        }
    }
    printf("\n");
    hashmap_dump(hm);
    printf("\n");

    hashmap_delete(hm, 234);
    hashmap_dump(hm);

    hashmap_free(hm);
    return 0;
}
