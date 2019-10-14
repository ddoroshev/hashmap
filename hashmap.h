#pragma once

#include "array.h"
#include "hashmap_item.h"

#define HASHMAP_BASE_SIZE 8

#define E_HASHMAP_KEY_NOT_FOUND 1
#define E_HASHMAP_FULL 2
#define E_HASHMAP_CANNOT_SET_VALUE 3

typedef struct {
    int length;
    array *values;  // array of hashmap_item
} hashmap;

hashmap *hashmap_init();
void hashmap_free(hashmap *hm);
void hashmap_dump(hashmap *hm);

int hashmap_set(hashmap*, char *key, int value);
hashmap_item *hashmap_get(hashmap *hm, char *key);
int hashmap_delete(hashmap *hm, char *key);

int _hashmap_find_index(hashmap *hm, char *key);
int _hashmap_find_empty_index(hashmap *hm, char *key);

unsigned long hash(char *s);
