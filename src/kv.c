#include "kv.h"
#include <string.h>
#include <stdlib.h>

kv_t *kv_init(size_t capacity)
{
    if(capacity == 0) return NULL;

    kv_t *table = malloc(sizeof(kv_t));
    if(table == NULL) return NULL;

    table->capacity = capacity;
    table->count = 0;

    table->entries = calloc(sizeof(kv_entry_t), capacity);
    if(table->entries == NULL) return NULL;

    return table;
}

size_t hash(char *val, int capacity)
{
    size_t hash = 0x13371337deadbeef;

    while(*val)
    {
        hash ^= *val;
        hash = hash << 8;
        hash += *val;
        val++;
    }

    return hash % capacity;
}

// fn returns 0 on success, -1 on error, -2 if db is full
int kv_put(kv_t *db, char *key, char *value)
{
    if(!db || !key || !value) return -1;

    size_t idx = hash(key, db->capacity);

    for(int i = 0; i < db->capacity-1; i++)
    {
        size_t real_idx = (idx + i) % db->capacity;
        kv_entry_t *entry = &db->entries[real_idx];

        // found the slot occupied by the same key, update the value
        if(entry->key && entry->key != (void*)TOMBSTONE && !strcmp(entry->key, key))
        {
            char *new_value = strdup(value); // duplicate the value to avoid dangling pointer issues
            if(!new_value) return -1; // memory allocation failed
            free(entry->value);
            entry->value = new_value;
            return real_idx;
        }

        //land in a slot that is empty, null or tombstone
        if(entry->key == NULL || entry->key == (void*)TOMBSTONE)
        {
            char *new_value = strdup(value); // duplicate the value to avoid dangling pointer issues
            char *new_key = strdup(key);
            if(new_value == NULL || new_key == NULL) 
            {
                free(new_value);
                free(new_key);
                return -1; // memory allocation failed
            }
            entry->value = new_value;
            entry->key = new_key;
            db->count++;
            return real_idx;
        }
    }

    return -2; // db is full
}

// fn returns the pointer to the key, and null if not found
char* kv_get(kv_t* db,  char* key)
{
    if(!db || !key) return NULL;

    size_t idx = hash(key, db->capacity);

    for(int i = 0; i < db->capacity-1; i++)
    {
        size_t real_idx = (idx + i) % db->capacity;
        kv_entry_t *entry = &db->entries[real_idx];
        if(entry->key == NULL) return NULL; // key not found

        // find and entry and the keys match
        if(entry->key && entry->key != (void*)TOMBSTONE && strcmp(entry->key, key) == 0)
        {
            return entry->value;
        }
    }
    return NULL; // key not found
}


// fn returns the index of the deleted key, -1 if key not found
int kv_delete(kv_t *db, char *key)
{
    if(!db || !key) return -1;

    size_t idx = hash(key, db->capacity);

    for(int i = 0; i < db->capacity-1; i++)
    {
        size_t real_idx = (idx + i) % db->capacity;
        kv_entry_t *entry = &db->entries[real_idx];
        if(entry->key == NULL) return -1; // key not found

        // find and entry and the keys match
        if(entry->key && entry->key != (void*)TOMBSTONE && strcmp(entry->key, key) == 0)
        {
            free(entry->key);
            free(entry->value);
            db->count--;
            entry->key = (void*)TOMBSTONE; // mark as deleted
            entry->value = NULL;
           
            return real_idx;
        }
    }
    return -1; // key not found
}