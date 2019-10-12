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

void hashmap_free(hashmap *hm)
{
    array_free(hm->keys);
    array_free(hm->values);
    free(hm);
}

int hashmap_set_value(hashmap *hm, int key, HASHMAP_VALUE value)
{
    int init_index = key & (HASHMAP_BASE_SIZE - 1);
    int index = init_index;

    for (;;) {
        int *pkey = array_get_value(hm->keys, index);
        if (pkey == NULL) {
            break;
        }
        index++;
        if (index >= hm->length) {
            index = 0;
        }
        if (index == init_index) {
            return -1;
        }
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
    int init_index = key & (HASHMAP_BASE_SIZE - 1);
    int index = init_index;

    for (;;) {
        int *pkey = array_get_value(hm->keys, index);
        if (pkey == NULL) {
            return NULL;
        }
        if (*pkey == key) {
            break;
        }
        index++;
        if (index >= hm->length) {
            index = 0;
        }
        if (index == init_index) {
            return NULL;
        }
    }

    return array_get_value(hm->values, index);
}
