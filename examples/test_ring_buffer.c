#include "ds_ring_buffer.h"
#include <stdio.h>
#include <assert.h>

void test_basic_ops(void) {
    ds_ring_buffer *rb = ds_ring_buffer_create(4);
    assert(rb != NULL);
    assert(ds_ring_buffer_empty(rb));
    assert(!ds_ring_buffer_full(rb));
    assert(ds_ring_buffer_size(rb) == 0);
    assert(ds_ring_buffer_capacity(rb) == 4);

    int a = 10, b = 20, c = 30;
    assert(ds_ring_buffer_push(rb, &a) == 0);
    assert(ds_ring_buffer_push(rb, &b) == 0);
    assert(ds_ring_buffer_push(rb, &c) == 0);
    assert(ds_ring_buffer_size(rb) == 3);

    assert(*(int *)ds_ring_buffer_peek(rb, 0) == 10);
    assert(*(int *)ds_ring_buffer_peek(rb, 1) == 20);
    assert(*(int *)ds_ring_buffer_peek(rb, 2) == 30);

    assert(*(int *)ds_ring_buffer_pop(rb) == 10);
    assert(ds_ring_buffer_size(rb) == 2);
    assert(*(int *)ds_ring_buffer_peek(rb, 0) == 20);

    ds_ring_buffer_destroy(&rb);
    assert(rb == NULL);
    puts("  [PASS] basic_ops");
}

void test_full(void) {
    ds_ring_buffer *rb = ds_ring_buffer_create(2);
    int a = 1, b = 2, c = 3;

    assert(ds_ring_buffer_push(rb, &a) == 0);
    assert(ds_ring_buffer_push(rb, &b) == 0);
    assert(ds_ring_buffer_full(rb));
    assert(ds_ring_buffer_push(rb, &c) == -1);

    ds_ring_buffer_destroy(&rb);
    puts("  [PASS] full");
}

void test_wrap_around(void) {
    ds_ring_buffer *rb = ds_ring_buffer_create(3);
    int vals[] = {1, 2, 3, 4, 5};

    assert(ds_ring_buffer_push(rb, &vals[0]) == 0);
    assert(ds_ring_buffer_push(rb, &vals[1]) == 0);
    assert(ds_ring_buffer_push(rb, &vals[2]) == 0);
    assert(ds_ring_buffer_full(rb));

    assert(*(int *)ds_ring_buffer_pop(rb) == 1);
    assert(ds_ring_buffer_push(rb, &vals[3]) == 0);
    assert(*(int *)ds_ring_buffer_pop(rb) == 2);
    assert(ds_ring_buffer_push(rb, &vals[4]) == 0);

    assert(*(int *)ds_ring_buffer_pop(rb) == 3);
    assert(*(int *)ds_ring_buffer_pop(rb) == 4);
    assert(*(int *)ds_ring_buffer_pop(rb) == 5);
    assert(ds_ring_buffer_empty(rb));

    ds_ring_buffer_destroy(&rb);
    puts("  [PASS] wrap_around");
}

void test_empty_ops(void) {
    ds_ring_buffer *rb = ds_ring_buffer_create(3);
    assert(ds_ring_buffer_pop(rb) == NULL);
    assert(ds_ring_buffer_peek(rb, 0) == NULL);
    assert(ds_ring_buffer_empty(rb));
    ds_ring_buffer_destroy(&rb);
    puts("  [PASS] empty_ops");
}

int main(void) {
    puts("ds_ring_buffer tests:");
    test_basic_ops();
    test_full();
    test_wrap_around();
    test_empty_ops();
    puts("All ring_buffer tests passed!");
    return 0;
}
