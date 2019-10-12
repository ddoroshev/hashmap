#include <stdio.h>
#include <stdlib.h>

#define HASH_SIZE 8
#define HASH_MASK 0b111

typedef char HASH_KEY[3];
typedef int* HASH_VALUE;

typedef struct {
    HASH_VALUE values[HASH_SIZE];
} hash_map;

void init_hm(hash_map*);
int get_value(hash_map*, HASH_KEY);
void set_value(hash_map*, HASH_KEY, HASH_VALUE);
void delete_value(hash_map*, HASH_KEY);
int hashsum(HASH_KEY);
void dump_pointers(hash_map*);

int main(void)
{
    int i;
    char keys[][HASH_SIZE] = {
        "abc", "cde", "def", "efg",
        "ghi", "hij", "ijk", "jkl"
    };
    hash_map hm;
    init_hm(&hm);

    for (i = 0; i < HASH_SIZE; i++) {
        int val = i;
        set_value(&hm, keys[i], &val);
    }
    dump_pointers(&hm);
    for (i = 0; i < HASH_SIZE; i++) {
        get_value(&hm, keys[i]);
    }

    return 0;
}

void init_hm(hash_map *hm)
{
    for (int i = 0; i < HASH_SIZE; i++) {
        hm->values[i] = NULL;
    }
}

int get_value(hash_map *hm, HASH_KEY key)
{
    int h = hashsum(key);
    int prob_index = h & HASH_MASK;
    if (hm->values[prob_index] == NULL) {
        return -1;
    }
    int val = *hm->values[prob_index];
    printf("get_value: [%s] -> %d, %d\n", key, prob_index, val);
    return val;
}

void set_value(hash_map *hm, HASH_KEY key, HASH_VALUE value)
{
    HASH_VALUE _value = (HASH_VALUE)malloc(sizeof(value));
    _value = value;
    printf("addr %p\n", _value);
    int h = hashsum(key);
    int prob_index = h & HASH_MASK;
    hm->values[prob_index] = _value;

    printf("set [%s (%d)] -> [%d]\n", key, prob_index, *_value);
}

void delete_value(hash_map *hm, HASH_KEY key)
{
    return;
}

void dump_pointers(hash_map *hm)
{
    for (int i = 0; i < HASH_SIZE; i++) {
        printf("%p\n", hm->values[i]);
    }
}

int hashsum(HASH_KEY key)
{
    int h = 0;
    for (int i = 0; i < 3; i++) {
        int charcode = (int)key[i];
        h += charcode;
    }
    return h;
}
