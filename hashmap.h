#pragma once

#include <stdint.h>

#include "array.h"

#define HASHMAP_BASE_SIZE 8

// From CPython
#define USABLE_FRACTION(n) (((n) << 1)/3)
#define ESTIMATE_SIZE(hm) (hm->count*2 + (hm->values->length >> 1))

#define E_HASHMAP_KEY_NOT_FOUND 1
#define E_HASHMAP_FULL 2
#define E_HASHMAP_CANNOT_SET_VALUE 3
#define E_HASHMAP_CANNOT_RESIZE 4

typedef struct {
    char *key;
    int value;
    int is_deleted;
    uint32_t hash;
} hashmap_item;

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

int hashmap_set(hashmap*, char *key, int value);
hashmap_item *hashmap_get(hashmap *hm, char *key);
int hashmap_delete(hashmap *hm, char *key);

int32_t _hashmap_find_index(hashmap *hm, char *key, uint32_t hash);
uint32_t _hashmap_find_empty_index(hashmap *hm, uint32_t hash);

unsigned long hash(char *s);
