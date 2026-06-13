#include "ds_tensor.h"
#include <stdio.h>
#include <assert.h>

void test_create_destroy(void) {
    size_t dims[] = {2, 3, 4};
    ds_tensor *t = ds_tensor_create(3, dims);
    assert(t != NULL);
    assert(ds_tensor_ndim(t) == 3);
    assert(ds_tensor_size(t) == 24);

    const size_t *shape = ds_tensor_shape(t);
    assert(shape[0] == 2);
    assert(shape[1] == 3);
    assert(shape[2] == 4);

    ds_tensor_destroy(&t);
    assert(t == NULL);
    puts("  [PASS] create_destroy");
}

void test_get_set(void) {
    size_t dims[] = {2, 3};
    ds_tensor *t = ds_tensor_create(2, dims);
    assert(t != NULL);

    int vals[] = {1, 2, 3, 4, 5, 6};
    size_t idx = 0;
    for (size_t i = 0; i < 2; i++) {
        for (size_t j = 0; j < 3; j++) {
            size_t indices[] = {i, j};
            ds_tensor_set(t, indices, &vals[idx++]);
        }
    }

    size_t indices[] = {1, 2};
    int *v = (int *)ds_tensor_get(t, indices);
    assert(v != NULL && *v == 6);

    size_t bad_idx[] = {0, 5};
    assert(ds_tensor_get(t, bad_idx) == NULL);

    ds_tensor_destroy(&t);
    puts("  [PASS] get_set");
}

void test_reshape(void) {
    size_t dims[] = {6};
    ds_tensor *t = ds_tensor_create(1, dims);
    assert(t != NULL);

    int vals[] = {10, 20, 30, 40, 50, 60};
    for (size_t i = 0; i < 6; i++) {
        size_t idx[] = {i};
        ds_tensor_set(t, idx, &vals[i]);
    }

    size_t new_dims[] = {2, 3};
    assert(ds_tensor_reshape(t, 2, new_dims) == 0);
    assert(ds_tensor_ndim(t) == 2);
    assert(ds_tensor_size(t) == 6);

    size_t indices[] = {1, 2};
    int *v = (int *)ds_tensor_get(t, indices);
    assert(v != NULL && *v == 60);

    size_t bad_dims[] = {2, 4};
    assert(ds_tensor_reshape(t, 2, bad_dims) == -1);

    ds_tensor_destroy(&t);
    puts("  [PASS] reshape");
}

void test_slice(void) {
    size_t dims[] = {3, 4};
    ds_tensor *t = ds_tensor_create(2, dims);
    assert(t != NULL);

    int vals[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11};
    size_t idx = 0;
    for (size_t i = 0; i < 3; i++) {
        for (size_t j = 0; j < 4; j++) {
            size_t indices[] = {i, j};
            ds_tensor_set(t, indices, &vals[idx++]);
        }
    }

    ds_tensor *s = ds_tensor_slice(t, 0, 1, 3);
    assert(s != NULL);
    assert(ds_tensor_size(s) == 8);

    size_t si[] = {0, 0};
    int *v = (int *)ds_tensor_get(s, si);
    assert(v != NULL && *v == 4);

    ds_tensor_destroy(&s);
    ds_tensor_destroy(&t);
    puts("  [PASS] slice");
}

int main(void) {
    puts("ds_tensor tests:");
    test_create_destroy();
    test_get_set();
    test_reshape();
    test_slice();
    puts("All tensor tests passed!");
    return 0;
}
