#include "dictionary.h"

static void* _clib_dict_key_alloc(const char* src)
{
	size_t size = strlen(src);
	char* val = (char*)malloc(size + 1);

	if (!val) return NULL;

	strcpy(val, src);
	val[size] = '\0';

	return (void*)val;
}

static void _clib_dict_key_free(void* key)
{
	free(key);
}

int clib_dict_init(clib_dict* dict, size_t initial_size)
{
	int err = clib_hashmap_init(dict, clib_hashmap_hash_string, clib_hashmap_compare_string, initial_size);

	if (!err)
	{
		clib_hashmap_set_key_alloc_funcs(dict, _clib_dict_key_alloc, _clib_dict_key_free);
	}

	return err;
}

void clib_dict_destroy(clib_dict* dict)
{
	clib_hashmap_destroy(dict);
}

void* clib_dict_insert(clib_dict* dict, const char* key, void* value)
{
	return clib_hashmap_insert(dict, (const void*)key, value);
}

void* clib_dict_get(const clib_dict* dict, const char* key)
{
	return clib_hashmap_get(dict, (const void*)key);
}

void* clib_dict_remove(clib_dict* dict, const char* key)
{
	return clib_hashmap_remove(dict, (const void*)key);
}

void clib_dict_clear(clib_dict* dict)
{
	clib_hashmap_clear(dict);
}

void clib_dict_reset(clib_dict* dict)
{
	clib_hashmap_reset(dict);
}

clib_dict_iter* clib_dict_iterator(const clib_dict* dict)
{
	return clib_hashmap_iterator(dict);
}

clib_dict_iter* clib_dict_iter_next(const clib_dict* dict, const clib_dict_iter* iter)
{
	return clib_hashmap_iter_next(dict, iter);
}

clib_dict_iter* clib_dict_iter_remove(clib_dict* dict, const clib_dict_iter* iter)
{
	return clib_hashmap_iter_remove(dict, iter);
}

const char* clib_dict_iter_get_key(const clib_dict_iter* iter)
{
	return (const char*)clib_hashmap_iter_get_key(iter);
}

void* clib_dict_iter_get_value(const clib_dict_iter* iter)
{
	return clib_hashmap_iter_get_value(iter);
}

void clib_dict_iter_set_value(const clib_dict_iter* iter, void* value)
{
	clib_hashmap_iter_set_value(iter, value);
}

