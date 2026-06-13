#ifndef DS_MATRIX_H
#define DS_MATRIX_H

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct ds_matrix ds_matrix;

ds_matrix *ds_matrix_create(size_t rows, size_t cols);
void ds_matrix_destroy(ds_matrix **m);
double ds_matrix_get(const ds_matrix *m, size_t r, size_t c);
void ds_matrix_set(ds_matrix *m, size_t r, size_t c, double val);
ds_matrix *ds_matrix_mul(const ds_matrix *a, const ds_matrix *b);
ds_matrix *ds_matrix_add(const ds_matrix *a, const ds_matrix *b);
ds_matrix *ds_matrix_sub(const ds_matrix *a, const ds_matrix *b);
ds_matrix *ds_matrix_transpose(const ds_matrix *m);
ds_matrix *ds_matrix_identity(size_t n);
double ds_matrix_det(const ds_matrix *m);
int ds_matrix_inverse(ds_matrix *out, const ds_matrix *m);

#ifdef __cplusplus
}
#endif

#endif
