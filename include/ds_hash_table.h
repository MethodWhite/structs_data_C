#ifndef DS_HASH_TABLE_H
#define DS_HASH_TABLE_H

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct ds_hash_table ds_hash_table;

ds_hash_table *ds_hash_table_create(size_t initial_capacity);
void ds_hash_table_destroy(ds_hash_table **table);
void ds_hash_table_destroy_all(ds_hash_table **table, void (*free_value)(void *));
void ds_hash_table_clear(ds_hash_table *table);
int ds_hash_table_contains(const ds_hash_table *table, const char *key);

int ds_hash_table_put(ds_hash_table *table, const char *key, void *value);
void *ds_hash_table_get(const ds_hash_table *table, const char *key);
int ds_hash_table_remove(ds_hash_table *table, const char *key);

size_t ds_hash_table_size(const ds_hash_table *table);
size_t ds_hash_table_capacity(const ds_hash_table *table);
int ds_hash_table_empty(const ds_hash_table *table);

typedef void (*ds_hash_table_visit_fn)(const char *key, void *value, void *ctx);
void ds_hash_table_foreach(const ds_hash_table *table, ds_hash_table_visit_fn func, void *ctx);

#ifdef __cplusplus
}
#endif

#endif
