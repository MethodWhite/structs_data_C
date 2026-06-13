#include "ds_matrix.h"
#include <stdlib.h>
#include <string.h>
#include <math.h>

struct ds_matrix {
    size_t rows;
    size_t cols;
    double *data;
};

ds_matrix *ds_matrix_create(size_t rows, size_t cols) {
    if (rows == 0 || cols == 0) return NULL;
    ds_matrix *m = calloc(1, sizeof(ds_matrix));
    if (!m) return NULL;
    m->data = calloc(rows * cols, sizeof(double));
    if (!m->data) { free(m); return NULL; }
    m->rows = rows;
    m->cols = cols;
    return m;
}

void ds_matrix_destroy(ds_matrix **m) {
    if (!m || !*m) return;
    ds_matrix *self = *m;
    free(self->data);
    free(self);
    *m = NULL;
}

double ds_matrix_get(const ds_matrix *m, size_t r, size_t c) {
    if (!m || r >= m->rows || c >= m->cols) return 0.0;
    return m->data[r * m->cols + c];
}

void ds_matrix_set(ds_matrix *m, size_t r, size_t c, double val) {
    if (!m || r >= m->rows || c >= m->cols) return;
    m->data[r * m->cols + c] = val;
}

ds_matrix *ds_matrix_mul(const ds_matrix *a, const ds_matrix *b) {
    if (!a || !b || a->cols != b->rows) return NULL;
    ds_matrix *c = ds_matrix_create(a->rows, b->cols);
    if (!c) return NULL;
    for (size_t i = 0; i < a->rows; i++) {
        for (size_t j = 0; j < b->cols; j++) {
            double sum = 0.0;
            for (size_t k = 0; k < a->cols; k++) {
                sum += a->data[i * a->cols + k] * b->data[k * b->cols + j];
            }
            c->data[i * c->cols + j] = sum;
        }
    }
    return c;
}

ds_matrix *ds_matrix_add(const ds_matrix *a, const ds_matrix *b) {
    if (!a || !b || a->rows != b->rows || a->cols != b->cols) return NULL;
    ds_matrix *c = ds_matrix_create(a->rows, a->cols);
    if (!c) return NULL;
    size_t n = a->rows * a->cols;
    for (size_t i = 0; i < n; i++) {
        c->data[i] = a->data[i] + b->data[i];
    }
    return c;
}

ds_matrix *ds_matrix_sub(const ds_matrix *a, const ds_matrix *b) {
    if (!a || !b || a->rows != b->rows || a->cols != b->cols) return NULL;
    ds_matrix *c = ds_matrix_create(a->rows, a->cols);
    if (!c) return NULL;
    size_t n = a->rows * a->cols;
    for (size_t i = 0; i < n; i++) {
        c->data[i] = a->data[i] - b->data[i];
    }
    return c;
}

ds_matrix *ds_matrix_transpose(const ds_matrix *m) {
    if (!m) return NULL;
    ds_matrix *t = ds_matrix_create(m->cols, m->rows);
    if (!t) return NULL;
    for (size_t i = 0; i < m->rows; i++) {
        for (size_t j = 0; j < m->cols; j++) {
            t->data[j * t->cols + i] = m->data[i * m->cols + j];
        }
    }
    return t;
}

ds_matrix *ds_matrix_identity(size_t n) {
    if (n == 0) return NULL;
    ds_matrix *m = ds_matrix_create(n, n);
    if (!m) return NULL;
    for (size_t i = 0; i < n; i++) {
        m->data[i * n + i] = 1.0;
    }
    return m;
}

double ds_matrix_det(const ds_matrix *m) {
    if (!m || m->rows != m->cols) return 0.0;
    size_t n = m->rows;

    if (n == 1) return m->data[0];
    if (n == 2) return m->data[0] * m->data[3] - m->data[1] * m->data[2];

    double *mat = malloc(n * n * sizeof(double));
    if (!mat) return 0.0;
    memcpy(mat, m->data, n * n * sizeof(double));

    int sign = 1;
    for (size_t col = 0; col < n; col++) {
        size_t pivot = col;
        for (size_t row = col + 1; row < n; row++) {
            if (fabs(mat[row * n + col]) > fabs(mat[pivot * n + col])) {
                pivot = row;
            }
        }
        if (fabs(mat[pivot * n + col]) < 1e-15) {
            free(mat);
            return 0.0;
        }
        if (pivot != col) {
            for (size_t j = col; j < n; j++) {
                double tmp = mat[col * n + j];
                mat[col * n + j] = mat[pivot * n + j];
                mat[pivot * n + j] = tmp;
            }
            sign = -sign;
        }
        for (size_t row = col + 1; row < n; row++) {
            double factor = mat[row * n + col] / mat[col * n + col];
            for (size_t j = col + 1; j < n; j++) {
                mat[row * n + j] -= factor * mat[col * n + j];
            }
        }
    }

    double det = (double)sign;
    for (size_t i = 0; i < n; i++) {
        det *= mat[i * n + i];
    }
    free(mat);
    return det;
}

int ds_matrix_inverse(ds_matrix *out, const ds_matrix *m) {
    if (!out || !m || m->rows != m->cols || out->rows != m->rows || out->cols != m->cols) return -1;
    size_t n = m->rows;

    double *a = malloc(n * n * sizeof(double));
    if (!a) return -1;
    memcpy(a, m->data, n * n * sizeof(double));

    double *inv = calloc(n * n, sizeof(double));
    if (!inv) { free(a); return -1; }
    for (size_t i = 0; i < n; i++) {
        inv[i * n + i] = 1.0;
    }

    for (size_t col = 0; col < n; col++) {
        size_t pivot = col;
        for (size_t row = col + 1; row < n; row++) {
            if (fabs(a[row * n + col]) > fabs(a[pivot * n + col])) {
                pivot = row;
            }
        }
        if (fabs(a[pivot * n + col]) < 1e-15) {
            free(a);
            free(inv);
            return -1;
        }
        if (pivot != col) {
            for (size_t j = 0; j < n; j++) {
                double tmp = a[col * n + j];
                a[col * n + j] = a[pivot * n + j];
                a[pivot * n + j] = tmp;
                tmp = inv[col * n + j];
                inv[col * n + j] = inv[pivot * n + j];
                inv[pivot * n + j] = tmp;
            }
        }
        double pivot_val = a[col * n + col];
        for (size_t j = 0; j < n; j++) {
            a[col * n + j] /= pivot_val;
            inv[col * n + j] /= pivot_val;
        }
        for (size_t row = 0; row < n; row++) {
            if (row == col) continue;
            double factor = a[row * n + col];
            for (size_t j = 0; j < n; j++) {
                a[row * n + j] -= factor * a[col * n + j];
                inv[row * n + j] -= factor * inv[col * n + j];
            }
        }
    }

    memcpy(out->data, inv, n * n * sizeof(double));
    free(a);
    free(inv);
    return 0;
}
