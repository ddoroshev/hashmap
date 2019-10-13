#include <stdio.h>
#include <stdlib.h>

#include "alloc.h"
#include "array.h"
#include "hashmap.h"

hashmap *hashmap_init()
{
    int length = HASHMAP_BASE_SIZE;
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

void hashmap_free(hashmap *hm)
{
    array_free(hm->keys);
    hm->keys = NULL;
    array_free(hm->values);
    hm->values = NULL;
    free(hm);
}

int hashmap_set(hashmap *hm, int key, HASHMAP_VALUE value)
{
    int index = _hashmap_find_empty_index(hm, key);
    if (index == -1) {
        return -E_HASHMAP_FULL;
    }
    if (array_set_value(hm->keys, index, key) != 0) {
        return -E_HASHMAP_CANNOT_SET_KEY;
    }
    if (array_set_value(hm->values, index, *value) != 0) {
        array_delete_value(hm->keys, index);
        return -E_HASHMAP_CANNOT_SET_VALUE;
    }

    return 0;
}

int hashmap_delete(hashmap *hm, int key)
{
    int index = _hashmap_find_index(hm, key);
    if (index < 0) {
        return index;
    }

    array_delete_value(hm->keys, index);
    array_delete_value(hm->values, index);

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

int *hashmap_get(hashmap *hm, int key)
{
    int index = _hashmap_find_index(hm, key);
    if (index == -E_HASHMAP_KEY_NOT_FOUND) {
        return NULL;
    }
    return array_get_value(hm->values, index);
}

int _hashmap_find_index(hashmap *hm, int key)
{
    int init_index = key & (HASHMAP_BASE_SIZE - 1);
    int index = init_index;

    for (;;) {
        int *pkey = array_get_value(hm->keys, index);
        if (pkey == NULL) {
            break;
        }
        if (*pkey == key) {
            return index;
        }
        index++;
        if (index == hm->length) {
            index = 0;
        }
        if (index == init_index) {
            break;
        }
    }

    return -E_HASHMAP_KEY_NOT_FOUND;
}

int _hashmap_find_empty_index(hashmap *hm, int key)
{
    int init_index = key & (HASHMAP_BASE_SIZE - 1);
    int index = init_index;

    for (;;) {
        int *pkey = array_get_value(hm->keys, index);
        if (pkey == NULL) {
            return index;
        }
        index++;
        if (index == hm->length) {
            index = 0;
        }
        if (index == init_index) {
            break;
        }
    }

    return -E_HASHMAP_KEY_NOT_FOUND;
}
