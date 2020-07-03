#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "alloc/alloc.h"
#include "array.h"
#include "dump.h"
#include "hashmap/hashmap_item.h"
#include "tests/assert.h"

const int item_size = sizeof(hashmap_item);

void test_array_init(void)
{
    int length = 10;
    array *ar = array_init(length, item_size);
    ASSERT(array_len(ar) == length);
    for (int i = 0; i < length; i++) {
        ASSERT(array_get(ar, i) == NULL);
    }
    array_free(ar);
}

void test_array_init_fail(void)
{
    WITH_SUCCESS_ALLOCS(0, ASSERT(array_init(1, item_size) == NULL));
    WITH_SUCCESS_ALLOCS(1, ASSERT(array_init(1, item_size) == NULL));
}

void test_array_dump(void)
{
    array *ar = array_init(3, item_size);
    array_set(ar, 0, &(hashmap_item){"ab", 1});
    array_set(ar, 1, &(hashmap_item){"bc", 2});
    array_set(ar, 2, &(hashmap_item){"cd", 3});

    array_dump(ar);
}

void test_array_free(void)
{
    array *ar = array_init(3, item_size);
    array_free(ar);
    ASSERT(array_empty(ar) == 1);
}

void test_array_set(void)
{
    const int length = 3;
    array *ar = array_init(length, item_size);

    ASSERT(array_set(ar, 1, &(hashmap_item){"ab", 2}) == 0);
    ASSERT(strcmp(((hashmap_item*)array_get(ar, 1))->key, "ab") == 0);
    ASSERT(((hashmap_item*)array_get(ar, 1))->value == 2);

    ASSERT(array_set(ar, -1, &(hashmap_item){"a", 2}) == -E_ARRAY_INDEX_OUT_OF_RANGE);
    ASSERT(array_set(ar, length, &(hashmap_item){"a", 2}) == -E_ARRAY_INDEX_OUT_OF_RANGE);

    WITH_SUCCESS_ALLOCS(0, ASSERT(array_set(ar, 1, &(hashmap_item){"a", 2}) == -E_ALLOC))

    array_free(ar);
}

void test_array_get(void)
{
    const int length = 3;
    array *ar = array_init(length, item_size);

    array_set(ar, 1, &(hashmap_item){"ab", 2});

    ASSERT(strcmp(((hashmap_item*)array_get(ar, 1))->key, "ab") == 0);
    ASSERT(((hashmap_item*)array_get(ar, 1))->value == 2);
    ASSERT(array_get(ar, -1) == NULL);
    ASSERT(array_get(ar, length) == NULL);

    array_free(ar);
}

void test_array_delete(void)
{
    const int length = 3;
    array *ar = array_init(length, item_size);

    array_set(ar, 1, &(hashmap_item){"a", 2});
    ASSERT(array_delete(ar, 1) == 0);
    ASSERT(array_get(ar, 1) == NULL);

    ASSERT(array_delete(ar, -1) == -E_ARRAY_INDEX_OUT_OF_RANGE);
    ASSERT(array_delete(ar, length) == -E_ARRAY_INDEX_OUT_OF_RANGE);

    array_free(ar);
}
