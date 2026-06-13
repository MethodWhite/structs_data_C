#ifndef DS_TENSOR_H
#define DS_TENSOR_H

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct ds_tensor ds_tensor;

ds_tensor *ds_tensor_create(size_t ndim, const size_t *dims);
void ds_tensor_destroy(ds_tensor **t);
void *ds_tensor_get(const ds_tensor *t, const size_t *indices);
void ds_tensor_set(ds_tensor *t, const size_t *indices, void *value);
int ds_tensor_reshape(ds_tensor *t, size_t new_ndim, const size_t *new_dims);
ds_tensor *ds_tensor_slice(const ds_tensor *t, size_t dim, size_t start, size_t end);
size_t ds_tensor_size(const ds_tensor *t);
size_t ds_tensor_ndim(const ds_tensor *t);
const size_t *ds_tensor_shape(const ds_tensor *t);

#ifdef __cplusplus
}
#endif

#endif
