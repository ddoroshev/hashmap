#include "tests/assert.h"

int main(void)
{
    test_array_init();
    test_array_init_fail();
    test_array_dump();
    test_array_free();
    test_array_get_value();
    test_array_set_value();
    test_array_delete_value();

    test_hashmap_init();
    test_hashmap_init_fail();
    test_hashmap_free();
    test_hashmap_dump();
    test_hashmap_set();
    test_hashmap_get();
    test_hashmap_delete();

    FIN;
}
