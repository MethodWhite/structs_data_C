#include "ds_ring_buffer.h"
#include <stdlib.h>

struct ds_ring_buffer {
    void **data;
    size_t capacity;
    size_t head;
    size_t tail;
    size_t count;
};

ds_ring_buffer *ds_ring_buffer_create(size_t capacity) {
    if (capacity == 0) return NULL;
    ds_ring_buffer *rb = calloc(1, sizeof(ds_ring_buffer));
    if (!rb) return NULL;
    rb->data = calloc(capacity, sizeof(void *));
    if (!rb->data) { free(rb); return NULL; }
    rb->capacity = capacity;
    rb->head = 0;
    rb->tail = 0;
    rb->count = 0;
    return rb;
}

void ds_ring_buffer_destroy(ds_ring_buffer **rb) {
    if (!rb || !*rb) return;
    ds_ring_buffer *self = *rb;
    free(self->data);
    free(self);
    *rb = NULL;
}

int ds_ring_buffer_push(ds_ring_buffer *rb, void *value) {
    if (!rb) return -1;
    if (rb->count >= rb->capacity) return -1;
    rb->data[rb->head] = value;
    rb->head = (rb->head + 1) % rb->capacity;
    rb->count++;
    return 0;
}

void *ds_ring_buffer_pop(ds_ring_buffer *rb) {
    if (!rb || rb->count == 0) return NULL;
    void *value = rb->data[rb->tail];
    rb->tail = (rb->tail + 1) % rb->capacity;
    rb->count--;
    return value;
}

void *ds_ring_buffer_peek(const ds_ring_buffer *rb, size_t index) {
    if (!rb || index >= rb->count) return NULL;
    size_t pos = (rb->tail + index) % rb->capacity;
    return rb->data[pos];
}

size_t ds_ring_buffer_size(const ds_ring_buffer *rb) {
    return rb ? rb->count : 0;
}

size_t ds_ring_buffer_capacity(const ds_ring_buffer *rb) {
    return rb ? rb->capacity : 0;
}

int ds_ring_buffer_full(const ds_ring_buffer *rb) {
    return rb ? (rb->count >= rb->capacity) : 0;
}

int ds_ring_buffer_empty(const ds_ring_buffer *rb) {
    return !rb || rb->count == 0;
}
