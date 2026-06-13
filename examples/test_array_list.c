#include "ds_array_list.h"
#include <stdio.h>
#include <assert.h>

typedef struct { int count; int sum; } sum_ctx;

static void sum_callback(void *element, void *ctx) {
    sum_ctx *c = (sum_ctx *)ctx;
    c->sum += *(int *)element;
    c->count++;
}

void test_basic_ops(void) {
    ds_array_list *list = ds_array_list_create(4);
    assert(list != NULL);
    assert(ds_array_list_empty(list));
    assert(ds_array_list_size(list) == 0);

    int a = 1, b = 2, c = 3;
    assert(ds_array_list_push(list, &a) == 0);
    assert(ds_array_list_push(list, &b) == 0);
    assert(ds_array_list_push(list, &c) == 0);

    assert(ds_array_list_size(list) == 3);
    assert(*(int *)ds_array_list_front(list) == 1);
    assert(*(int *)ds_array_list_back(list) == 3);
    assert(*(int *)ds_array_list_get(list, 1) == 2);

    ds_array_list_pop(list);
    assert(ds_array_list_size(list) == 2);
    assert(*(int *)ds_array_list_back(list) == 2);

    ds_array_list_destroy(&list);
    assert(list == NULL);
    puts("  [PASS] basic_ops");
}

void test_insert_erase(void) {
    ds_array_list *list = ds_array_list_create(2);
    int vals[] = {10, 20, 30, 40};

    ds_array_list_push(list, &vals[0]);
    ds_array_list_push(list, &vals[2]);

    assert(ds_array_list_insert(list, 1, &vals[1]) == 0);
    assert(ds_array_list_size(list) == 3);
    assert(*(int *)ds_array_list_get(list, 1) == 20);

    ds_array_list_erase(list, 0);
    assert(ds_array_list_size(list) == 2);
    assert(*(int *)ds_array_list_get(list, 0) == 20);

    ds_array_list_destroy(&list);
    puts("  [PASS] insert_erase");
}

void test_foreach(void) {
    ds_array_list *list = ds_array_list_create(4);
    int vals[] = {1, 2, 3};
    sum_ctx ctx = {0};

    for (int i = 0; i < 3; i++) assert(ds_array_list_push(list, &vals[i]) == 0);

    ds_array_list_foreach(list, sum_callback, &ctx);
    assert(ctx.count == 3);
    assert(ctx.sum == 6);

    ds_array_list_destroy(&list);
    puts("  [PASS] foreach");
}

int main(void) {
    puts("ds_array_list tests:");
    test_basic_ops();
    test_insert_erase();
    test_foreach();
    puts("All array_list tests passed!");
    return 0;
}
