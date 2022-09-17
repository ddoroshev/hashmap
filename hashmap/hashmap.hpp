#pragma once

#include <cstdlib>
#include <cstdint>
#include <new>
#include <memory>

#define HASHMAP_BASE_SIZE 8

// From CPython
#define USABLE_FRACTION(n) (((n) << 1) / 3)
#define ESTIMATE_SIZE(n)   (((n)*3+1) >> 1)

#define TH template<class K, class V>
#define Hashmap_t Hashmap<K, V>
#define HashmapItem_t HashmapItem<K, V>

using namespace std;

TH
struct HashmapItem {
    K k;
    V v;
    bool active;
};

unsigned long hash_key(int i) {
    return (unsigned long)i;
}

unsigned long hash_key(char c) {
    return (unsigned long)c;
}

unsigned long hash_key(const char *s) {
    unsigned long hash = 5381;
    int c;

    while ((c = *s++)) {
        hash = ((hash << 5) + hash) + c;
    }

    return hash;
}

TH
struct Hashmap {

uint32_t count = 0;  // number of keys
uint32_t capacity = HASHMAP_BASE_SIZE;  // total number of slots in hashmap
HashmapItem_t *items;

Hashmap() {
    items = new HashmapItem_t[capacity]();
}

~Hashmap() {  delete[] items;  }

auto resize() {
    HashmapItem_t *prev_items = items;
    uint32_t prev_capacity = capacity;
    int new_capacity = ESTIMATE_SIZE(capacity);
    items = new HashmapItem_t[new_capacity]();
    count = 0;
    capacity = new_capacity;
    for (uint32_t i = 0; i < prev_capacity; i++) {
        HashmapItem_t item = prev_items[i];
        if (!item.active) continue;
        if (SetWOResize(item.k, item.v) == -1) return -1;
    }
    delete[] prev_items;
    return 0;
}

auto ensureSize() {
    if (count < USABLE_FRACTION(capacity)) return 0;
    return resize();
}

auto Set(K key, V value) {
    if (ensureSize() == -1) return -1;
    return SetWOResize(key, value);
}

auto SetWOResize(K key, V value) {
    int index = findEmptyIndex(key);
    if (index == -1) return -1;
    items[index] = (HashmapItem_t) {key, value, true};
    count++;
    return 0;
}

auto Del(K key) {
    int index = findIndex(key);
    if (index == -1) return -1;
    items[index].active = false;
    count--;
    return 0;
}

auto Get(K key) -> HashmapItem_t {
    int index = findIndex(key);
    if (index == -1) return {0, 0, false};
    return items[index];
}

auto findIndex(K key) {
    int hash = (int)hash_key(key);
    int init_index = (int)(hash % capacity);
    int index = init_index;
    int step = 1;
    do {
        HashmapItem_t item = items[index];
        if (item.k == key && item.active) return (int)index;
        index = (int)((index + step * step) % capacity);
        step++;
    } while (index != init_index);
    return -1;
}

auto findEmptyIndex(K key) {
    int hash = (int)hash_key(key);
    int init_index = (int)(hash % capacity);
    int index = init_index;
    int step = 1;
    do {
        HashmapItem_t item = items[index];
        if (!item.active) return (int)index;
        index = (int)((index + step * step) % capacity);
        step++;
    } while (index != init_index);
    return -1;
}

};
