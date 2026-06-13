#include "ds_heap.h"
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

static int cmp_int(const void *a, const void *b) {
    int ia = *(const int *)a;
    int ib = *(const int *)b;
    if (ia > ib) return 1;
    if (ia < ib) return -1;
    return 0;
}

void test_basic_ops(void) {
    ds_heap *h = ds_heap_create(cmp_int);
    assert(h != NULL);
    assert(ds_heap_empty(h));
    assert(ds_heap_size(h) == 0);

    int vals[] = {3, 1, 4, 1, 5, 9, 2, 6};
    for (int i = 0; i < 8; i++) {
        assert(ds_heap_push(h, &vals[i]) == 0);
    }
    assert(ds_heap_size(h) == 8);
    assert(!ds_heap_empty(h));

    int *top = (int *)ds_heap_peek(h);
    assert(top != NULL && *top == 9);

    int expected[] = {9, 6, 5, 4, 3, 2, 1, 1};
    for (int i = 0; i < 8; i++) {
        int *v = (int *)ds_heap_pop(h);
        assert(v != NULL && *v == expected[i]);
    }
    assert(ds_heap_size(h) == 0);
    assert(ds_heap_pop(h) == NULL);

    ds_heap_destroy(&h);
    assert(h == NULL);
    puts("  [PASS] basic_ops");
}

void test_empty(void) {
    ds_heap *h = ds_heap_create(cmp_int);
    assert(ds_heap_peek(h) == NULL);
    assert(ds_heap_pop(h) == NULL);
    ds_heap_destroy(&h);
    puts("  [PASS] empty");
}

int main(void) {
    puts("ds_heap tests:");
    test_basic_ops();
    test_empty();
    puts("All heap tests passed!");
    return 0;
}
