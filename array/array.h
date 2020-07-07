#pragma once

#include <stdint.h>

#define E_ARRAY_INDEX_OUT_OF_RANGE 1
#define E_ALLOC 2

typedef struct array array;

array *array_init(uint32_t length, uint32_t size);
uint32_t array_len(array *ar);
void array_free(array *ar);
void array_dump(array *ar);

int array_set(array *ar, uint32_t index, void *item);
void *array_get(array *ar, uint32_t index);
int array_delete(array *ar, uint32_t index);
