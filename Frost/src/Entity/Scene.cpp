#include "Scene.hpp"

CLIB_VECTOR_DEFINE_FUNCS(layer, Entity)

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
	WorldLoad(scene->world, vec2_(0.0f, -980.0f));
	
	scene->smooth_movement = 0.5f;

	return 1;
}

void SceneQuit(Scene* scene)
{
	BackgroundClear(&scene->background);

	SceneClearEntities(scene);

	free(scene->layers);

	WorldDestroy(scene->world);
	free(scene->world);
}

void SceneAddEntity(Scene* scene, Entity* entity, size_t layer)
{
	if (entity == nullptr)
		return;

	auto phys = EntityGetComponent<PhysicsComponent>(entity);

	if (phys != nullptr)
	{
		WorldAddBody(scene->world, phys->GetBody().get());
	}

	layer_vector_push(&scene->layers[layer], entity);
}

void SceneAddEntity(Scene* scene, Entity* entity, size_t layer, const vec2 position)
{
	if (entity == nullptr)
		return;

	EntitySetPosition(entity, position);
	SceneAddEntity(scene, entity, layer);
}

void SceneRemoveEntity(Scene* scene, const char* name, size_t layer)
{
	if (layer >= scene->max_layer)
		return;

	for (size_t i = 0; i < scene->layers[layer].size; i++)
	{
		if (strcmp(layer_vector_get(&scene->layers[layer], i)->name.c_str(), name) == 0)
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
			delete layer_vector_get(&scene->layers[layer], i);
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
			EntityOnUpdate(layer_vector_get(&scene->layers[layer], i), scene, deltaTime);

	BackgroundUpdate(&scene->background, scene->camera->position.x - scene->camera->size.x / 2.0f, deltaTime);
}

void SceneOnRender(Scene* scene)
{
	BackgroundRender(&scene->background, CameraGetViewProjectionPtr(scene->camera));

	BatchRenderer2DStart(CameraGetViewProjectionPtr(scene->camera));

	for (size_t layer = 0; layer < scene->max_layer; layer++)
		for (size_t i = 0; i < scene->layers[layer].size; i++)
			EntityOnRender(layer_vector_get(&scene->layers[layer], i), scene);

	BatchRenderer2DFlush();
}

void SceneOnRenderDebug(Scene* scene)
{
	Primitives2DStart(CameraGetViewProjectionPtr(scene->camera));

	for (size_t layer = 0; layer < scene->max_layer; layer++)
	{
		for (size_t i = 0; i < scene->layers[layer].size; i++)
		{
			EntityOnRenderDebug(layer_vector_get(&scene->layers[layer], i));

			auto& pos = EntityGetPosition(layer_vector_get(&scene->layers[layer], i));
			Primitives2DRenderCircle(pos.x, pos.y, 2.0f, IGNIS_WHITE);
		}
	}

	for (size_t i = 0; i < scene->world->bodies.size; i++)
	{
		Body* body = WorldGetBody(scene->world, i);

		auto& pos = vec2_sub(body->position, body->halfSize);
		auto& dim = vec2_mult(body->halfSize, 2.0f);
		Primitives2DRenderRect(pos.x, pos.y, dim.x, dim.y, body->type == BODY_TYPE_DYNAMIC ? IGNIS_GREEN : IGNIS_WHITE);
	}

	//Primitives2D::DrawCircle(m_camera->GetPosition(), 2.0f);
	//
	//// smooth movement area
	//float smooth_w = (m_camera->GetSize().x / 2.0f) * m_smoothMovement;
	//float smooth_h = (m_camera->GetSize().y / 2.0f) * m_smoothMovement;
	//
	//Primitives2D::DrawRect(m_camera->GetPosition().x - smooth_w, m_camera->GetPosition().y - smooth_h, smooth_w * 2.0f, smooth_h * 2.0f);

	Primitives2DFlush();
}

void SceneSetCameraPosition(Scene* scene, const vec2 position)
{
	float smooth_w = (scene->camera->size.x / 2.0f) * scene->smooth_movement;
	float smooth_h = (scene->camera->size.y / 2.0f) * scene->smooth_movement;

	float center_x = scene->camera->position.x;
	float center_y = scene->camera->position.y;

	if (position.x > scene->camera->position.x + smooth_w)
		center_x = position.x - smooth_w;
	if (position.x < scene->camera->position.x - smooth_w)
		center_x = position.x + smooth_w;

	if (position.y > scene->camera->position.y + smooth_h)
		center_y = position.y - smooth_h;
	if (position.y < scene->camera->position.y - smooth_h)
		center_y = position.y + smooth_h;

	// constraint
	float constraint_x = scene->camera->size.x / 2.0f;
	float constraint_y = scene->camera->size.y / 2.0f;

	if (center_x < constraint_x)
		center_x = constraint_x;
	if (center_x > scene->width - constraint_x)
		center_x = scene->width - constraint_x;

	if (center_y < constraint_y)
		center_y = constraint_y;
	if (center_y > scene->height - constraint_y)
		center_y = scene->height - constraint_y;

	scene->camera->position.x = center_x;
	scene->camera->position.y = center_y;
	CameraUpdateViewOrtho(scene->camera);
}

Entity* SceneGetEntity(Scene* scene, const char* name, size_t layer)
{
	if (layer >= scene->max_layer)
		return nullptr;

	for (size_t i = 0; i < scene->layers[layer].size; i++)
	{
		if (strcmp(layer_vector_get(&scene->layers[layer], i)->name.c_str(), name) == 0)
			return layer_vector_get(&scene->layers[layer], i);
	}

	return nullptr;
}

bool inside(const vec2 min, const vec2 max, const vec2& p)
{
	if (p.x < min.x || p.y < min.y) return false;
	if (p.x > max.x || p.y > max.y) return false;

	return true;
}

Entity* SceneGetEntityAt(Scene* scene, const vec2 pos, size_t layer)
{
	if (layer >= scene->max_layer)
		return nullptr;

	for (size_t i = 0; i < scene->layers[layer].size; i++)
	{
		auto tex = EntityGetComponent<TextureComponent>(layer_vector_get(&scene->layers[layer], i));

		if (tex == nullptr)
			continue;

		vec2 position = EntityGetPosition(layer_vector_get(&scene->layers[layer], i));

		vec2 min = vec2_sub(position, vec2_(tex->GetWidth() / 2.0f, 0.0f));
		vec2 max = vec2_add(min, tex->GetDimension());

		if (inside(min, max, pos))
			return layer_vector_get(&scene->layers[layer], i);
	}

	return nullptr;
}
