#pragma once

#include <stdio.h>

static short __has_errors;

#define ASSERT(chk) \
    if (!(chk)) { \
        __has_errors = 1; \
        printf("Assertion (%s) failed %s at line %d \n", #chk, __FILE__, __LINE__); \
        return; \
    }

#define FIN return -__has_errors
