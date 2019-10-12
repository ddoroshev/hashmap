#include "tests/assert.h"

int main(void)
{
    test_array_init();
    test_array_init_fail();
    test_array_dump();
    test_array_free();
    test_array_set_value();
    test_array_delete_value();

    FIN;
}
