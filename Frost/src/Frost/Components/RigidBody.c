#include "RigidBody.h"

#include "Frost/Frost.h"

#include "toolbox/toolbox.h"
#include "toolbox/tb_json.h"

#include "Application/Debugger.h"

void RigidBodyLoad(Scene* scene, EcsEntityID entity, char* json)
{
	Transform* transform = EcsGetDataComponent(&scene->ecs, entity, COMPONENT_TRANSFORM);
	if (!transform)
	{
		DEBUG_ERROR("[ECS] RigidBody requires Transform\n");
		return;
	}

	tb_json_element element;
	tb_json_read(json, &element, "{'rigidbody'");
	if (element.error == TB_JSON_OK)
	{
		TileBodyType type = tb_json_parse(element.value, "{'type'", NULL, (tb_json_parse_func)RigidBodyParseType);

		float half_w = tb_json_float(element.value, "{'halfsize'[0", NULL, transform->size.x / 2.0f);
		float half_h = tb_json_float(element.value, "{'halfsize'[1", NULL, transform->size.y / 2.0f);

		RigidBody body;
		body.filter = tb_json_parse(element.value, "{'filter'", NULL, (tb_json_parse_func)RigidBodyParseFilter);

		body.offset.x = tb_json_float(element.value, "{'offset'[0", NULL, 0.0f);
		body.offset.y = tb_json_float(element.value, "{'offset'[1", NULL, half_h);

		vec2 position = vec2_add(transform->position, body.offset);
		TileBodyInit(&body.body, &scene->map, type, position.x, position.y, half_w, half_h);

		EcsAddDataComponent(&scene->ecs, entity, COMPONENT_RIGID_BODY, &body);
	}
}

TileBodyType RigidBodyParseType(const char* str, size_t max_count)
{
	switch (str[0])
	{
	case 'D': if (strncmp(str, "DYNAMIC", max_count) == 0) return TILE_BODY_DYNAMIC;
	}
	return TILE_BODY_STATIC;
}

RigidBodyFilter RigidBodyParseFilter(const char* str, size_t max_count)
{
	switch (str[0])
	{
	case 'W': if (strncmp(str, "WORLD", max_count) == 0) return RIGID_BODY_FILTER_WORLD; break;
	case 'P': if (strncmp(str, "PLAYER", max_count) == 0) return RIGID_BODY_FILTER_PLAYER; break;
	}
    return RIGID_BODY_FILTER_NONE;
}
