#ifndef DS_HEAP_H
#define DS_HEAP_H

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct ds_heap ds_heap;

ds_heap *ds_heap_create(int (*cmp)(const void *, const void *));
void ds_heap_destroy(ds_heap **h);
int ds_heap_push(ds_heap *h, void *value);
void *ds_heap_pop(ds_heap *h);
void *ds_heap_peek(const ds_heap *h);
size_t ds_heap_size(const ds_heap *h);
int ds_heap_empty(const ds_heap *h);

#ifdef __cplusplus
}
#endif

#endif
