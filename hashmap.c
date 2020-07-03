#include <stdint.h>
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

    hm->count = 0;
    return hm;
}

void hashmap_free(hashmap *hm)
{
    array_free(hm->values);
    hm->values = NULL;
    hm->count = 0;
    free(hm);
}

uint32_t hashmap_len(hashmap *hm)
{
    return hm->count;
}

array *hashmap_values(hashmap *hm)
{
    return hm->values;
}

int hashmap_set(hashmap *hm, char *key, int value)
{
    int result;
    if ((result = hashmap_ensure_size(hm)) != 0) {
        return result;
    }

    int index = _hashmap_find_empty_index(hm, key);
    if (index == -E_HASHMAP_KEY_NOT_FOUND) {
        return -E_HASHMAP_FULL;
    }
    if (array_set(hm->values, index, &(hashmap_item){key, value}) != 0) {
        return -E_HASHMAP_CANNOT_SET_VALUE;
    }
    hm->count++;

    return 0;
}

int hashmap_ensure_size(hashmap *hm)
{
    int result;
    if (hm->count >= USABLE_FRACTION(array_len(hm->values))) {
        result = hashmap_resize(hm);
        if (result != 0) {
            return result;
        }
    }
    return 0;
}

int hashmap_resize(hashmap *hm)
{
    int result;
    hashmap_item *item;
    array *old_values = hm->values;
    int new_length = ESTIMATE_SIZE(array_len(hm->values));
    hm->count = 0;
    hm->values = array_init(new_length, sizeof(hashmap_item));
    if (hm->values == NULL) {
        return -E_ALLOC;
    }

    for (int i = 0; i < array_len(old_values); i++) {
        item = array_get(old_values, i);
        if (item == NULL) {
            continue;
        }
        result = hashmap_set(hm, item->key, item->value);
        if (result != 0) {
            return result;
        }
    }
    free(old_values);
    return 0;
}

int hashmap_delete(hashmap *hm, char *key)
{
    int index = _hashmap_find_index(hm, key);
    if (index < 0) {
        return index;
    }

    array_delete(hm->values, index);
    hm->count--;

    return 0;
}

hashmap_item *hashmap_get(hashmap *hm, char *key)
{
    int index = _hashmap_find_index(hm, key);
    if (index == -E_HASHMAP_KEY_NOT_FOUND) {
        return NULL;
    }
    return array_get(hm->values, index);
}

int _hashmap_find_index(hashmap *hm, char *key)
{
    unsigned long h = hash(key);
    int init_index = (int)(h & (array_len(hm->values) - 1));
    int index = init_index;

    for (;;) {
        hashmap_item *item = array_get(hm->values, index);
        if (item == NULL) {
            break;
        }
        if (strcmp(item->key, key) == 0) {
            return index;
        }
        index++;
        if (index == array_len(hm->values)) {
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
    int init_index = (int)(h & (array_len(hm->values) - 1));
    int index = init_index;

    for (;;) {
        hashmap_item *item = array_get(hm->values, index);
        if (item == NULL) {
            return index;
        }
        index++;
        if (index == array_len(hm->values)) {
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
