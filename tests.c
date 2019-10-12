#include <assert.h>
#include <stdlib.h>

#include "alloc.h"
#include "array.h"

void test_array_init(void)
{
    int length = 10;
    array *ar = array_init(length);
    assert(ar->length == length);
    for (int i = 0; i < length; i++) {
        assert(ar->values[i] == NULL);
    }
    array_free(ar);
}

void test_array_init_fail(void)
{
    WITH_SUCCESS_ALLOCS(0, assert(array_init(1) == NULL));
    WITH_SUCCESS_ALLOCS(1, assert(array_init(1) == NULL));
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
    assert(ar->values == NULL);
}

void test_array_set_value(void)
{
    const int length = 3;
    array *ar = array_init(length);

    assert(array_set_value(ar, 1, 2) == 0);
    assert(*ar->values[1] == 2);

    assert(array_set_value(ar, -1, 2) == -E_ARRAY_INDEX_OUT_OF_RANGE);
    assert(array_set_value(ar, length, 2) == -E_ARRAY_INDEX_OUT_OF_RANGE);

    WITH_SUCCESS_ALLOCS(0, assert(array_set_value(ar, 1, 2) == -E_ALLOC))

    array_free(ar);
}

void test_array_get_value(void)
{
    const int length = 3;
    array *ar = array_init(length);

    array_set_value(ar, 1, 2);

    assert(*array_get_value(ar, 1) == 2);
    assert(array_get_value(ar, -1) == NULL);
    assert(array_get_value(ar, length) == NULL);

    array_free(ar);
}

void test_array_delete_value(void)
{
    const int length = 3;
    array *ar = array_init(length);

    array_set_value(ar, 1, 2);
    array_delete_value(ar, 1);

    assert(array_get_value(ar, 1) != NULL);

    array_free(ar);
}

int main(void)
{
    test_array_init();
    test_array_init_fail();
    test_array_dump();
    test_array_free();
    test_array_set_value();
    test_array_delete_value();
}
