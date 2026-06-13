#include "ds_hash_table.h"
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define DS_HT_MIN_CAPACITY 16
#define DS_HT_LOAD_FACTOR 0.75
#define DS_HT_GROW_FACTOR 2

typedef struct ds_hash_entry {
    char *key;
    void *value;
    struct ds_hash_entry *next;
} ds_hash_entry;

struct ds_hash_table {
    size_t size;
    size_t capacity;
    ds_hash_entry **table;
};

static uint64_t ds_hash_djb2(const char *str) {
    uint64_t hash = 5381;
    int c;
    while ((c = (unsigned char)*str++)) {
        hash = ((hash << 5) + hash) + c;
    }
    return hash;
}

ds_hash_table *ds_hash_table_create(size_t initial_capacity) {
    if (initial_capacity < DS_HT_MIN_CAPACITY) {
        initial_capacity = DS_HT_MIN_CAPACITY;
    }
    ds_hash_table *table = calloc(1, sizeof(ds_hash_table));
    if (!table) return NULL;

    table->table = calloc(initial_capacity, sizeof(ds_hash_entry *));
    if (!table->table) {
        free(table);
        return NULL;
    }

    table->size = 0;
    table->capacity = initial_capacity;
    return table;
}

static void ds_hash_table_free_entries(ds_hash_table *table) {
    for (size_t i = 0; i < table->capacity; i++) {
        ds_hash_entry *entry = table->table[i];
        while (entry) {
            ds_hash_entry *next = entry->next;
            free(entry->key);
            free(entry);
            entry = next;
        }
        table->table[i] = NULL;
    }
}

void ds_hash_table_destroy(ds_hash_table **table) {
    if (!table || !*table) return;
    ds_hash_table *self = *table;
    ds_hash_table_free_entries(self);
    free(self->table);
    free(self);
    *table = NULL;
}

void ds_hash_table_destroy_all(ds_hash_table **table, void (*free_value)(void *)) {
    if (!table || !*table || !free_value) return;
    ds_hash_table *self = *table;
    for (size_t i = 0; i < self->capacity; i++) {
        ds_hash_entry *entry = self->table[i];
        while (entry) {
            ds_hash_entry *next = entry->next;
            free(entry->key);
            free_value(entry->value);
            free(entry);
            entry = next;
        }
        self->table[i] = NULL;
    }
    free(self->table);
    free(self);
    *table = NULL;
}

void ds_hash_table_clear(ds_hash_table *table) {
    if (!table) return;
    ds_hash_table_free_entries(table);
    table->size = 0;
}

static int ds_hash_table_resize(ds_hash_table *table, size_t new_capacity) {
    ds_hash_entry **new_table = calloc(new_capacity, sizeof(ds_hash_entry *));
    if (!new_table) return -1;

    for (size_t i = 0; i < table->capacity; i++) {
        ds_hash_entry *entry = table->table[i];
        while (entry) {
            ds_hash_entry *next = entry->next;
            size_t idx = ds_hash_djb2(entry->key) % new_capacity;
            entry->next = new_table[idx];
            new_table[idx] = entry;
            entry = next;
        }
    }

    free(table->table);
    table->table = new_table;
    table->capacity = new_capacity;
    return 0;
}

int ds_hash_table_put(ds_hash_table *table, const char *key, void *value) {
    if (!table || !key) return -1;

    size_t idx = ds_hash_djb2(key) % table->capacity;
    ds_hash_entry *entry = table->table[idx];

    while (entry) {
        if (strcmp(entry->key, key) == 0) {
            entry->value = value;
            return 0;
        }
        entry = entry->next;
    }

    if ((double)table->size >= table->capacity * DS_HT_LOAD_FACTOR) {
        if (ds_hash_table_resize(table, table->capacity * DS_HT_GROW_FACTOR) != 0) {
            return -1;
        }
        idx = ds_hash_djb2(key) % table->capacity;
    }

    ds_hash_entry *new_entry = malloc(sizeof(ds_hash_entry));
    if (!new_entry) return -1;

    new_entry->key = strdup(key);
    if (!new_entry->key) {
        free(new_entry);
        return -1;
    }
    new_entry->value = value;
    new_entry->next = table->table[idx];
    table->table[idx] = new_entry;
    table->size++;
    return 0;
}

void *ds_hash_table_get(const ds_hash_table *table, const char *key) {
    if (!table || !key) return NULL;

    size_t idx = ds_hash_djb2(key) % table->capacity;
    ds_hash_entry *entry = table->table[idx];

    while (entry) {
        if (strcmp(entry->key, key) == 0) {
            return entry->value;
        }
        entry = entry->next;
    }
    return NULL;
}

int ds_hash_table_contains(const ds_hash_table *table, const char *key) {
    if (!table || !key) return 0;
    size_t idx = ds_hash_djb2(key) % table->capacity;
    ds_hash_entry *entry = table->table[idx];
    while (entry) {
        if (strcmp(entry->key, key) == 0) return 1;
        entry = entry->next;
    }
    return 0;
}

int ds_hash_table_remove(ds_hash_table *table, const char *key) {
    if (!table || !key) return 0;

    size_t idx = ds_hash_djb2(key) % table->capacity;
    ds_hash_entry *entry = table->table[idx];
    ds_hash_entry *prev = NULL;

    while (entry) {
        if (strcmp(entry->key, key) == 0) {
            if (prev) {
                prev->next = entry->next;
            } else {
                table->table[idx] = entry->next;
            }
            free(entry->key);
            free(entry);
            table->size--;
            return 1;
        }
        prev = entry;
        entry = entry->next;
    }
    return 0;
}

size_t ds_hash_table_size(const ds_hash_table *table) {
    return table ? table->size : 0;
}

size_t ds_hash_table_capacity(const ds_hash_table *table) {
    return table ? table->capacity : 0;
}

int ds_hash_table_empty(const ds_hash_table *table) {
    return !table || table->size == 0;
}

void ds_hash_table_foreach(const ds_hash_table *table, ds_hash_table_visit_fn func, void *ctx) {
    if (!table || !func) return;
    for (size_t i = 0; i < table->capacity; i++) {
        ds_hash_entry *entry = table->table[i];
        while (entry) {
            func(entry->key, entry->value, ctx);
            entry = entry->next;
        }
    }
}
