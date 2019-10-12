#include <stdio.h>
#include <stdlib.h>

#include "array.h"
#include "hashmap.h"

hashmap *hashmap_init(int length)
{
    hashmap *hm = malloc(sizeof(hashmap));
    if (hm == NULL) {
        return NULL;
    }

    hm->keys = array_init(length);
    if (hm->keys == NULL) {
        free(hm);
        return NULL;
    }
    hm->values = array_init(length);
    if (hm->values == NULL) {
        free(hm->keys);
        free(hm);
        return NULL;
    }

    hm->length = length;
    return hm;
}

int hashmap_set_value(hashmap *hm, int key, HASH_VALUE value)
{
    int index = key & HASH_MASK;
    int *pkey = array_get_value(hm->keys, index);
    if (pkey != NULL) {
        index = 0;
    }
    while (pkey != NULL) {
        index++;
        pkey = array_get_value(hm->keys, index);
    }

    if (array_set_value(hm->keys, index, key) == -1) {
        return -1;
    }
    if (array_set_value(hm->values, index, *value) == -1) {
        return -1;
    }

    return 0;
}

void hashmap_dump(hashmap *hm)
{
    int *pkey, *pvalue;
    for (int i = 0; i < hm->length; i++) {
        pkey = array_get_value(hm->keys, i);
        if (pkey == NULL) {
            printf("NULL => ");
        } else {
            printf("%d => ", *pkey);
        }
        pvalue = array_get_value(hm->values, i);
        if (pvalue == NULL) {
            printf("NULL\n");
        } else {
            printf("%d\n", *pvalue);
        }
    }
}

int *hashmap_get_value(hashmap *hm, int key)
{
    int index = key & HASH_MASK;
    int *probe_key = array_get_value(hm->keys, index);
    if (probe_key == NULL) {
        return NULL;
    }
    if (*probe_key != key) {
        index = 0;
    }
    while (probe_key != NULL && *probe_key != key) {
        index++;
        probe_key = array_get_value(hm->keys, index);
    }
    return array_get_value(hm->values, index);
}
