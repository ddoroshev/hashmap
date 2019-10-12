#pragma once

#include <stdio.h>

#undef ASSERT

#define ASSERT(chk)  \
    if (!(chk)) { \
        printf("Assertion (%s) failed %s at line %d \n", #chk, __FILE__, __LINE__); \
        return; \
    }
