#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <sys/resource.h>
#include "hashmap.h"

#define SIZE 5000000
#define REPORT_FREQUENCY 10

struct rusage r;

void benchmark_init(hashmap **map) {
    clock_t start, end;
    double cpu_time_used;

    start = clock();

    *map = hashmap_init();

    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;

    printf("Initialization benchmark completed. Time taken: %.5f seconds\n", cpu_time_used);
}

void benchmark_insert(hashmap *map) {
    clock_t start, end;
    double cpu_time_used;

    start = clock();

    for (int i = 0; i < SIZE; i++) {
        char *key = malloc(20 * sizeof(char));
        sprintf(key, "key%d", i);
        hashmap_set(map, key, i);
        if (i % REPORT_FREQUENCY == 0) {
            printf("Inserted %d items\n", i);
        }
        free(key);
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
        if (i % REPORT_FREQUENCY == 0) {
            printf("Searched %d items\n", i);
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
        if (i % REPORT_FREQUENCY == 0) {
            printf("Deleted %d items\n", i);
        }
    }

    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;

    printf("Deletion benchmark completed. Time taken: %.5f seconds\n", cpu_time_used);
}

void benchmark_free(hashmap *map) {
    clock_t start, end;
    double cpu_time_used;

    start = clock();

    hashmap_free(map);

    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;

    printf("Memory deallocation benchmark completed. Time taken: %.5f seconds\n", cpu_time_used);
}

void benchmark_mixed_workload(hashmap *map) {
    clock_t start, end;
    double cpu_time_used;

    start = clock();

    for (int i = 0; i < SIZE; i++) {
        char *key = malloc(20 * sizeof(char));
        sprintf(key, "key%d", i);

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
        free(key);
    }

    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;

    printf("Mixed workload benchmark completed. Time taken: %.5f seconds\n", cpu_time_used);
}

long get_peak_memory() {
    getrusage(RUSAGE_SELF, &r);
    return r.ru_maxrss;
}

void memory_report(long mem_before, long mem_after) {
    double mem_before_kb = mem_before / 1024.0;
    double mem_after_kb = mem_after / 1024.0;
    printf("Memory usage before: %.2f KB, after: %.2f KB, growth: %.2f KB\n", mem_before_kb, mem_after_kb, mem_after_kb - mem_before_kb);
}

int main() {
    long mem_before, mem_after;
    hashmap *map;

    mem_before = get_peak_memory();
    benchmark_init(&map);
    mem_after = get_peak_memory();
    memory_report(mem_before, mem_after);

    // mem_before = get_peak_memory();
    // benchmark_insert(map);
    // mem_after = get_peak_memory();
    // memory_report(mem_before, mem_after);

    // mem_before = get_peak_memory();
    // benchmark_search(map);
    // mem_after = get_peak_memory();
    // memory_report(mem_before, mem_after);

    // mem_before = get_peak_memory();
    // benchmark_delete(map);
    // mem_after = get_peak_memory();
    // memory_report(mem_before, mem_after);

    // mem_before = get_peak_memory();
    // benchmark_free(map);
    // mem_after = get_peak_memory();

    // map = hashmap_init();
    mem_before = get_peak_memory();
    benchmark_mixed_workload(map);
    mem_after = get_peak_memory();
    memory_report(mem_before, mem_after);

    hashmap_free(map);
    return 0;
}