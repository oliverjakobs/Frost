#include "Resources.h"

#include "toolbox/tb_str.h"

#include "Application/Logger.h"

int ResourcesInit(Resources* res)
{
    if (tb_hashmap_alloc(&res->textures, tb_hash_string, strcmp, 0) != TB_HASHMAP_OK)
        return 0;

    tb_hashmap_set_key_alloc_funcs(&res->textures, tb_strdup, free);

    return 1;
}

void ResourcesDestroy(Resources* res)
{
    tb_hashmap_free(&res->textures);
}

void ResourcesClear(Resources* res)
{
	for (tb_hashmap_iter* iter = tb_hashmap_iterator(&res->textures); iter; iter = tb_hashmap_iter_next(&res->textures, iter))
	{
		IgnisTexture2D* tex = tb_hashmap_iter_get_value(iter);
		ignisDeleteTexture2D(tex);
	}
	tb_hashmap_clear(&res->textures);
}

IgnisTexture2D* ResourcesAddTexture2D(Resources* res, const char* name, const char* path, int rows, int cols)
{
	IgnisTexture2D* texture = malloc(sizeof(IgnisTexture2D));
	if (ignisCreateTexture2D(texture, path, rows, cols, 1, NULL))
	{
		if (tb_hashmap_insert(&res->textures, name, texture) == texture)
			return texture;

		DEBUG_ERROR("[Scenes] Failed to add texture: %s (%s)", name, path);
		ignisDeleteTexture2D(texture);
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
