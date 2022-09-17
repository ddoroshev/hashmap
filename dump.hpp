#pragma once

#include <cstdio>
#include <memory>
#include "hashmap/hashmap.hpp"

void hashmap_dump(Hashmap<int, int> *hm) {
    for (uint32_t i = 0; i < hm->capacity; i++) {
        HashmapItem<int, int> item = hm->items[i];
        printf("%i => %d (%d)\n", item.k, item.v, item.active);
    }
}
void hashmap_dump(std::unique_ptr<Hashmap<int, int>> & hm) {
    for (uint32_t i = 0; i < hm->capacity; i++) {
        HashmapItem<int, int> item = hm->items[i];
        printf("%i => %d (%d)\n", item.k, item.v, item.active);
    }
}
