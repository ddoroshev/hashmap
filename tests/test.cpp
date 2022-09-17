#include "assert.h"
#include "hashmap/test_hashmap.cpp"

int main() {
    test_hashmap_init();
    test_hashmap_dump();
    test_hashmap_set();
    test_hashmap_get();
    test_hashmap_delete();
    test_hashmap_find_index();
    test_hashmap_collision_1();
    test_hashmap_collision_2();
    test_hashmap_fill_and_read();
    test_set_wo_resize_fail();

    FIN;
}
