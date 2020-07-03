#include <stdio.h>
#include <stdlib.h>

int _test_success_allocs = -1;

void *tmalloc(size_t size) {
    void *result = NULL;
    if (_test_success_allocs != 0) {
        result = malloc(size);
        if (_test_success_allocs > 0) {
            _test_success_allocs--;
        }
    }

    return result;
}
