#include "foreach.h"

#include "clib.h"

int clib_hashmap_foreach(const clib_hashmap* map, int (*func)(const void*, void*, void*), void* arg)
{
    CLIB_ASSERT(map != NULL);
    CLIB_ASSERT(func != NULL);

    for (clib_hashmap_entry* entry = map->table; entry < &map->table[map->capacity]; ++entry)
    {
        if (!entry->key)
            continue;

        size_t num_entries = map->size;
        const void* key = entry->key;
        int rc = func(entry->key, entry->value, arg);

        if (rc < 0)
            return rc;

        if (rc > 0)
            return 0;

        /* Run this entry again if func() deleted it */
        if (entry->key != key)
            --entry;
        else if (num_entries != map->size)
            return -1; /* Stop immediately if func put/removed another entry */
    }

    return 0;
}
