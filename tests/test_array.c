#include <stdlib.h>

#include "alloc.h"
#include "array.h"
#include "tests/assert.h"

void test_array_init(void)
{
    int length = 10;
    array *ar = array_init(length);
    ASSERT(ar->length == length);
    for (int i = 0; i < length; i++) {
        ASSERT(ar->values[i] == NULL);
    }
    array_free(ar);
}

void test_array_init_fail(void)
{
    WITH_SUCCESS_ALLOCS(0, ASSERT(array_init(1) == NULL));
    WITH_SUCCESS_ALLOCS(1, ASSERT(array_init(1) == NULL));
}

void test_array_dump(void)
{
    array *ar = array_init(3);
    array_set_value(ar, 0, 1);
    array_set_value(ar, 1, 2);
    array_set_value(ar, 2, 3);

    array_dump(ar);
}

void test_array_free(void)
{
    array *ar = array_init(3);
    array_free(ar);
    ASSERT(ar->values != NULL);
}

void test_array_set_value(void)
{
    const int length = 3;
    array *ar = array_init(length);

    ASSERT(array_set_value(ar, 1, 2) == 0);
    ASSERT(*ar->values[1] == 2);

    ASSERT(array_set_value(ar, -1, 2) == -E_ARRAY_INDEX_OUT_OF_RANGE);
    ASSERT(array_set_value(ar, length, 2) == -E_ARRAY_INDEX_OUT_OF_RANGE);

    WITH_SUCCESS_ALLOCS(0, ASSERT(array_set_value(ar, 1, 2) == -E_ALLOC))

    array_free(ar);
}

void test_array_get_value(void)
{
    const int length = 3;
    array *ar = array_init(length);

    array_set_value(ar, 1, 2);

    ASSERT(*array_get_value(ar, 1) == 2);
    ASSERT(array_get_value(ar, -1) == NULL);
    ASSERT(array_get_value(ar, length) == NULL);

    array_free(ar);
}

void test_array_delete_value(void)
{
    const int length = 3;
    array *ar = array_init(length);

    array_set_value(ar, 1, 2);
    array_delete_value(ar, 1);

    ASSERT(array_get_value(ar, 1) == NULL);

    array_free(ar);
}
