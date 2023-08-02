#include "tests/assert.h"

int main(void)
{
    test_hashmap_init();
    test_hashmap_init_fail();
    test_hashmap_free();
    test_hashmap_dump();
    test_hashmap_set();
    test_hashmap_get();
    test_hashmap_delete();
    test_hashmap_find_index();

    test_hashmap_key_memory_management();
    FIN;
}
