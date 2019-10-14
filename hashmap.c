#include <stdlib.h>
#include <string.h>

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

    hm->values = array_init(length, sizeof(hashmap_item));
    if (hm->values == NULL) {
        free(hm);
        return NULL;
    }

    hm->length = length;
    return hm;
}

void hashmap_free(hashmap *hm)
{
    array_free(hm->values);
    hm->values = NULL;
    free(hm);
}

int hashmap_set(hashmap *hm, char *key, int value)
{
    int index = _hashmap_find_empty_index(hm, key);
    if (index == -E_HASHMAP_KEY_NOT_FOUND) {
        return -E_HASHMAP_FULL;
    }
    if (array_set_item(hm->values, index, &(hashmap_item){key, value}) != 0) {
        return -E_HASHMAP_CANNOT_SET_VALUE;
    }

    return 0;
}

int hashmap_delete(hashmap *hm, char *key)
{
    int index = _hashmap_find_index(hm, key);
    if (index < 0) {
        return index;
    }

    array_delete_item(hm->values, index);

    return 0;
}

hashmap_item *hashmap_get(hashmap *hm, char *key)
{
    int index = _hashmap_find_index(hm, key);
    if (index == -E_HASHMAP_KEY_NOT_FOUND) {
        return NULL;
    }
    return array_get_item(hm->values, index);
}

int _hashmap_find_index(hashmap *hm, char *key)
{
    unsigned long h = hash(key);
    int init_index = (int)(h & (HASHMAP_BASE_SIZE - 1));
    int index = init_index;

    for (;;) {
        hashmap_item *item = array_get_item(hm->values, index);
        if (item == NULL) {
            break;
        }
        if (strcmp(item->key, key) == 0) {
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

int _hashmap_find_empty_index(hashmap *hm, char *key)
{
    unsigned long h = hash(key);
    int init_index = (int)(h & (HASHMAP_BASE_SIZE - 1));
    int index = init_index;

    for (;;) {
        hashmap_item *item = array_get_item(hm->values, index);
        if (item == NULL) {
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

unsigned long hash(char *s)
{
    unsigned long hash = 5381;
    int c;

    while ((c = *s++)) {
        hash = ((hash << 5) + hash) + c;
    }

    return hash;
}
