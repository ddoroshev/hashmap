#pragma once

extern int _test_success_allocs;
void *tcalloc(size_t count, size_t size);

#define WITH_SUCCESS_ALLOCS(num, code) { \
    int _prev_test_success_allocs = _test_success_allocs; \
    _test_success_allocs = num; \
    code; \
    _test_success_allocs = _prev_test_success_allocs; \
}

#define calloc tcalloc
