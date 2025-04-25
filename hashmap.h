#pragma once

#include <stdint.h>

/* Initial size of hashmap array. Must be a power of 2. */
#define HASHMAP_BASE_SIZE 8

/* From CPython: calculate usable fraction of hash table size (2/3) */
#define USABLE_FRACTION(n) (((n) << 1)/3)
/* Estimate size for resize based on current count and table length */
#define ESTIMATE_SIZE(hm) (hm->count*2 + (hm->length >> 1))

/* Error codes for hashmap operations */
#define E_HASHMAP_KEY_NOT_FOUND 1
#define E_HASHMAP_FULL 2
#define E_HASHMAP_CANNOT_SET_VALUE 3
#define E_HASHMAP_CANNOT_RESIZE 4
#define E_ALLOC 5

typedef struct {
    char *key;      /* dynamically allocated string, owned by hashmap */
    int value;      /* integer associated with the key                */
    int is_deleted; /* deletion flag                                  */
    uint32_t hash;  /* hash value of the key                          */
} hashmap_item;

typedef struct {
    uint32_t count;       /* number of active (non-deleted) items */
    uint32_t length;      /* number of slots in the items array   */
    hashmap_item **items; /* array of hashmap_item pointers       */
} hashmap;

hashmap *hashmap_init();
void hashmap_free(hashmap *hm);
uint32_t hashmap_len(hashmap *hm);
int hashmap_ensure_size(hashmap *hm);
int hashmap_resize(hashmap *hm);
uint32_t hashmap_get_length(hashmap *hm);
int hashmap_set(hashmap*, char *key, int value);
hashmap_item *hashmap_get(hashmap *hm, char *key);
int hashmap_delete(hashmap *hm, char *key);
int32_t _hashmap_find_index(hashmap *hm, char *key, uint32_t hash);
uint32_t _hashmap_find_empty_index(hashmap *hm, uint32_t hash);
unsigned long hash(char *s);
