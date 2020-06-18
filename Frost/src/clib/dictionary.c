#include "dictionary.h"

#include "clib.h"
#include "hash.h"

static size_t _clib_dict_hash_key(const void* key)
{
    return clib_hash_string((const char*)key);
}

static int _clib_dict_cmp_key(const void* a, const void* b)
{
    return strcmp((const char*)a, (const char*)b);
}

static void* _clib_dict_key_alloc(const void* src)
{
	size_t size = strlen(src);
	char* val = (char*)malloc(size + 1);

	if (!val) return NULL;

	strcpy(val, src);
	val[size] = '\0'; /* make sure key is null-terminated */

	return (void*)val;
}

static void _clib_dict_key_free(void* key)
{
	free(key);
}

clib_hashmap_error clib_dict_alloc(clib_hashmap* dict, size_t initial_size)
{
	clib_hashmap_error err = clib_hashmap_alloc(dict, _clib_dict_hash_key, _clib_dict_cmp_key, initial_size);

	if (err == CLIB_HASHMAP_OK)
		clib_hashmap_set_key_alloc_funcs(dict, _clib_dict_key_alloc, _clib_dict_key_free);

	return err;
}

void clib_dict_free(clib_hashmap* dict)
{
	clib_hashmap_free(dict);
}

void* clib_dict_insert(clib_hashmap* dict, const char* key, void* value)
{
	return clib_hashmap_insert(dict, (const void*)key, value);
}

void* clib_dict_find(const clib_hashmap* dict, const char* key)
{
	return clib_hashmap_find(dict, (const void*)key);
}

const char* clib_dict_get_key_ptr(const clib_hashmap* dict, const char* key)
{
	CLIB_ASSERT(dict != NULL);
	CLIB_ASSERT(key != NULL);

	clib_hashmap_entry* entry = clib_hashmap_entry_find(dict, key, 0);

	if (!entry) return NULL;

	return (const char*)entry->key;
}

clib_hashmap_error clib_dict_remove(clib_hashmap* dict, const char* key)
{
	return clib_hashmap_remove(dict, (const void*)key);
}

void clib_dict_clear(clib_hashmap* dict)
{
	clib_hashmap_clear(dict);
}

clib_hashmap_iter* clib_dict_iterator(const clib_hashmap* dict)
{
	return clib_hashmap_iterator(dict);
}

clib_hashmap_iter* clib_dict_iter_next(const clib_hashmap* dict, const clib_hashmap_iter* iter)
{
	return clib_hashmap_iter_next(dict, iter);
}

clib_hashmap_iter* clib_dict_iter_remove(clib_hashmap* dict, const clib_hashmap_iter* iter)
{
	return clib_hashmap_iter_remove(dict, iter);
}

const char* clib_dict_iter_get_key(const clib_hashmap_iter* iter)
{
	return (const char*)clib_hashmap_iter_get_key(iter);
}

void* clib_dict_iter_get_value(const clib_hashmap_iter* iter)
{
	return clib_hashmap_iter_get_value(iter);
}

