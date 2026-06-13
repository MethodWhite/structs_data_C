#include "ds_array_list.h"
#include <stdlib.h>
#include <string.h>

#define DS_GROWTH_FACTOR 2
#define DS_MIN_CAPACITY 8

struct ds_array_list {
    size_t size;
    size_t capacity;
    void **data;
};

ds_array_list *ds_array_list_create(size_t initial_capacity) {
    ds_array_list *list = calloc(1, sizeof(ds_array_list));
    if (!list) return NULL;

    if (initial_capacity < DS_MIN_CAPACITY) {
        initial_capacity = DS_MIN_CAPACITY;
    }

    list->data = malloc(initial_capacity * sizeof(void *));
    if (!list->data) {
        free(list);
        return NULL;
    }

    list->size = 0;
    list->capacity = initial_capacity;
    return list;
}

void ds_array_list_destroy(ds_array_list **list) {
    if (!list || !*list) return;
    ds_array_list *self = *list;
    free(self->data);
    self->data = NULL;
    self->size = self->capacity = 0;
    free(self);
    *list = NULL;
}

void ds_array_list_destroy_all(ds_array_list **list, void (*free_func)(void *)) {
    if (!list || !*list || !free_func) return;
    ds_array_list *self = *list;
    for (size_t i = 0; i < self->size; i++) {
        free_func(self->data[i]);
    }
    ds_array_list_destroy(list);
}

void ds_array_list_clear(ds_array_list *list) {
    if (!list) return;
    for (size_t i = 0; i < list->size; i++) {
        list->data[i] = NULL;
    }
    list->size = 0;
}

int ds_array_list_grow(ds_array_list *list, size_t min_capacity) {
    size_t new_cap = list->capacity ? list->capacity : DS_MIN_CAPACITY;
    while (new_cap < min_capacity) {
        new_cap *= DS_GROWTH_FACTOR;
    }
    void **new_data = realloc(list->data, new_cap * sizeof(void *));
    if (!new_data) return -1;
    list->data = new_data;
    list->capacity = new_cap;
    return 0;
}

int ds_array_list_push(ds_array_list *list, void *value) {
    if (!list) return -1;
    if (list->size >= list->capacity) {
        if (ds_array_list_grow(list, list->capacity + 1) != 0) return -1;
    }
    list->data[list->size++] = value;
    return 0;
}

void ds_array_list_pop(ds_array_list *list) {
    if (!list || list->size == 0) return;
    list->data[--list->size] = NULL;
}

void *ds_array_list_back(const ds_array_list *list) {
    if (!list || list->size == 0) return NULL;
    return list->data[list->size - 1];
}

void *ds_array_list_front(const ds_array_list *list) {
    if (!list || list->size == 0) return NULL;
    return list->data[0];
}

void *ds_array_list_get(const ds_array_list *list, size_t index) {
    if (!list || index >= list->size) return NULL;
    return list->data[index];
}

void ds_array_list_set(ds_array_list *list, size_t index, void *value) {
    if (!list || index >= list->size) return;
    list->data[index] = value;
}

int ds_array_list_insert(ds_array_list *list, size_t index, void *value) {
    if (!list || index > list->size) return -1;
    if (list->size >= list->capacity) {
        if (ds_array_list_grow(list, list->capacity + 1) != 0) return -1;
    }
    if (index < list->size) {
        memmove(&list->data[index + 1], &list->data[index],
                (list->size - index) * sizeof(void *));
    }
    list->data[index] = value;
    list->size++;
    return 0;
}

void ds_array_list_erase(ds_array_list *list, size_t index) {
    if (!list || index >= list->size) return;
    if (index < list->size - 1) {
        memmove(&list->data[index], &list->data[index + 1],
                (list->size - index - 1) * sizeof(void *));
    }
    list->size--;
}

int ds_array_list_reserve(ds_array_list *list, size_t capacity) {
    if (!list || capacity <= list->capacity) return 0;
    return ds_array_list_grow(list, capacity);
}

void ds_array_list_shrink(ds_array_list *list) {
    if (!list || list->size == list->capacity || list->size == 0) return;
    void **new_data = realloc(list->data, list->size * sizeof(void *));
    if (!new_data) return;
    list->data = new_data;
    list->capacity = list->size;
}

size_t ds_array_list_size(const ds_array_list *list) {
    return list ? list->size : 0;
}

size_t ds_array_list_capacity(const ds_array_list *list) {
    return list ? list->capacity : 0;
}

int ds_array_list_empty(const ds_array_list *list) {
    return !list || list->size == 0;
}

void **ds_array_list_data(const ds_array_list *list) {
    return list ? list->data : NULL;
}

void ds_array_list_foreach(const ds_array_list *list, ds_array_list_visit_fn func, void *ctx) {
    if (!list || !func) return;
    for (size_t i = 0; i < list->size; i++) {
        func(list->data[i], ctx);
    }
}
