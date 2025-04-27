#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <sys/resource.h>
#include "hashmap.h"

#define SIZE 5000000
#define REPORT_FREQUENCY 1000000

struct rusage r;

char *keys;

static void benchmark_init(hashmap **map) {
    clock_t start, end;
    double cpu_time_used;

    start = clock();

    *map = hashmap_init();

    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;

    printf("Initialization benchmark completed. Time taken: %.5f seconds\n", cpu_time_used);
}

static void benchmark_insert(hashmap *map) {
    clock_t start, end;
    double cpu_time_used;

    start = clock();

    for (int i = 0; i < SIZE; i++) {
        hashmap_set(map, &keys[i * 20], i);
        if (i % REPORT_FREQUENCY == 0) {
            printf("Inserted %d items\n", i);
        }
    }

    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("Insertion benchmark completed. Time taken: %.5f seconds\n", cpu_time_used);
}

static void benchmark_search(hashmap *map) {
    clock_t start, end;
    double cpu_time_used;

    start = clock();
    for (int i = 0; i < SIZE; i++) {
        hashmap_item *item = hashmap_get(map, &keys[i * 20]);
        if (item->value != i) {
            printf("Error! %d != %d\n", item->value, i);
        }
        if (i % REPORT_FREQUENCY == 0) {
            printf("Searched %d items\n", i);
        }
    }

    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("Search benchmark completed. Time taken: %.5f seconds\n", cpu_time_used);
}

static void benchmark_delete(hashmap *map) {
    clock_t start, end;
    double cpu_time_used;

    start = clock();

    for (int i = 0; i < SIZE; i++) {;
        int res = hashmap_delete(map, &keys[20 * i]);
        if (res != 0) {
            printf("Error deleting %s\n", &keys[20 * i]);
        }
        if (i % REPORT_FREQUENCY == 0) {
            printf("Deleted %d items\n", i);
        }
    }

    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("Deletion benchmark completed. Time taken: %.5f seconds\n", cpu_time_used);
}

static void benchmark_free(hashmap *map) {
    clock_t start, end;
    double cpu_time_used;

    start = clock();

    hashmap_free(map);

    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;

    printf("Memory deallocation benchmark completed. Time taken: %.5f seconds\n", cpu_time_used);
}

static void benchmark_mixed_workload(hashmap *map) {
    clock_t start, end;
    double cpu_time_used;

    start = clock();

    for (int i = 0; i < SIZE; i++) {
        char *key = &keys[i * 20];
        // Insertion
        hashmap_set(map, key, i);

        // Update the value of a key
        if (i % 2 == 0) {
            hashmap_set(map, key, i + 1);
        }

        // Search
        hashmap_item *item = hashmap_get(map, key);
        if (item->value != (i % 2 == 0 ? i + 1 : i)) {
            printf("Error! %d != %d\n", item->value, i);
        }

        // Deletion
        if (i % 3 == 0) {
            hashmap_delete(map, key);
        }

        if (i % REPORT_FREQUENCY == 0) {
            printf("Processed %d items\n", i);
        }
    }

    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;

    printf("Mixed workload benchmark completed. Time taken: %.5f seconds\n", cpu_time_used);
}

static long get_peak_memory() {
    getrusage(RUSAGE_SELF, &r);
    return r.ru_maxrss;
}

int main() {
    long mem_before, mem_after;

    keys = malloc(20 * sizeof(char) * SIZE);
    for (int i = 0; i < SIZE; i++) {
        snprintf(&keys[i * 20], 20, "key%d", i);
    }

    mem_before = get_peak_memory();

    hashmap *map;
    benchmark_init(&map);
    benchmark_insert(map);
    benchmark_search(map);
    benchmark_delete(map);
    benchmark_mixed_workload(map);
    benchmark_free(map);
    mem_after = get_peak_memory();
    printf("Memory usage before: %ld, after: %ld, growth: %ld\n", mem_before, mem_after, mem_after - mem_before);
    free(keys);
    return 0;
}
