#pragma once

#include <stdint.h>

#include "array/array.h"
#include "hashmap_item.h"

#define HASHMAP_BASE_SIZE 8

// From CPython
#define USABLE_FRACTION(n) (((n) << 1)/3)
#define ESTIMATE_SIZE(n) (((n)*3+1) >> 1)

#define E_HASHMAP_KEY_NOT_FOUND 1
#define E_HASHMAP_FULL 2
#define E_HASHMAP_CANNOT_SET_VALUE 3
#define E_HASHMAP_CANNOT_RESIZE 4

typedef struct {
    uint32_t count;
    array *values;  // array of hashmap_item
} hashmap;

hashmap *hashmap_init();
void hashmap_free(hashmap *hm);
uint32_t hashmap_len(hashmap *hm);
int hashmap_ensure_size(hashmap *hm);
int hashmap_resize(hashmap *hm);

array *hashmap_values(hashmap *hm);

int hashmap_set(hashmap*, const char *key, int value);
hashmap_item *hashmap_get(hashmap *hm, const char *key);
int hashmap_delete(hashmap *hm, const char *key);

int _hashmap_find_index(hashmap *hm, const char *key);
int _hashmap_find_empty_index(hashmap *hm, const char *key);

unsigned long hash(const char *s);
