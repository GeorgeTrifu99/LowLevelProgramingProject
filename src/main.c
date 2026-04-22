#include "kv.h"
#include <stdio.h>
#include <assert.h>

int main()
{
    kv_t *db = kv_init(16);

    printf("%p\n", db);
    printf("%lld\n", db->capacity);
    
    assert(db != NULL);
    assert(db->capacity == 16);
    assert(db->count == 0);

    free(db);

    return 0;
}