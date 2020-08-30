#ifndef SCENE_REGISTER_H
#define SCENE_REGISTER_H

#include "clib/strmap.h"

#include "Scenes.h"

typedef struct
{
	clib_hashmap scenes;
	clib_hashmap templates;

	Resources* resources;
} SceneRegister;

int SceneRegisterInit(SceneRegister* reg, const char* path, Resources* resources);
void SceneRegisterDestroy(SceneRegister* reg);

void ScenesChangeActive(SceneRegister* reg, Scenes* scenes, const char* name);

int ScenesLoadScene(SceneRegister* reg, Scenes* scenes, const char* path);
int ScenesSaveScene(SceneRegister* reg, Scenes* scenes, const char* path);

int ScenesLoadTemplate(SceneRegister* reg, Scenes* scenes, const char* templ, EntityID entity, vec2 pos, int z_index);

#endif /* !SCENE_REGISTER_H */
