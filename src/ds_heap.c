#include "ds_heap.h"
#include <stdlib.h>

#define DS_HEAP_INIT_CAPACITY 16

struct ds_heap {
    void **data;
    size_t size;
    size_t capacity;
    int (*cmp)(const void *, const void *);
};

ds_heap *ds_heap_create(int (*cmp)(const void *, const void *)) {
    if (!cmp) return NULL;
    ds_heap *h = calloc(1, sizeof(ds_heap));
    if (!h) return NULL;
    h->data = malloc(DS_HEAP_INIT_CAPACITY * sizeof(void *));
    if (!h->data) { free(h); return NULL; }
    h->size = 0;
    h->capacity = DS_HEAP_INIT_CAPACITY;
    h->cmp = cmp;
    return h;
}

void ds_heap_destroy(ds_heap **h) {
    if (!h || !*h) return;
    ds_heap *self = *h;
    free(self->data);
    free(self);
    *h = NULL;
}

int ds_heap_push(ds_heap *h, void *value) {
    if (!h) return -1;
    if (h->size >= h->capacity) {
        size_t new_cap = h->capacity * 2;
        void **new_data = realloc(h->data, new_cap * sizeof(void *));
        if (!new_data) return -1;
        h->data = new_data;
        h->capacity = new_cap;
    }
    size_t i = h->size++;
    h->data[i] = value;
    while (i > 0) {
        size_t parent = (i - 1) / 2;
        if (h->cmp(h->data[parent], h->data[i]) >= 0) break;
        void *tmp = h->data[i];
        h->data[i] = h->data[parent];
        h->data[parent] = tmp;
        i = parent;
    }
    return 0;
}

void *ds_heap_pop(ds_heap *h) {
    if (!h || h->size == 0) return NULL;
    void *top = h->data[0];
    h->data[0] = h->data[--h->size];
    size_t i = 0;
    for (;;) {
        size_t largest = i;
        size_t left = 2 * i + 1;
        size_t right = 2 * i + 2;
        if (left < h->size && h->cmp(h->data[left], h->data[largest]) > 0) {
            largest = left;
        }
        if (right < h->size && h->cmp(h->data[right], h->data[largest]) > 0) {
            largest = right;
        }
        if (largest == i) break;
        void *tmp = h->data[i];
        h->data[i] = h->data[largest];
        h->data[largest] = tmp;
        i = largest;
    }
    return top;
}

void *ds_heap_peek(const ds_heap *h) {
    if (!h || h->size == 0) return NULL;
    return h->data[0];
}

size_t ds_heap_size(const ds_heap *h) {
    return h ? h->size : 0;
}

int ds_heap_empty(const ds_heap *h) {
    return !h || h->size == 0;
}
