#include "Scene.hpp"

#include "Obelisk/Obelisk.hpp"

void _SceneLayerInit(SceneLayer* layer, size_t initial_size)
{
	layer->entities = (Entity**)malloc(initial_size * sizeof(Entity));
	layer->used = 0;
	layer->size = initial_size;
}

void _SceneLayerFree(SceneLayer* layer)
{
	free(layer->entities);
	layer->entities = NULL;
	layer->used = layer->size = 0;
}

void _SceneLayerInsert(SceneLayer* layer, Entity* entity)
{
	// a->used is the number of used entries, because a->array[a->used++] updates a->used only *after* the array has been accessed.
	// Therefore a->used can go up to a->size 
	if (layer->used == layer->size)
	{
		layer->size *= 2;
		layer->entities = (Entity**)realloc(layer->entities, layer->size * sizeof(Entity));
	}
	layer->entities[layer->used++] = entity;
}

int SceneLoad(Scene* scene, Camera* camera, float w, float h)
{
	scene->camera = camera;
	scene->width = w;
	scene->height = h;

	scene->world = new World(glm::vec2(0.0f, -980.0f));
	
	scene->smooth_movement = 0.5f;

	for (size_t i = 0; i < SCENE_MAX_LAYER; i++)
		_SceneLayerInit(&scene->layers[i], 4);

	return 1;
}

void SceneQuit(Scene* scene)
{
	SceneClearEntities(scene);

	delete scene->world;
}

void SceneAddEntity(Scene* scene, Entity* entity, size_t layer)
{
	if (entity == nullptr)
		return;

	auto phys = EntityGetComponent<PhysicsComponent>(entity);

	if (phys != nullptr)
	{
		scene->world->AddBody(phys->GetBody());
	}

	_SceneLayerInsert(&scene->layers[layer], entity);
}

void SceneAddEntity(Scene* scene, Entity* entity, size_t layer, const glm::vec2& position)
{
	if (entity == nullptr)
		return;

	EntitySetPosition(entity, position);
	SceneAddEntity(scene, entity, layer);
}

void SceneRemoveEntity(Scene* scene, const std::string& name, size_t layer)
{
	if (layer >= SCENE_MAX_LAYER)
		return;

	// scene->entities[layer].erase(std::remove_if(scene->entities[layer].begin(), scene->entities[layer].end(), [&](auto& e)
	// 	{
	// 		return obelisk::StringCompare(e->GetName(), name);
	// 	}), scene->entities[layer].end());
}

void SceneClearEntities(Scene* scene)
{
	for (size_t layer = 0; layer < SCENE_MAX_LAYER; layer++)
	{
		for (size_t i = 0; i < scene->layers[layer].used; i++)
		{
			delete scene->layers[layer].entities[i];
		}

		_SceneLayerFree(&scene->layers[layer]);
	}
}

void SceneOnEvent(Scene* scene, const Event& e)
{
}

void SceneOnUpdate(Scene* scene, float deltaTime)
{
	scene->world->Tick(deltaTime);

	for (size_t layer = 0; layer < SCENE_MAX_LAYER; layer++)
		for (size_t i = 0; i < scene->layers[layer].used; i++)
			EntityOnUpdate(scene->layers[layer].entities[i], scene, deltaTime);
}

void SceneOnRender(Scene* scene)
{
	BatchRenderer2DStart(CameraGetViewProjectionPtr(scene->camera));

	for (size_t layer = 0; layer < SCENE_MAX_LAYER; layer++)
		for (size_t i = 0; i < scene->layers[layer].used; i++)
			EntityOnRender(scene->layers[layer].entities[i], scene);

	BatchRenderer2DFlush();
}

void SceneOnRenderDebug(Scene* scene)
{
	Primitives2DStart(CameraGetViewProjectionPtr(scene->camera));

	for (size_t layer = 0; layer < SCENE_MAX_LAYER; layer++)
	{
		for (size_t i = 0; i < scene->layers[layer].used; i++)
		{
			EntityOnRenderDebug(scene->layers[layer].entities[i]);

			auto& pos = EntityGetPosition(scene->layers[layer].entities[i]);
			Primitives2DRenderCircle(pos.x, pos.y, 2.0f, IGNIS_WHITE);
		}
	}

	for (auto& body : scene->world->GetBodies())
	{
		auto& pos = body->GetPosition() - body->GetHalfSize();
		auto& dim = body->GetSize();
		Primitives2DRenderRect(pos.x, pos.y, dim.x, dim.y, body->GetType() == BodyType::DYNAMIC ? IGNIS_GREEN : IGNIS_WHITE);
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

void SceneSetCameraPosition(Scene* scene, const glm::vec3& position)
{
	float smooth_w = (scene->camera->size.x / 2.0f) * scene->smooth_movement;
	float smooth_h = (scene->camera->size.y / 2.0f) * scene->smooth_movement;

	glm::vec3 center = scene->camera->position;

	if (position.x > scene->camera->position.x + smooth_w)
		center.x = position.x - smooth_w;
	if (position.x < scene->camera->position.x - smooth_w)
		center.x = position.x + smooth_w;

	if (position.y > scene->camera->position.y + smooth_h)
		center.y = position.y - smooth_h;
	if (position.y < scene->camera->position.y - smooth_h)
		center.y = position.y + smooth_h;

	// constraint
	float constraint_x = scene->camera->size.x / 2.0f;
	float constraint_y = scene->camera->size.y / 2.0f;

	if (center.x < constraint_x)
		center.x = constraint_x;
	if (center.x > scene->width - constraint_x)
		center.x = scene->width - constraint_x;

	if (center.y < constraint_y)
		center.y = constraint_y;
	if (center.y > scene->height - constraint_y)
		center.y = scene->height - constraint_y;

	scene->camera->position = center;
	CameraUpdateViewOrtho(scene->camera);
}

Entity* SceneGetEntity(Scene* scene, const std::string& name, size_t layer)
{
	if (layer >= SCENE_MAX_LAYER)
		return nullptr;

	SceneLayer* scene_layer = &scene->layers[layer];
	for (size_t i = 0; i < scene_layer->used; i++)
	{
		if (obelisk::StringCompare(scene_layer->entities[i]->name, name))
			return scene_layer->entities[i];
	}

	return nullptr;
}

bool inside(const glm::vec2& min, const glm::vec2& max, const glm::vec2& p)
{
	if (p.x < min.x || p.y < min.y) return false;
	if (p.x > max.x || p.y > max.y) return false;

	return true;
}

Entity* SceneGetEntityAt(Scene* scene, const glm::vec2& pos, size_t layer)
{
	if (layer >= SCENE_MAX_LAYER)
		return nullptr;

	for (size_t i = 0; i < scene->layers[layer].used; i++)
	{
		auto tex = EntityGetComponent<TextureComponent>(scene->layers[layer].entities[i]);

		if (tex == nullptr)
			continue;

		glm::vec2 position = EntityGetPosition(scene->layers[layer].entities[i]);

		glm::vec2 min = position - glm::vec2(tex->GetWidth() / 2.0f, 0.0f);
		glm::vec2 max = min + tex->GetDimension();

		if (inside(min, max, pos))
			return scene->layers[layer].entities[i];
	}

	return nullptr;
}

SceneLayer* SceneGetLayer(Scene* scene, size_t layer)
{
	if (layer >= SCENE_MAX_LAYER)
		return NULL;

	return &scene->layers[layer];
}

std::vector<size_t> SceneGetUsedLayers(Scene* scene)
{
	std::vector<size_t> layers;
	for (size_t i = 0; i < SCENE_MAX_LAYER; i++)
	{
		if (scene->layers[i].used > 0)
			layers.push_back(i);
	}

	return layers;
}
