#include "Scene.hpp"

#include "Obelisk/Obelisk.hpp"

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

	scene->world = new World(glm::vec2(0.0f, -980.0f));
	
	scene->smooth_movement = 0.5f;

	return 1;
}

void SceneQuit(Scene* scene)
{
	BackgroundClear(&scene->background);

	SceneClearEntities(scene);

	free(scene->layers);

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

	layer_vector_push(&scene->layers[layer], entity);
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
	if (layer >= scene->max_layer)
		return;

	for (size_t i = 0; i < scene->layers[layer].size; i++)
	{
		if (obelisk::StringCompare(layer_vector_get(&scene->layers[layer], i)->name, name))
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

void SceneOnEvent(Scene* scene, const Event& e)
{
}

void SceneOnUpdate(Scene* scene, float deltaTime)
{
	scene->world->Tick(deltaTime);

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
	if (layer >= scene->max_layer)
		return nullptr;

	for (size_t i = 0; i < scene->layers[layer].size; i++)
	{
		if (obelisk::StringCompare(layer_vector_get(&scene->layers[layer], i)->name, name))
			return layer_vector_get(&scene->layers[layer], i);
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
	if (layer >= scene->max_layer)
		return nullptr;

	for (size_t i = 0; i < scene->layers[layer].size; i++)
	{
		auto tex = EntityGetComponent<TextureComponent>(layer_vector_get(&scene->layers[layer], i));

		if (tex == nullptr)
			continue;

		glm::vec2 position = EntityGetPosition(layer_vector_get(&scene->layers[layer], i));

		glm::vec2 min = position - glm::vec2(tex->GetWidth() / 2.0f, 0.0f);
		glm::vec2 max = min + tex->GetDimension();

		if (inside(min, max, pos))
			return layer_vector_get(&scene->layers[layer], i);
	}

	return nullptr;
}

clib_vector* SceneGetLayer(Scene* scene, size_t layer)
{
	if (layer >= scene->max_layer)
		return NULL;

	return &scene->layers[layer];
}

std::vector<size_t> SceneGetUsedLayers(Scene* scene)
{
	std::vector<size_t> layers;
	for (size_t i = 0; i < scene->max_layer; i++)
	{
		if (scene->layers[i].size > 0)
			layers.push_back(i);
	}

	return layers;
}
