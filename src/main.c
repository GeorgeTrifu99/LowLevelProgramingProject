#include "kv.h"
#include <stdio.h>
#include <assert.h>

int main()
{
    kv_t *table = kv_init(1024);

    printf("%p\n", (void*)table);
    printf("%lld\n", table->capacity);
    
    // this is for debugging purposes
    // assert(table != NULL);
    // assert(table->capacity == 16);
    // assert(table->count == 0);

    kv_put(table, "hehe", "haha");
    kv_put(table, "hehe", "hoho");
    kv_put(table, "lala", "baba");

    char *val1 = kv_get(table, "hehe");
    char *val2 = kv_get(table, "lala");
    char *val3 = kv_get(table, "this does not exist");

    printf("%s %s %s\n", val1, val2, val3);

    kv_delete(table, "hehe");
    val1 = NULL;
    val1 = kv_get(table, "hehe");

    printf("%s %s %s\n", val1, val2, val3);

    kv_free(table);
    table = NULL;

    val1 = kv_get(table, "hehe");
    printf("%d\n", val1);


    
    // for(int i = 0; i < table->capacity; i++)
    // {
    //     if(table->entries[i].key && table->entries[i].key != (void*)TOMBSTONE)
    //     {
    //         printf("Idx %d: Key: %s, Value: %s\n", 
    //             i,
    //             table->entries[i].key, 
    //             table->entries[i].value);
    //     }
    // }

    return 0;
}