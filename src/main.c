#include "kv.h"
#include <stdio.h>
#include <assert.h>

int main()
{
    kv_t *table = kv_init(1024);

    printf("%p\n", table);
    printf("%lld\n", table->capacity);
    
    // this is for debugging purposes
    // assert(table != NULL);
    // assert(table->capacity == 16);
    // assert(table->count == 0);

    kv_put(table, "hehe", "haha");
    kv_put(table, "hehe", "hoho");
    kv_put(table, "lala", "haha");

    
    for(int i = 0; i < table->capacity; i++)
    {
        if(table->entries[i].key)
        {
            printf("Idx %d: Key: %s, Value: %s\n", 
                i,
                table->entries[i].key, 
                table->entries[i].value);
        }
    }

    return 0;
}