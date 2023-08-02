#include <stdio.h>
#include <time.h>
#include "hashmap/hashmap.h"

#define SIZE 80000

void benchmark_insert(hashmap *map) {
    clock_t start, end;
    double cpu_time_used;

    start = clock();

    for (int i = 0; i < SIZE; i++) {
        char key[20];
        sprintf(key, "key%d", i);
        hashmap_set(map, key, i);
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
        hashmap_get(map, key);
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
        hashmap_delete(map, key);
    }

    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;

    printf("Deletion benchmark completed. Time taken: %.5f seconds\n", cpu_time_used);
}

int main() {
    hashmap *map = hashmap_init();
    benchmark_insert(map);
    benchmark_search(map);
    benchmark_delete(map);

    return 0;
}
