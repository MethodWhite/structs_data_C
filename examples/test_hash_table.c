#include "ds_hash_table.h"
#include <stdio.h>
#include <assert.h>
#include <string.h>

typedef struct { int count; } visit_ctx;

static void visit_callback(const char *key, void *value, void *ctx) {
    (void)key;
    (void)value;
    ((visit_ctx *)ctx)->count++;
}

void test_basic_ops(void) {
    ds_hash_table *table = ds_hash_table_create(8);
    assert(table != NULL);
    assert(ds_hash_table_empty(table));
    assert(ds_hash_table_size(table) == 0);

    int a = 42, b = 99, c = 123;
    assert(ds_hash_table_put(table, "foo", &a) == 0);
    assert(ds_hash_table_put(table, "bar", &b) == 0);
    assert(ds_hash_table_put(table, "baz", &c) == 0);

    assert(ds_hash_table_size(table) == 3);
    assert(ds_hash_table_contains(table, "foo"));
    assert(ds_hash_table_contains(table, "bar"));
    assert(!ds_hash_table_contains(table, "nonexistent"));
    assert(*(int *)ds_hash_table_get(table, "foo") == 42);
    assert(*(int *)ds_hash_table_get(table, "bar") == 99);
    assert(ds_hash_table_get(table, "nonexistent") == NULL);

    ds_hash_table_destroy(&table);
    assert(table == NULL);
    puts("  [PASS] basic_ops");
}

void test_overwrite(void) {
    ds_hash_table *table = ds_hash_table_create(8);
    int a = 1, b = 2;

    assert(ds_hash_table_put(table, "key", &a) == 0);
    assert(*(int *)ds_hash_table_get(table, "key") == 1);

    assert(ds_hash_table_put(table, "key", &b) == 0);
    assert(*(int *)ds_hash_table_get(table, "key") == 2);
    assert(ds_hash_table_size(table) == 1);

    ds_hash_table_destroy(&table);
    puts("  [PASS] overwrite");
}

void test_remove(void) {
    ds_hash_table *table = ds_hash_table_create(8);
    int a = 10, b = 20, c = 30;

    assert(ds_hash_table_put(table, "x", &a) == 0);
    assert(ds_hash_table_put(table, "y", &b) == 0);
    assert(ds_hash_table_put(table, "z", &c) == 0);
    assert(ds_hash_table_size(table) == 3);

    assert(ds_hash_table_remove(table, "y"));
    assert(!ds_hash_table_contains(table, "y"));
    assert(ds_hash_table_size(table) == 2);
    assert(*(int *)ds_hash_table_get(table, "x") == 10);
    assert(*(int *)ds_hash_table_get(table, "z") == 30);

    assert(!ds_hash_table_remove(table, "nonexistent"));
    assert(ds_hash_table_size(table) == 2);

    ds_hash_table_destroy(&table);
    puts("  [PASS] remove");
}

void test_foreach(void) {
    ds_hash_table *table = ds_hash_table_create(8);
    int a = 1, b = 2, c = 3;

    assert(ds_hash_table_put(table, "a", &a) == 0);
    assert(ds_hash_table_put(table, "b", &b) == 0);
    assert(ds_hash_table_put(table, "c", &c) == 0);

    visit_ctx ctx = { 0 };
    ds_hash_table_foreach(table, visit_callback, &ctx);
    assert(ctx.count == 3);

    ds_hash_table_destroy(&table);
    puts("  [PASS] foreach");
}

void test_resize(void) {
    ds_hash_table *table = ds_hash_table_create(4);
    int vals[100];
    for (int i = 0; i < 100; i++) {
        vals[i] = i;
        char key[16];
        snprintf(key, sizeof(key), "k%d", i);
        assert(ds_hash_table_put(table, key, &vals[i]) == 0);
    }
    assert(ds_hash_table_size(table) == 100);
    assert(ds_hash_table_capacity(table) >= 100);

    for (int i = 0; i < 100; i++) {
        char key[16];
        snprintf(key, sizeof(key), "k%d", i);
        assert(*(int *)ds_hash_table_get(table, key) == i);
    }

    ds_hash_table_destroy(&table);
    puts("  [PASS] resize");
}

int main(void) {
    puts("ds_hash_table tests:");
    test_basic_ops();
    test_overwrite();
    test_remove();
    test_foreach();
    test_resize();
    puts("All hash_table tests passed!");
    return 0;
}
