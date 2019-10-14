#include "tests/assert.h"

int main(void)
{
    test_array_init();
    test_array_init_fail();
    test_array_dump();
    test_array_free();
    test_array_get_item();
    test_array_set_item();
    test_array_delete_item();

    test_hashmap_init();
    test_hashmap_init_fail();
    test_hashmap_free();
    test_hashmap_dump();
    test_hashmap_set();
    test_hashmap_get();
    test_hashmap_delete();
    test_hashmap_find_index();

    FIN;
}
