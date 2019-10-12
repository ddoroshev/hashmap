#include <stdlib.h>

#include "alloc.h"
#include "hashmap.h"
#include "tests/assert.h"

void test_hashmap_init(void)
{
    hashmap *hm = hashmap_init();

    ASSERT(hm->length == HASHMAP_BASE_SIZE);
    ASSERT(hm->keys->length == HASHMAP_BASE_SIZE);
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
    ASSERT(hm->keys == NULL);
    ASSERT(hm->values == NULL);
}

void test_hashmap_dump(void)
{
    hashmap *hm = hashmap_init();
    int val = 1;
    hashmap_set(hm, 10, &val);
    hashmap_set(hm, 11, &val);
    hashmap_set(hm, 12, &val);

    hashmap_dump(hm);

    hashmap_free(hm);
}

void test_hashmap_set(void)
{
    hashmap *hm = hashmap_init();
    int val = 1;
    WITH_SUCCESS_ALLOCS(0, ASSERT(hashmap_set(hm, 1, &val) == -E_HASHMAP_CANNOT_SET_KEY));
    WITH_SUCCESS_ALLOCS(1, ASSERT(hashmap_set(hm, 1, &val) == -E_HASHMAP_CANNOT_SET_VALUE));
    for (int i = 0; i < HASHMAP_BASE_SIZE; i++) {
        ASSERT(hashmap_set(hm, i, &val) == 0);
    }
    ASSERT(hashmap_set(hm, 100, &val) == -E_HASHMAP_FULL);

    hashmap_free(hm);
}

void test_hashmap_get(void)
{
    hashmap *hm = hashmap_init();
    for (int i = 0; i < 3; i++) {
        hashmap_set(hm, i, &i);
        ASSERT(*hashmap_get(hm, i) == i);
    }
    ASSERT(hashmap_get(hm, 4) == NULL);

    hashmap_free(hm);
}

void test_hashmap_delete(void)
{
    hashmap *hm = hashmap_init();
    for (int i = 0; i < 3; i++) {
        hashmap_set(hm, i, &i);
    }
    ASSERT(hashmap_delete(hm, 0) == 0);
    ASSERT(hashmap_delete(hm, 10) == -E_HASHMAP_KEY_NOT_FOUND);

    hashmap_free(hm);
}
