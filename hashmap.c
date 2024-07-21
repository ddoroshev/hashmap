#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "alloc.h"
#include "hashmap.h"

hashmap *hashmap_init()
{
    int length = HASHMAP_BASE_SIZE;
    hashmap *hm = calloc(1, sizeof(hashmap));
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
    hashmap_item *item;
    for (int i = 0; i < hm->values->length; i++) {
        item = hm->values->items[i];
        if (item != NULL) {
            free(item->key);
        }
    }
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
    int is_new_key = 0;

    if ((result = hashmap_ensure_size(hm)) != 0) {
        return result;
    }

    unsigned long h = hash(key);
    int index = _hashmap_find_index(hm, key, h);
    if (index == -E_HASHMAP_KEY_NOT_FOUND) {
        is_new_key = 1;
        index = _hashmap_find_empty_index(hm, h);
    }
    if (index == -E_HASHMAP_KEY_NOT_FOUND) {
        return -E_HASHMAP_FULL;
    }

    if (is_new_key) {
        char *key_copy = strdup(key);
        if (key_copy == NULL) {
            return -E_HASHMAP_CANNOT_SET_VALUE;
        }
        if (array_set(hm->values, index, &(hashmap_item){key_copy, value, 0, h}) != 0) {
            free(key_copy);
            return -E_HASHMAP_CANNOT_SET_VALUE;
        }
    } else {
        hashmap_item *item = hm->values->items[index];
        item->value = value;
    }

    if (is_new_key) {
        hm->count++;
    }

    return 0;
}


int hashmap_ensure_size(hashmap *hm)
{
    int result;
    if (hm->count >= USABLE_FRACTION(hm->values->length)) {
        result = hashmap_resize(hm);
        if (result != 0) {
            return result;
        }
    }
    return 0;
}

int hashmap_resize(hashmap *hm)
{
    hashmap_item *item;
    array *old_values = hm->values;
    uint32_t old_len = hm->values->length;
    int est_size = ESTIMATE_SIZE(old_len);
    int new_length = HASHMAP_BASE_SIZE;
    while (new_length <= est_size && new_length > 0) {
        new_length <<= 1;
    }
    hm->count = 0;
    hm->values = array_init(new_length, sizeof(hashmap_item));
    if (hm->values == NULL) {
        return -E_ALLOC;
    }

    for (int i = 0; i < old_len; i++) {
        item = old_values->items[i];
        if (item == NULL || item->is_deleted == 1) {
            continue;
        }

        int index = _hashmap_find_empty_index(hm, item->hash);
        if (index == -E_HASHMAP_KEY_NOT_FOUND) {
            array_free(hm->values);
            hm->values = old_values;
            return index;
        }
        hm->values->items[index] = item;
        hm->count++;
    }
    free(old_values->items);
    free(old_values);
    return 0;
}

int hashmap_delete(hashmap *hm, char *key)
{
    int index = _hashmap_find_index(hm, key, hash(key));
    if (index < 0) {
        return index;
    }

    hashmap_item *item = hm->values->items[index];
    item->is_deleted = 1;
    hm->count--;

    return 0;
}

hashmap_item *hashmap_get(hashmap *hm, char *key)
{
    int index = _hashmap_find_index(hm, key, hash(key));
    if (index == -E_HASHMAP_KEY_NOT_FOUND) {
        return NULL;
    }
    return hm->values->items[index];
}

int _hashmap_find_index(hashmap *hm, char *key, unsigned long hash)
{
    uint32_t len = hm->values->length;
    int index = (int)(hash & (len - 1));
    unsigned long perturb = hash;
    for (;; perturb >>= 5) {
        hashmap_item *item = hm->values->items[index];
        if (item == NULL) {
            break;
        }
        if (item->is_deleted == 0 && item->hash == hash && strcmp(item->key, key) == 0) {
            return index;
        }
        index = ((index << 2) + index + perturb + 1) % len;
        if (index == len) {
            index = 0;
        }
    }

    return -E_HASHMAP_KEY_NOT_FOUND;
}

int _hashmap_find_empty_index(hashmap *hm, unsigned long hash)
{
    uint32_t len = hm->values->length;
    int index = (int)(hash & (len - 1));
    hashmap_item *item;
    unsigned long perturb = hash;

    for (;; perturb >>= 5) {
        item = hm->values->items[index];
        if (item == NULL || item->is_deleted == 1) {
            return index;
        }
        index = ((index << 2) + index + perturb + 1) % len;
        if (index == len) {
            index = 0;
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
