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

int hashmap_set_value(hashmap*, int, HASHMAP_VALUE);
int *hashmap_get_value(hashmap *hm, int key);
