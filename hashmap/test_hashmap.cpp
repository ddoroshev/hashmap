#include <memory>
#include "hashmap.hpp"
#include "dump.hpp"

using namespace std;

int keys[] = {
    12,  // -> 4
    23,  // -> 7
    34,  // -> 2
    45,  // -> 5
    56,  // -> 0
    67,  // -> 3
    78,  // -> 6
    89   // -> 1
};

void test_hashmap_init() {
    auto h = new Hashmap<int, int>();

    ASSERT(h->count == 0);
    ASSERT(h->capacity == HASHMAP_BASE_SIZE);

    delete h;
}

void test_hashmap_dump() {
    auto h = make_unique<Hashmap<int, int>>();

    int val = 1;
    h->Set(12, val);
    h->Set(23, val);
    h->Set(34, val);
    hashmap_dump(h);
}

void test_hashmap_set() {
    auto h = make_unique<Hashmap<int, int>>();

    int val = 1;
    for (uint32_t i = 0; i < HASHMAP_BASE_SIZE; i++) {
        h->Set(keys[i], val);
        ASSERT(h->count == i + 1);
    }
    h->Set(123, val);
    ASSERT(h->count == 9);
    ASSERT(h->capacity == 18);
}

void test_hashmap_get() {
    auto h = make_unique<Hashmap<int, int>>();

    for (int i = 0; i < 3; i++) {
        h->Set(keys[i], i);
        ASSERT(h->Get(keys[i]).v == i);
    }
    ASSERT(!h->Get(keys[4]).active)
}

void test_hashmap_delete() {
    auto h = make_unique<Hashmap<int, int>>();

    int i;
    for (i = 0; i < 3; i++) h->Set(keys[i], i);
    ASSERT(h->Del(123) == -1);
    for (i = 0; i < 3; i++) {
        ASSERT(h->Del(keys[i]) == 0);
    }
    ASSERT(h->count == 0);
    ASSERT(h->Del(456) == -1);
}

void test_hashmap_find_index() {
    auto h = make_unique<Hashmap<int, int>>();

    int i;
    for (i = 0; i < 7; i++) h->Set(keys[i], i);
    ASSERT(h->findIndex(keys[0]) > -1);
    ASSERT(h->findIndex(keys[6]) > -1);
    ASSERT(h->findIndex(keys[7]) == -1);
    h->Set(keys[7], i + 1);
    ASSERT(h->findIndex(123) == -1);
}

void test_hashmap_collision_1() {
    auto h = make_unique<Hashmap<int, int>>();

    h->Set(0, 100);
    h->Set(8, 200);
    h->Set(16, 300);
    ASSERT(h->items[0].v == 100);
    ASSERT(h->items[1].v == 200);
    ASSERT(h->items[5].v == 300);

    h->Del(0);
    ASSERT(!h->Get(0).active);
    ASSERT(h->Get(8).active);
    ASSERT(h->Get(16).active);

    h->Del(8);
    ASSERT(!h->Get(0).active);
    ASSERT(!h->Get(8).active);
    ASSERT(h->Get(16).active);

    h->Del(16);
    ASSERT(!h->Get(0).active);
    ASSERT(!h->Get(8).active);
    ASSERT(!h->Get(16).active);
}

void test_hashmap_collision_2() {
    auto h = make_unique<Hashmap<int, int>>();

    h->Set(16, 300);
    h->Set(8, 200);
    h->Set(0, 100);
    ASSERT(h->items[0].v == 300);
    ASSERT(h->items[1].v == 200);
    ASSERT(h->items[5].v == 100);

    h->Del(16);
    ASSERT(h->Get(0).active);
    ASSERT(h->Get(8).active);
    ASSERT(!h->Get(16).active);

    h->Del(8);
    ASSERT(h->Get(0).active);
    ASSERT(!h->Get(8).active);
    ASSERT(!h->Get(16).active);

    h->Del(0);
    ASSERT(!h->Get(0).active);
    ASSERT(!h->Get(8).active);
    ASSERT(!h->Get(16).active);
}

void test_hashmap_fill_and_read() {
    auto h = make_unique<Hashmap<int, int>>();

    for (int i = 0; i < 99999; i++) h->Set(i, i);
    for (int i = 0; i < 99999; i++) {
        HashmapItem<int, int> item = h->Get(i);
        ASSERT(item.active);
        ASSERT(item.v == i);
    }
}

void test_set_wo_resize_fail() {
    auto h = make_unique<Hashmap<int, int>>();

    for (uint32_t i = 0; i < h->capacity; i++) {
        h->items[i].active = true;
    }

    int res = h->SetWOResize(1, 2);
    ASSERT(res == -1);
}
