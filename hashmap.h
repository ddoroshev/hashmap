#include "array.h"

#define HASHMAP_BASE_SIZE 8
#define HASHMAP_VALUE int*

typedef struct {
    int length;
    array *keys;
    array *values;
} hashmap;

hashmap *hashmap_init(int length);
void hashmap_free(hashmap *hm);
void hashmap_dump(hashmap *hm);

int hashmap_set(hashmap*, int, HASHMAP_VALUE);
int *hashmap_get(hashmap *hm, int key);
int hashmap_delete(hashmap *hm, int key);

int _hashmap_find_index(hashmap *hm, int key);
int _hashmap_find_empty_index(hashmap *hm, int key);
