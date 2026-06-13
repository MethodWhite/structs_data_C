#ifndef DS_ARRAY_LIST_H
#define DS_ARRAY_LIST_H

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct ds_array_list ds_array_list;

ds_array_list *ds_array_list_create(size_t initial_capacity);
void ds_array_list_destroy(ds_array_list **list);
void ds_array_list_destroy_all(ds_array_list **list, void (*free_func)(void *));
void ds_array_list_clear(ds_array_list *list);

int ds_array_list_push(ds_array_list *list, void *value);
void ds_array_list_pop(ds_array_list *list);
void *ds_array_list_back(const ds_array_list *list);
void *ds_array_list_front(const ds_array_list *list);
void *ds_array_list_get(const ds_array_list *list, size_t index);
void ds_array_list_set(ds_array_list *list, size_t index, void *value);
int ds_array_list_insert(ds_array_list *list, size_t index, void *value);
void ds_array_list_erase(ds_array_list *list, size_t index);

int ds_array_list_reserve(ds_array_list *list, size_t capacity);
void ds_array_list_shrink(ds_array_list *list);

size_t ds_array_list_size(const ds_array_list *list);
size_t ds_array_list_capacity(const ds_array_list *list);
int ds_array_list_empty(const ds_array_list *list);
void **ds_array_list_data(const ds_array_list *list);

typedef void (*ds_array_list_visit_fn)(void *element, void *ctx);
void ds_array_list_foreach(const ds_array_list *list, ds_array_list_visit_fn func, void *ctx);

#ifdef __cplusplus
}
#endif

#endif
