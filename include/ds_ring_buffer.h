#ifndef DS_RING_BUFFER_H
#define DS_RING_BUFFER_H

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct ds_ring_buffer ds_ring_buffer;

ds_ring_buffer *ds_ring_buffer_create(size_t capacity);
void ds_ring_buffer_destroy(ds_ring_buffer **rb);
int ds_ring_buffer_push(ds_ring_buffer *rb, void *value);
void *ds_ring_buffer_pop(ds_ring_buffer *rb);
void *ds_ring_buffer_peek(const ds_ring_buffer *rb, size_t index);
size_t ds_ring_buffer_size(const ds_ring_buffer *rb);
size_t ds_ring_buffer_capacity(const ds_ring_buffer *rb);
int ds_ring_buffer_full(const ds_ring_buffer *rb);
int ds_ring_buffer_empty(const ds_ring_buffer *rb);

#ifdef __cplusplus
}
#endif

#endif
