#include "ds_matrix.h"
#include <stdio.h>
#include <assert.h>
#include <math.h>

void test_create_destroy(void) {
    ds_matrix *m = ds_matrix_create(3, 4);
    assert(m != NULL);
    ds_matrix_destroy(&m);
    assert(m == NULL);
    puts("  [PASS] create_destroy");
}

void test_get_set(void) {
    ds_matrix *m = ds_matrix_create(2, 3);
    assert(m != NULL);

    ds_matrix_set(m, 0, 0, 1.0);
    ds_matrix_set(m, 1, 2, 5.5);
    assert(ds_matrix_get(m, 0, 0) == 1.0);
    assert(ds_matrix_get(m, 1, 2) == 5.5);
    assert(ds_matrix_get(m, 0, 1) == 0.0);

    ds_matrix_destroy(&m);
    puts("  [PASS] get_set");
}

void test_add_sub(void) {
    ds_matrix *a = ds_matrix_create(2, 2);
    ds_matrix *b = ds_matrix_create(2, 2);
    ds_matrix_set(a, 0, 0, 1); ds_matrix_set(a, 0, 1, 2);
    ds_matrix_set(a, 1, 0, 3); ds_matrix_set(a, 1, 1, 4);
    ds_matrix_set(b, 0, 0, 5); ds_matrix_set(b, 0, 1, 6);
    ds_matrix_set(b, 1, 0, 7); ds_matrix_set(b, 1, 1, 8);

    ds_matrix *c = ds_matrix_add(a, b);
    assert(c != NULL);
    assert(ds_matrix_get(c, 0, 0) == 6);
    assert(ds_matrix_get(c, 1, 1) == 12);

    ds_matrix *d = ds_matrix_sub(a, b);
    assert(d != NULL);
    assert(ds_matrix_get(d, 0, 0) == -4);
    assert(ds_matrix_get(d, 1, 1) == -4);

    ds_matrix_destroy(&a);
    ds_matrix_destroy(&b);
    ds_matrix_destroy(&c);
    ds_matrix_destroy(&d);
    puts("  [PASS] add_sub");
}

void test_mul(void) {
    ds_matrix *a = ds_matrix_create(2, 3);
    ds_matrix *b = ds_matrix_create(3, 2);
    ds_matrix_set(a, 0, 0, 1); ds_matrix_set(a, 0, 1, 2); ds_matrix_set(a, 0, 2, 3);
    ds_matrix_set(a, 1, 0, 4); ds_matrix_set(a, 1, 1, 5); ds_matrix_set(a, 1, 2, 6);
    ds_matrix_set(b, 0, 0, 7); ds_matrix_set(b, 0, 1, 8);
    ds_matrix_set(b, 1, 0, 9); ds_matrix_set(b, 1, 1, 10);
    ds_matrix_set(b, 2, 0, 11); ds_matrix_set(b, 2, 1, 12);

    ds_matrix *c = ds_matrix_mul(a, b);
    assert(c != NULL);
    assert(ds_matrix_get(c, 0, 0) == 58);
    assert(ds_matrix_get(c, 0, 1) == 64);
    assert(ds_matrix_get(c, 1, 0) == 139);
    assert(ds_matrix_get(c, 1, 1) == 154);

    ds_matrix_destroy(&a);
    ds_matrix_destroy(&b);
    ds_matrix_destroy(&c);
    puts("  [PASS] mul");
}

void test_transpose(void) {
    ds_matrix *m = ds_matrix_create(2, 3);
    ds_matrix_set(m, 0, 0, 1); ds_matrix_set(m, 0, 1, 2); ds_matrix_set(m, 0, 2, 3);
    ds_matrix_set(m, 1, 0, 4); ds_matrix_set(m, 1, 1, 5); ds_matrix_set(m, 1, 2, 6);

    ds_matrix *t = ds_matrix_transpose(m);
    assert(t != NULL);
    assert(ds_matrix_get(t, 0, 0) == 1);
    assert(ds_matrix_get(t, 1, 0) == 2);
    assert(ds_matrix_get(t, 2, 1) == 6);

    ds_matrix_destroy(&m);
    ds_matrix_destroy(&t);
    puts("  [PASS] transpose");
}

void test_identity(void) {
    ds_matrix *m = ds_matrix_identity(3);
    assert(m != NULL);
    for (size_t i = 0; i < 3; i++) {
        for (size_t j = 0; j < 3; j++) {
            if (i == j)
                assert(ds_matrix_get(m, i, j) == 1.0);
            else
                assert(ds_matrix_get(m, i, j) == 0.0);
        }
    }
    ds_matrix_destroy(&m);
    puts("  [PASS] identity");
}

void test_det(void) {
    ds_matrix *m = ds_matrix_create(2, 2);
    ds_matrix_set(m, 0, 0, 1); ds_matrix_set(m, 0, 1, 2);
    ds_matrix_set(m, 1, 0, 3); ds_matrix_set(m, 1, 1, 4);
    assert(fabs(ds_matrix_det(m) - (-2.0)) < 1e-10);

    ds_matrix *m3 = ds_matrix_create(3, 3);
    ds_matrix_set(m3, 0, 0, 6); ds_matrix_set(m3, 0, 1, 1); ds_matrix_set(m3, 0, 2, 1);
    ds_matrix_set(m3, 1, 0, 4); ds_matrix_set(m3, 1, 1, -2); ds_matrix_set(m3, 1, 2, 5);
    ds_matrix_set(m3, 2, 0, 2); ds_matrix_set(m3, 2, 1, 8); ds_matrix_set(m3, 2, 2, 7);
    assert(fabs(ds_matrix_det(m3) - (-306.0)) < 1e-10);

    ds_matrix_destroy(&m);
    ds_matrix_destroy(&m3);
    puts("  [PASS] det");
}

void test_inverse(void) {
    ds_matrix *m = ds_matrix_create(2, 2);
    ds_matrix_set(m, 0, 0, 4); ds_matrix_set(m, 0, 1, 7);
    ds_matrix_set(m, 1, 0, 2); ds_matrix_set(m, 1, 1, 6);

    ds_matrix *inv = ds_matrix_create(2, 2);
    assert(ds_matrix_inverse(inv, m) == 0);

    assert(fabs(ds_matrix_get(inv, 0, 0) - 0.6) < 1e-10);
    assert(fabs(ds_matrix_get(inv, 0, 1) - (-0.7)) < 1e-10);
    assert(fabs(ds_matrix_get(inv, 1, 0) - (-0.2)) < 1e-10);
    assert(fabs(ds_matrix_get(inv, 1, 1) - 0.4) < 1e-10);

    ds_matrix *ident = ds_matrix_mul(m, inv);
    assert(ident != NULL);
    for (size_t i = 0; i < 2; i++) {
        for (size_t j = 0; j < 2; j++) {
            if (i == j)
                assert(fabs(ds_matrix_get(ident, i, j) - 1.0) < 1e-10);
            else
                assert(fabs(ds_matrix_get(ident, i, j)) < 1e-10);
        }
    }

    ds_matrix_destroy(&m);
    ds_matrix_destroy(&inv);
    ds_matrix_destroy(&ident);
    puts("  [PASS] inverse");
}

int main(void) {
    puts("ds_matrix tests:");
    test_create_destroy();
    test_get_set();
    test_add_sub();
    test_mul();
    test_transpose();
    test_identity();
    test_det();
    test_inverse();
    puts("All matrix tests passed!");
    return 0;
}
