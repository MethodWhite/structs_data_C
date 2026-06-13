#include "ds_tensor.h"
#include <stdlib.h>
#include <string.h>

struct ds_tensor {
    size_t ndim;
    size_t size;
    size_t *dims;
    size_t *strides;
    void **data;
};

ds_tensor *ds_tensor_create(size_t ndim, const size_t *dims) {
    if (ndim == 0 || !dims) return NULL;

    ds_tensor *t = calloc(1, sizeof(ds_tensor));
    if (!t) return NULL;

    t->dims = malloc(ndim * sizeof(size_t));
    if (!t->dims) { free(t); return NULL; }

    t->strides = malloc(ndim * sizeof(size_t));
    if (!t->strides) { free(t->dims); free(t); return NULL; }

    size_t total = 1;
    for (size_t i = 0; i < ndim; i++) {
        t->dims[i] = dims[i];
        total *= dims[i];
    }

    t->strides[ndim - 1] = 1;
    for (size_t i = ndim - 1; i > 0; i--) {
        t->strides[i - 1] = t->strides[i] * t->dims[i];
    }

    t->data = calloc(total, sizeof(void *));
    if (!t->data) { free(t->strides); free(t->dims); free(t); return NULL; }

    t->ndim = ndim;
    t->size = total;
    return t;
}

void ds_tensor_destroy(ds_tensor **t) {
    if (!t || !*t) return;
    ds_tensor *self = *t;
    free(self->data);
    free(self->dims);
    free(self->strides);
    free(self);
    *t = NULL;
}

void *ds_tensor_get(const ds_tensor *t, const size_t *indices) {
    if (!t || !indices) return NULL;
    size_t idx = 0;
    for (size_t i = 0; i < t->ndim; i++) {
        if (indices[i] >= t->dims[i]) return NULL;
        idx += indices[i] * t->strides[i];
    }
    return t->data[idx];
}

void ds_tensor_set(ds_tensor *t, const size_t *indices, void *value) {
    if (!t || !indices) return;
    size_t idx = 0;
    for (size_t i = 0; i < t->ndim; i++) {
        if (indices[i] >= t->dims[i]) return;
        idx += indices[i] * t->strides[i];
    }
    t->data[idx] = value;
}

int ds_tensor_reshape(ds_tensor *t, size_t new_ndim, const size_t *new_dims) {
    if (!t || new_ndim == 0 || !new_dims) return -1;

    size_t total = 1;
    for (size_t i = 0; i < new_ndim; i++) {
        total *= new_dims[i];
    }
    if (total != t->size) return -1;

    size_t *new_dims_buf = realloc(t->dims, new_ndim * sizeof(size_t));
    if (!new_dims_buf) return -1;
    t->dims = new_dims_buf;

    size_t *new_strides = realloc(t->strides, new_ndim * sizeof(size_t));
    if (!new_strides) return -1;
    t->strides = new_strides;

    for (size_t i = 0; i < new_ndim; i++) {
        t->dims[i] = new_dims[i];
    }

    t->strides[new_ndim - 1] = 1;
    for (size_t i = new_ndim - 1; i > 0; i--) {
        t->strides[i - 1] = t->strides[i] * t->dims[i];
    }

    t->ndim = new_ndim;
    return 0;
}

ds_tensor *ds_tensor_slice(const ds_tensor *t, size_t dim, size_t start, size_t end) {
    if (!t || dim >= t->ndim || start > end || end > t->dims[dim]) return NULL;

    size_t new_ndim = t->ndim;
    size_t *new_dims = malloc(new_ndim * sizeof(size_t));
    if (!new_dims) return NULL;

    for (size_t i = 0; i < new_ndim; i++) {
        new_dims[i] = t->dims[i];
    }
    new_dims[dim] = end - start;

    ds_tensor *slice = ds_tensor_create(new_ndim, new_dims);
    free(new_dims);
    if (!slice) return NULL;

    size_t *indices = calloc(new_ndim, sizeof(size_t));
    if (!indices) { ds_tensor_destroy(&slice); return NULL; }

    for (size_t k = 0; k < slice->size; k++) {
        size_t flat = 0;
        for (size_t i = 0; i < new_ndim; i++) {
            size_t actual = (i == dim) ? start + indices[i] : indices[i];
            flat += actual * t->strides[i];
        }
        slice->data[k] = t->data[flat];

        for (size_t i = new_ndim; i > 0; i--) {
            indices[i - 1]++;
            if (indices[i - 1] < slice->dims[i - 1]) break;
            indices[i - 1] = 0;
        }
    }

    free(indices);
    return slice;
}

size_t ds_tensor_size(const ds_tensor *t) {
    return t ? t->size : 0;
}

size_t ds_tensor_ndim(const ds_tensor *t) {
    return t ? t->ndim : 0;
}

const size_t *ds_tensor_shape(const ds_tensor *t) {
    return t ? t->dims : NULL;
}
