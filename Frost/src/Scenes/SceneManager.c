#include "SceneManager.h"

#include "json/tb_json.h"
#include "json/tb_jwrite.h"

#include "Application/Debugger.h"

#include "SceneLoader.h"

#include "Animation/AnimationConditions.h"

int SceneManagerInit(SceneManager* manager, const char* reg, ResourceManager* resources, Camera* camera, float gridsize, uint16_t padding)
{
	manager->resources = resources;
	manager->camera = camera;

	memset(manager->scene_name, '\0', APPLICATION_STR_LEN);
	manager->width = 0.0f;
	manager->height = 0.0f;

	clib_strmap_alloc(&manager->scenes, 0);
	clib_strmap_alloc(&manager->templates, 0);

	SceneLoaderLoadRegister(manager, reg);

	AnimationConditionsInit();

	AnimationConditionsRegisterCondition("condition_jump", AnimationConditionJump);
	AnimationConditionsRegisterCondition("condition_fall", AnimationConditionFall);
	AnimationConditionsRegisterCondition("condition_walk", AnimationConditionWalk);
	AnimationConditionsRegisterCondition("condition_idle", AnimationConditionIdle);

	/* ecs */
	EcsInit(&manager->ecs, 4, 2, 8);
	EcsAddUpdateSystem(&manager->ecs, PhysicsSystem);
	EcsAddUpdateSystem(&manager->ecs, PlayerSystem);
	EcsAddUpdateSystem(&manager->ecs, AnimationSystem);
	EcsAddUpdateSystem(&manager->ecs, InteractionSystem);
	EcsAddRenderSystem(&manager->ecs, RenderSystem);
	EcsAddRenderSystem(&manager->ecs, DebugRenderSystem);

	EcsRegisterDataComponent(&manager->ecs, sizeof(Transform), NULL);
	EcsRegisterDataComponent(&manager->ecs, sizeof(RigidBody), NULL);
	EcsRegisterDataComponent(&manager->ecs, sizeof(Movement), NULL);
	EcsRegisterDataComponent(&manager->ecs, sizeof(Sprite), NULL);
	EcsRegisterDataComponent(&manager->ecs, sizeof(Animator), AnimatorFree);
	EcsRegisterDataComponent(&manager->ecs, sizeof(CameraController), NULL);
	EcsRegisterDataComponent(&manager->ecs, sizeof(Interaction), NULL);
	EcsRegisterDataComponent(&manager->ecs, sizeof(Interactor), NULL);

	EcsRegisterOrderComponent(&manager->ecs, sizeof(Template), TemplateCmp);
	EcsRegisterOrderComponent(&manager->ecs, sizeof(ZIndex), ZIndexCmp);

	return 1;
}

void SceneManagerDestroy(SceneManager* manager)
{
	if (manager->scene_name[0] != '\0')
		SceneManagerQuitScene(manager);

	AnimationConditionsDestroy();

	clib_strmap_free(&manager->scenes);
	clib_strmap_free(&manager->templates);

	EcsDestroy(&manager->ecs);
}

void SceneManagerChangeScene(SceneManager* manager, const char* name)
{
	if (strcmp(manager->scene_name, name) != 0)
	{
		/* Check if scene is in the register */
		char* path = clib_strmap_find(&manager->scenes, name);
		if (!path)
		{
			DEBUG_ERROR("[SceneManager] Couldn't find scene: %s\n", name);
			return;
		}

		// Exit old Scene
		if (manager->scene_name[0] != '\0')
		{
			SceneManagerQuitScene(manager);
		}

		// Enter new scene
		SceneLoaderLoadScene(manager, path);
		strcpy(manager->scene_name, name);
	}
}

int SceneManagerLoadScene(SceneManager* manager, float w, float h)
{
	if (!(manager && w > 0.0f && h > 0.0f)) return 0;

	manager->width = w;
	manager->height = h;

	return 1;
}

void SceneManagerQuitScene(SceneManager* manager)
{
	BackgroundClear(&manager->background);
	EcsClear(&manager->ecs);
}

void SceneManagerOnEvent(SceneManager* manager, Event e)
{

}

void SceneManagerOnUpdate(SceneManager* manager, float deltatime)
{
	BackgroundUpdate(&manager->background, manager->camera->position.x - manager->camera->size.x / 2.0f, deltatime);

	EcsUpdate(&manager->ecs, deltatime);
}

void SceneManagerOnRender(SceneManager* manager)
{
	BackgroundRender(&manager->background, CameraGetViewProjectionPtr(manager->camera));

	EcsRender(&manager->ecs, CameraGetViewProjectionPtr(manager->camera));
}

void SceneManagerOnRenderDebug(SceneManager* manager)
{

}
