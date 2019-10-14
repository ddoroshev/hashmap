#include <stdlib.h>

#include "alloc.h"
#include "hashmap.h"
#include "tests/assert.h"

char *keys[] = {
    "ab",  // 5863208 -> 0
    "bc",  // 5863242 -> 2
    "cd",  // 5863276 -> 4
    "de",  // 5863310 -> 6
    "ef",  // 5863344 -> 0
    "fg",  // 5863378 -> 2
    "gh",  // 5863412 -> 4
    "hi"   // 5863446 -> 6
};

void test_hashmap_init(void)
{
    hashmap *hm = hashmap_init();

    ASSERT(hm->length == HASHMAP_BASE_SIZE);
    ASSERT(hm->values->length == HASHMAP_BASE_SIZE);

    hashmap_free(hm);
}

void test_hashmap_init_fail(void)
{
    WITH_SUCCESS_ALLOCS(0, ASSERT(hashmap_init(1) == NULL));
    WITH_SUCCESS_ALLOCS(1, ASSERT(hashmap_init(1) == NULL));
    WITH_SUCCESS_ALLOCS(2, ASSERT(hashmap_init(1) == NULL));
}

void test_hashmap_free(void)
{
    hashmap *hm = hashmap_init();
    hashmap_free(hm);
    ASSERT(hm->values == NULL);
}

void test_hashmap_dump(void)
{
    hashmap *hm = hashmap_init();
    int val = 1;
    hashmap_set(hm, "ab", val);
    hashmap_set(hm, "bc", val);
    hashmap_set(hm, "cd", val);

    hashmap_dump(hm);

    hashmap_free(hm);
}

void test_hashmap_set(void)
{
    hashmap *hm = hashmap_init();
    int val = 1;
    WITH_SUCCESS_ALLOCS(0, ASSERT(hashmap_set(hm, "ab", val) == -E_HASHMAP_CANNOT_SET_VALUE));
    for (int i = 0; i < HASHMAP_BASE_SIZE; i++) {
        ASSERT(hashmap_set(hm, keys[i], val) == 0);
    }
    ASSERT(hashmap_set(hm, "foo", val) == -E_HASHMAP_FULL);

    hashmap_free(hm);
}

void test_hashmap_get(void)
{
    hashmap *hm = hashmap_init();
    hashmap_item *item;

    for (int i = 0; i < 3; i++) {
        hashmap_set(hm, keys[i], i);
        item = hashmap_get(hm, keys[i]);
        ASSERT(strcmp(item->key, keys[i]) == 0);
        ASSERT(item->value == i);
    }
    ASSERT(hashmap_get(hm, keys[4]) == NULL);

    hashmap_free(hm);
}

void test_hashmap_delete(void)
{
    hashmap *hm = hashmap_init();
    int i;
    for (i = 0; i < 3; i++) {
        hashmap_set(hm, keys[i], i);
    }

    ASSERT(hashmap_delete(hm, "foo") == -E_HASHMAP_KEY_NOT_FOUND);

    for (i = 0; i < 3; i++) {
        ASSERT(hashmap_delete(hm, keys[i]) == 0);
    }
    ASSERT(hashmap_delete(hm, "bar") == -E_HASHMAP_KEY_NOT_FOUND);

    hashmap_free(hm);
}

void test_hashmap_find_index(void)
{
    hashmap *hm = hashmap_init();
    int i;
    for (i = 0; i < 7; i++) {
        hashmap_set(hm, keys[i], i);
    }
    ASSERT(_hashmap_find_index(hm, keys[0]) == 0);
    ASSERT(_hashmap_find_index(hm, keys[6]) == 5);
    ASSERT(_hashmap_find_index(hm, keys[7]) == -E_HASHMAP_KEY_NOT_FOUND);

    i++;
    hashmap_set(hm, keys[7], i);

    ASSERT(_hashmap_find_index(hm, "foo") == -E_HASHMAP_KEY_NOT_FOUND);

    hashmap_free(hm);
}
