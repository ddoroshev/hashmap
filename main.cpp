#include <stdio.h>

#include "dump.hpp"
#include "hashmap/hashmap.hpp"

int main(void)
{
    int i;
    auto hm = new Hashmap<int, int>();
    int keys[] = {
        12,  // -> 4
        23,  // -> 7
        34,  // -> 2
        45,  // -> 5
        56,  // -> 0
        67,  // -> 3
        78,  // -> 6
        89   // -> 1
    };

    printf("Hashes:\n");
    for (i = 0; i < HASHMAP_BASE_SIZE; i++) {
        unsigned long h = hash_key(keys[i]);
        printf("%i -> %lu -> %i\n", keys[i], h, (int)(h & (HASHMAP_BASE_SIZE - 1)));
        hm->Set(keys[i], i);
    }

    for (i = 0; i < HASHMAP_BASE_SIZE; i++) {
        HashmapItem<int, int> tmp = hm->Get(keys[i]);
        if (!tmp.active) {
            printf("[%i]: NOT EXIST\n", keys[i]);
            continue;
        }
        printf("[%i]: %d\n", keys[i], tmp.v);
    }
    printf("\n");
    hashmap_dump(hm);
    printf("\n");

    hm->Del(23);
    hashmap_dump(hm);

    delete hm;
    return 0;
}
