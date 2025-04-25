#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "alloc.h"
#include "hashmap.h"

/**
 * Allocate and initialize a new hashmap with default capacity
 *
 * Return: Pointer to new hashmap, or NULL if allocation fails
 */
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

/**
 * Free all memory used by a hashmap
 * @hm: Hashmap to free
 */
void hashmap_free(hashmap *hm)
{
    if (hm == NULL) {
        return;
    }
    
    hashmap_item *item;
    for (int i = 0; i < hm->values->length; i++) {
        item = hm->values->items[i];
        if (item != NULL && item->key != NULL) {
            free(item->key);
        }
    }
    array_free(hm->values);
    hm->values = NULL;
    hm->count = 0;
    free(hm);
}

/**
 * Get number of active items in hashmap.
 * @hm: Hashmap to count
 *
 * Return: Number of active (non-deleted) key-value pairs
 */
uint32_t hashmap_len(hashmap *hm)
{
    return hm->count;
}

/**
 * Get the underlying array of values.
 * @hm: Hashmap to access
 *
 * Return: Pointer to the array of hashmap items
 */
array *hashmap_values(hashmap *hm)
{
    return hm->values;
}

/**
 * Set a key-value pair in the hashmap
 * @hm: Hashmap to update
 * @key: String key to set
 * @value: Integer value to store
 *
 * If the key already exists, its value is updated.
 * If the key doesn't exist, a new entry is created.
 * The key string is copied, so the original can be freely modified
 * or deallocated after this call.
 *
 * Return: 0 on success, negative error code on failure
 */
int hashmap_set(hashmap *hm, char *key, int value)
{
    int result;
    int is_new_key = 0;
    
    if (hm == NULL || key == NULL) {
        return -E_HASHMAP_CANNOT_SET_VALUE;
    }

    /* Ensure we have enough space */
    if ((result = hashmap_ensure_size(hm)) != 0) {
        return result;
    }

    /* Find the key or an empty slot */
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
        /* For new keys, allocate memory for the key copy */
        char *key_copy = strdup(key);
        if (key_copy == NULL) {
            return -E_HASHMAP_CANNOT_SET_VALUE;
        }
        if (array_set(hm->values, index, &(hashmap_item){key_copy, value, 0, h}) != 0) {
            free(key_copy);
            return -E_HASHMAP_CANNOT_SET_VALUE;
        }
    } else {
        /* For existing keys, just update the value */
        hashmap_item *item = hm->values->items[index];
        item->value = value;
    }

    if (is_new_key) {
        hm->count++;
    }

    return 0;
}

/**
 * Resize hashmap if it's too full, i.e. the load factor is above
 * the threshold.
 * @hm: Hashmap to check
 *
 * Return: 0 on success, error code on failure
 */
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

/**
 * Resize hashmap to a larger capacity.
 * @hm: Hashmap to resize
 *
 * Creates a new, larger array and rehashes all existing items
 * into it. Deleted items are purged during the resize.
 *
 * Return: 0 on success, error code on failure
 */
int hashmap_resize(hashmap *hm)
{
    hashmap_item *item;
    array *old_values = hm->values;
    uint32_t old_len = hm->values->length;
    uint32_t est_size = ESTIMATE_SIZE(hm);
    uint32_t new_length = HASHMAP_BASE_SIZE;
    
    /* Find the next power of 2 that will fit our data */
    for (new_length = HASHMAP_BASE_SIZE;
         new_length <= est_size && new_length > 0;
         new_length <<= 1);

    /* Reset count and allocate new array */
    hm->count = 0;
    hm->values = array_init(new_length, sizeof(hashmap_item));
    if (hm->values == NULL) {
        return -E_ALLOC;
    }

    /* Move all non-deleted items to the new array */
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
    
    /* Free the deleted items and old array */
    for (int i = 0; i < old_values->length; i++) {
        item = old_values->items[i];
        if (item != NULL && item->is_deleted == 1) {
            free(item);
        }
    }
    free(old_values->items);
    free(old_values);
    return 0;
}

/**
 * Delete a key-value pair from the hashmap
 * @hm: Hashmap to modify
 * @key: Key to delete
 *
 * Mark the item as deleted and free associated key memory.
 * The slot remains occupied but marked as deleted until a resize.
 *
 * Return: 0 on success, error code on failure
 */
int hashmap_delete(hashmap *hm, char *key)
{
    if (hm == NULL || key == NULL) {
        return -E_HASHMAP_KEY_NOT_FOUND;
    }
    
    int index = _hashmap_find_index(hm, key, hash(key));
    if (index < 0) {
        return index;
    }

    hashmap_item *item = hm->values->items[index];
    free(item->key);
    item->key = NULL;
    item->is_deleted = 1;
    hm->count--;

    return 0;
}

/**
 * Get a value by key from the hashmap
 * @hm: Hashmap to search
 * @key: Key to look up
 *
 * Return: Pointer to hashmap_item if found, NULL if not found
 */
hashmap_item *hashmap_get(hashmap *hm, char *key)
{
    if (hm == NULL || key == NULL) {
        return NULL;
    }
    
    int index = _hashmap_find_index(hm, key, hash(key));
    if (index == -E_HASHMAP_KEY_NOT_FOUND) {
        return NULL;
    }
    return hm->values->items[index];
}

/**
 * Find the array index of a key.
 * @hm: Hashmap to search
 * @key: Key to find
 * @hash: Pre-computed hash of the key
 *
 * Uses quadratic probing to handle collisions.
 * The probing sequence is derived from the CPython dict implementation.
 *
 * Return: Non-negative index if found, -E_HASHMAP_KEY_NOT_FOUND if not found
 */
int32_t _hashmap_find_index(hashmap *hm, char *key, uint32_t hash)
{
    uint32_t mask = hm->values->length - 1;
    uint32_t i = hash & mask;
    uint32_t perturb;
    hashmap_item *item;
    
    /* Maximum number of probes - prevent potential infinite loop */
    uint32_t max_iterations = hm->values->length;
    uint32_t iterations = 0;
    
    for (perturb = hash; iterations < max_iterations; 
         i = mask & (i * 5 + perturb + 1), perturb >>= 5, iterations++) {
        
        item = hm->values->items[i];
        if (item == NULL) {
            return -E_HASHMAP_KEY_NOT_FOUND;
        }
        if (item->is_deleted == 0 && item->hash == hash && strcmp(item->key, key) == 0) {
            return i;
        }
    }
    
    /* If we've probed everywhere and didn't find it, the key doesn't exist */
    return -E_HASHMAP_KEY_NOT_FOUND;
}

/**
 * Find empty or deleted slot for insertion.
 * @hm: Hashmap to search
 * @hash: Hash value to use for probing
 *
 * Uses the same probing sequence as _hashmap_find_index to find
 * the first empty or deleted slot.
 *
 * Return: Index of empty/deleted slot, or -E_HASHMAP_KEY_NOT_FOUND if table is full
 */
uint32_t _hashmap_find_empty_index(hashmap *hm, uint32_t hash)
{
    uint32_t mask = hm->values->length - 1;
    uint32_t i = hash & mask;
    uint32_t perturb;
    hashmap_item *item;
    
    /* Maximum number of probes - prevent potential infinite loop */
    uint32_t max_iterations = hm->values->length;
    uint32_t iterations = 0;
    
    for (perturb = hash; iterations < max_iterations; 
         i = mask & (i * 5 + perturb + 1), perturb >>= 5, iterations++) {
        
        item = hm->values->items[i];
        if (item == NULL || item->is_deleted == 1) {
            return i;
        }
    }

    /* If we've probed everywhere and all slots are full */
    return -E_HASHMAP_KEY_NOT_FOUND;
}

/**
 * DJB2 hash function for strings.
 * @s: String to hash
 *
 * This is the classic DJB2 hash algorithm:
 * hash(i) = hash(i-1) * 33 + str[i]
 *
 * Return: 32-bit hash value
 */
unsigned long hash(char *s)
{
    unsigned long hash = 5381;
    int c;

    while ((c = *s++)) {
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
    }

    return hash;
}
