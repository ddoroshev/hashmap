#ifdef UNIT_TEST

#include <stdio.h>
#include <stdlib.h>

int _test_success_allocs = -1;

void *tcalloc(size_t count, size_t size) {
    void *result = NULL;
    if (_test_success_allocs != 0) {
        result = calloc(count, size);
        if (_test_success_allocs > 0) {
            _test_success_allocs--;
        }
    }

    return result;
}

#endif
