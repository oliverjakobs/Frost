#include "Resources.h"

#include "toolbox/tb_str.h"
#include "toolbox/tb_mem.h"

#include "Application/Logger.h"

static int ResourcesAllocEntry(void* allocator, tb_hashmap_entry* entry, void* key, void* value)
{
	entry->key = tb_mem_dup(allocator, key, strlen(key) + 1);

	if (!entry->key) return 0;

	entry->value = tb_mem_dup(allocator, value, sizeof(IgnisTexture2D));

	return entry->value != NULL;
}

static void ResourcesFreeEntry(void* allocator, tb_hashmap_entry* entry)
{
	tb_mem_free(allocator, entry->key);
	ignisDeleteTexture2D(entry->value);
	tb_mem_free(allocator, entry->value);
}

int ResourcesInit(Resources* res, void* allocator)
{
	res->textures.allocator = allocator;
	res->textures.alloc = tb_mem_calloc;
	res->textures.free = tb_mem_free;
	res->textures.entry_alloc = ResourcesAllocEntry;
	res->textures.entry_free = ResourcesFreeEntry;

    return tb_hashmap_init(&res->textures, tb_hash_string, strcmp, 0) == TB_HASHMAP_OK;
}

void ResourcesDestroy(Resources* res)
{
    tb_hashmap_destroy(&res->textures);
}

void ResourcesClear(Resources* res)
{
	tb_hashmap_clear(&res->textures);
}

IgnisTexture2D* ResourcesAddTexture2D(Resources* res, const char* name, const char* path, int rows, int cols)
{
	IgnisTexture2D texture;
	if (ignisCreateTexture2D(&texture, path, rows, cols, 1, NULL))
	{
		IgnisTexture2D* entry = tb_hashmap_insert(&res->textures, name, &texture);
		if (entry != NULL) return entry;

		DEBUG_ERROR("[Scenes] Failed to add texture: %s (%s)", name, path);
		ignisDeleteTexture2D(&texture);
	}

	return NULL;
}

IgnisTexture2D* ResourcesGetTexture2D(const Resources* res, const char* name)
{
	IgnisTexture2D* tex = tb_hashmap_find(&res->textures, name);

	if (!tex) DEBUG_WARN("[Scenes] Could not find texture: %s", name);

	return tex;
}

const char* ResourcesGetTexture2DName(const Resources* res, const IgnisTexture2D* texture)
{
	for (tb_hashmap_iter* iter = tb_hashmap_iterator(&res->textures); iter; iter = tb_hashmap_iter_next(&res->textures, iter))
	{
		if (texture == tb_hashmap_iter_get_value(iter))
			return tb_hashmap_iter_get_key(iter);
	}

	return NULL;
}
