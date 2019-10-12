#pragma once

#include "array.h"

#define HASHMAP_BASE_SIZE 8
#define HASHMAP_VALUE int*

#define E_HASHMAP_KEY_NOT_FOUND 1
#define E_HASHMAP_FULL 2
#define E_HASHMAP_CANNOT_SET_KEY 3
#define E_HASHMAP_CANNOT_SET_VALUE 4
#define E_HASHMAP_CANNOT_DELETE_KEY 5
#define E_HASHMAP_CANNOT_DELETE_VALUE 6

typedef struct {
    int length;
    array *keys;
    array *values;
} hashmap;

hashmap *hashmap_init();
void hashmap_free(hashmap *hm);
void hashmap_dump(hashmap *hm);

int hashmap_set(hashmap*, int, HASHMAP_VALUE);
int *hashmap_get(hashmap *hm, int key);
int hashmap_delete(hashmap *hm, int key);

int _hashmap_find_index(hashmap *hm, int key);
int _hashmap_find_empty_index(hashmap *hm, int key);
