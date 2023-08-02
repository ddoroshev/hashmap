#include <stdio.h>
#include <time.h>
#include "hashmap/hashmap.h"

#define SIZE 5000000

void benchmark_insert(hashmap *map) {
    clock_t start, end;
    double cpu_time_used;

    start = clock();

    for (int i = 0; i < SIZE; i++) {
        char key[20];
        sprintf(key, "key%d", i);
        hashmap_set(map, key, i);
        if (i % 50000 == 0) {
            printf("%d\n", i);
        }
    }

    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;

    printf("Insertion benchmark completed. Time taken: %.5f seconds\n", cpu_time_used);
}

void benchmark_search(hashmap *map) {
    clock_t start, end;
    double cpu_time_used;

    start = clock();

    for (int i = 0; i < SIZE; i++) {
        char key[20];
        sprintf(key, "key%d", i);
        hashmap_item *item = hashmap_get(map, key);
        if (item->value != i) {
            printf("Error! %d != %d\n", item->value, i);
        }
        if (i % 50000 == 0) {
            printf("%d\n", i);
        }
    }

    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;

    printf("Search benchmark completed. Time taken: %.5f seconds\n", cpu_time_used);
}

void benchmark_delete(hashmap *map) {
    clock_t start, end;
    double cpu_time_used;

    start = clock();

    for (int i = 0; i < SIZE; i++) {
        char key[20];
        sprintf(key, "key%d", i);
        int res = hashmap_delete(map, key);
        if (res != 0) {
            printf("Error deleting %s\n", key);
        }
    }

    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;

    printf("Deletion benchmark completed. Time taken: %.5f seconds\n", cpu_time_used);
}

int main() {
    hashmap *map = hashmap_init();
    benchmark_insert(map);
    benchmark_insert(map);
    benchmark_search(map);
    benchmark_delete(map);

    hashmap_free(map);

    return 0;
}
