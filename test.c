#include "assert.h"

int main(void)
{
    printf("Testing hashmap_init...\n");
    test_hashmap_init();
    printf("Testing hashmap_init_fail...\n");
    test_hashmap_init_fail();
    printf("Testing hashmap_free...\n");
    test_hashmap_free();
    printf("Testing hashmap_dump...\n");
    test_hashmap_dump();
    printf("Testing hashmap_set...\n");
    test_hashmap_set();
    printf("Testing hashmap_get...\n");
    test_hashmap_get();
    printf("Testing hashmap_delete...\n");
    test_hashmap_delete();
    printf("Testing hashmap_find_index...\n");
    test_hashmap_find_index();
    printf("Testing hashmap_key_memory_management...\n");
    test_hashmap_key_memory_management();
    FIN;
}
