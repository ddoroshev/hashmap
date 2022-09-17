#include <stdio.h>

#include "hashmap/hashmap.hpp"
#include "dump.hpp"

int main(void) {
    auto hm = std::make_unique<Hashmap<const char*, const char*>>();

    hm->Set("aaa", "BBB");
    HashmapItem<const char*, const char*> item = hm->Get("aaa");
    printf("%s %s\n", item.k, item.v);

    return 0;
}
