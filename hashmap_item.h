#pragma once

typedef struct {
    char *key;
    int value;
    int is_deleted;
    unsigned long hash;
} hashmap_item;
