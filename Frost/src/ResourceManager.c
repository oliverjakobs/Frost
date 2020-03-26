#include "ResourceManager.h"

struct _texkvp
{
	char key[32];
	IgnisTexture* data;
};

struct _fontkvp
{
	char key[32];
	IgnisFont* data;
};

/* Declare type-specific type_hashmap_* functions with this handy macro */
HASHMAP_FUNCS_CREATE(tex, const char, struct _texkvp)
HASHMAP_FUNCS_CREATE(font, const char, struct _fontkvp)

struct _texkvp* _tex_kvp(const char* key, IgnisTexture* data)
{
	if (strlen(key) > 32)
		return NULL;

	struct _texkvp* kvp = (struct _texkvp*)malloc(sizeof(struct _texkvp));

	if (kvp)
	{
		strcpy(kvp->key, key);
		kvp->data = data;
	}

	return kvp;
}

struct _fontkvp* _font_kvp(const char* key, IgnisFont* data)
{
	if (strlen(key) > 32)
		return NULL;

	struct _fontkvp* kvp = (struct _fontkvp*)malloc(sizeof(struct _fontkvp));

	if (kvp)
	{
		strcpy(kvp->key, key);
		kvp->data = data;
	}

	return kvp;
}

int ResourceManagerInit(ResourceManager* manager, const char* path)
{
	hashmap_init(&manager->textures, hashmap_hash_string, hashmap_compare_string, 0);
	hashmap_init(&manager->fonts, hashmap_hash_string, hashmap_compare_string, 0);

	return 1;
}

void ResourceManagerDestroy(ResourceManager* manager)
{
	for (struct hashmap_iter* iter = hashmap_iter(&manager->textures); iter; iter = hashmap_iter_next(&manager->textures, iter))
		ignisDestroyTexture(tex_hashmap_iter_get_data(iter)->data);

	hashmap_destroy(&manager->textures);

	for (struct hashmap_iter* iter = hashmap_iter(&manager->fonts); iter; iter = hashmap_iter_next(&manager->fonts, iter))
		ignisDeleteFont(font_hashmap_iter_get_data(iter)->data);

	hashmap_destroy(&manager->fonts);
}

void ResourceManagerAddTexture(ResourceManager* manager, const char* name, const char* path, int rows, int columns)
{
	IgnisTexture* texture = (IgnisTexture*)malloc(sizeof(IgnisTexture));

	if (ignisCreateTexture(texture, path, rows, columns, 1, NULL))
	{
		struct _texkvp* kvp = _tex_kvp(name, texture);

		if (kvp && tex_hashmap_put(&manager->textures, kvp->key, kvp) != kvp)
		{
			printf("[res] Texture \"%s\" already in use", name);
			ignisDestroyTexture(texture);
			free(kvp);
		}
	}
}

IgnisTexture* ResourceManagerGetTexture(ResourceManager* manager, const char* name)
{
	struct _texkvp* kvp = tex_hashmap_get(&manager->textures, name);

	if (kvp) return kvp->data;

	return NULL;
}

void ResourceManagerAddFont(ResourceManager* manager, const char* name, const char* path, float size)
{
	IgnisFont* font = (IgnisFont*)malloc(sizeof(IgnisFont));

	if (ignisLoadFont(font, path, size))
	{
		struct _fontkvp* kvp = _font_kvp(name, font);

		if (kvp && font_hashmap_put(&manager->fonts, kvp->key, kvp) != kvp)
		{
			printf("[res] Font \"%s\" already in use", name);
			ignisDeleteFont(font);
			free(kvp);
		}
	}
}

IgnisFont* ResourceManagerGetFont(ResourceManager* manager, const char* name)
{
	struct _fontkvp* kvp = font_hashmap_get(&manager->fonts, name);

	if (kvp) return kvp->data;

	return NULL;
}
