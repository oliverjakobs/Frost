#include "Scene.h"

#include "ECS/Systems.h"

CLIB_VECTOR_DEFINE_FUNCS(layer, EcsEntity)

int SceneLoad(Scene* scene, Camera* camera, float w, float h, size_t max_layer)
{
	scene->layers = (clib_vector*)malloc(sizeof(clib_vector) * max_layer);
	if (!scene->layers)
		return 0;

	scene->max_layer = max_layer;

	for (size_t i = 0; i < max_layer; i++)
		clib_vector_init(&scene->layers[i], SCENE_INITIAL_LAYER_SIZE);

	scene->camera = camera;
	scene->width = w;
	scene->height = h;

	scene->world = (World*)malloc(sizeof(World));
	WorldLoad(scene->world, (vec2){ 0.0f, -980.0f });

	EcsInit(&scene->ecs, SCENE_INITIAL_ECS_SIZE);
	EcsAddUpdateSystem(&scene->ecs, EcsSystemPlayer);
	EcsAddUpdateSystem(&scene->ecs, EcsSystemAnimation);
	EcsAddRenderSystem(&scene->ecs, EcsSystemRender);
	
	scene->smooth_movement = 0.5f;

	return 1;
}

void SceneQuit(Scene* scene)
{
	BackgroundClear(&scene->background);

	SceneClearEntities(scene);

	free(scene->layers);

	EcsDestroy(&scene->ecs);

	WorldDestroy(scene->world);
	free(scene->world);
}

void SceneAddEntity(Scene* scene, EcsEntity* entity, size_t layer)
{
	if (!entity)
		return;

	EcsPhysicsComponent* phys = entity->physics;
	if (phys)
	{
		WorldAddBody(scene->world, phys->body);
	}

	EcsCameraComponent* cam = entity->camera;
	if (cam)
	{
		entity->camera->camera = scene->camera;
		entity->camera->scene_w = scene->width;
		entity->camera->scene_h = scene->height;
	}

	layer_vector_push(&scene->layers[layer], entity);
}

void SceneAddEntityPos(Scene* scene, EcsEntity* entity, size_t layer, vec2 position)
{
	if (!entity) return;

	EcsEntitySetPosition(entity, position);
	SceneAddEntity(scene, entity, layer);
}

void SceneRemoveEntity(Scene* scene, const char* name, size_t layer)
{
	if (layer >= scene->max_layer)
		return;

	for (size_t i = 0; i < scene->layers[layer].size; i++)
	{
		if (strcmp(layer_vector_get(&scene->layers[layer], i)->name, name) == 0)
		{
			layer_vector_delete(&scene->layers[layer], i);
			return;
		}
	}
}

void SceneClearEntities(Scene* scene)
{
	for (size_t layer = 0; layer < scene->max_layer; layer++)
	{
		for (size_t i = 0; i < scene->layers[layer].size; i++)
			free(layer_vector_get(&scene->layers[layer], i));
		clib_vector_free(&scene->layers[layer]);
	}
}

void SceneOnEvent(Scene* scene, const Event e)
{
}

void SceneOnUpdate(Scene* scene, float deltaTime)
{
	WorldTick(scene->world, deltaTime);

	for (size_t layer = 0; layer < scene->max_layer; layer++)
		for (size_t i = 0; i < scene->layers[layer].size; i++)
		{
			EcsUpdate(&scene->ecs, layer_vector_get(&scene->layers[layer], i), deltaTime);
		}

	BackgroundUpdate(&scene->background, scene->camera->position.x - scene->camera->size.x / 2.0f, deltaTime);
}

void SceneOnRender(Scene* scene)
{
	BackgroundRender(&scene->background, CameraGetViewProjectionPtr(scene->camera));

	BatchRenderer2DStart(CameraGetViewProjectionPtr(scene->camera));

	for (size_t layer = 0; layer < scene->max_layer; layer++)
		for (size_t i = 0; i < scene->layers[layer].size; i++)
		{
			EcsRender(&scene->ecs, layer_vector_get(&scene->layers[layer], i));
		}

	BatchRenderer2DFlush();
}

void SceneOnRenderDebug(Scene* scene)
{
	Primitives2DStart(CameraGetViewProjectionPtr(scene->camera));

	for (size_t layer = 0; layer < scene->max_layer; layer++)
	{
		for (size_t i = 0; i < scene->layers[layer].size; i++)
		{
			vec2 pos = EcsEntityGetPosition(layer_vector_get(&scene->layers[layer], i));
			Primitives2DRenderCircle(pos.x, pos.y, 2.0f, IGNIS_WHITE);
		}
	}

	for (size_t i = 0; i < scene->world->bodies.size; i++)
	{
		Body* body = WorldGetBody(scene->world, i);

		vec2 pos = vec2_sub(body->position, body->halfSize);
		vec2 dim = vec2_mult(body->halfSize, 2.0f);
		Primitives2DRenderRect(pos.x, pos.y, dim.x, dim.y, body->type == BODY_TYPE_DYNAMIC ? IGNIS_GREEN : IGNIS_WHITE);
	}

	Primitives2DFlush();
}

EcsEntity* SceneGetEntity(Scene* scene, const char* name, size_t layer)
{
	if (layer >= scene->max_layer)
		return NULL;

	for (size_t i = 0; i < scene->layers[layer].size; i++)
	{
		if (strcmp(layer_vector_get(&scene->layers[layer], i)->name, name) == 0)
			return layer_vector_get(&scene->layers[layer], i);
	}

	return NULL;
}

EcsEntity* SceneGetEntityAt(Scene* scene, vec2 pos, size_t layer)
{
	if (layer >= scene->max_layer)
		return NULL;

	for (size_t i = 0; i < scene->layers[layer].size; i++)
	{
		EcsTextureComponent* tex = layer_vector_get(&scene->layers[layer], i)->texture;

		if (!tex) continue;

		vec2 position = EcsEntityGetPosition(layer_vector_get(&scene->layers[layer], i));

		vec2 min = vec2_sub(position, (vec2){ tex->width / 2.0f, 0.0f });
		vec2 max = vec2_add(min, (vec2){ tex->width, tex->height });

		if (vec2_inside(pos, min, max))
			return layer_vector_get(&scene->layers[layer], i);
	}

	return NULL;
}

line* SceneGetEdges(Scene* scene, size_t* line_count)
{
	line* lines = (line*)malloc(sizeof(line) * scene->world->bodies.size * 4);

	size_t line_index = 0;
	for (size_t i = 0; i < scene->world->bodies.size; i++)
	{
		Body* body = WorldGetBody(scene->world, i);
		rect r = BodyGetRect(body);

		lines[line_index++] = (line){ { r.x, r.y }, { r.x + r.w, r.y } };
		lines[line_index++] = (line){ { r.x + r.w, r.y }, { r.x + r.w, r.y + r.h } };
		lines[line_index++] = (line){ { r.x + r.w, r.y + r.h }, { r.x, r.y + r.h } };
		lines[line_index++] = (line){ { r.x, r.y + r.h }, {  r.x, r.y } };
	}

	*line_count = line_index;

	return lines;
}
