#include "RigidBody.h"

#include "Frost/Frost.h"

#include "toolbox/tb_str.h"
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
		body.filter = RIGID_BODY_FILTER_NONE;

		tb_json_element filter;
		tb_json_read(element.value, &filter, "{'filter'");
		if (filter.error == TB_JSON_OK && filter.data_type == TB_JSON_ARRAY)
		{
			tb_json_element filter_bit;
			char* value = filter.value;
			for (size_t i = 0; i < filter.elements; ++i)
			{
				value = tb_json_array_step(value, &filter_bit);
				body.filter |= RigidBodyParseFilter(filter_bit.value, filter_bit.bytelen);
			}
		}

		body.offset.x = tb_json_float(element.value, "{'offset'[0", NULL, 0.0f);
		body.offset.y = tb_json_float(element.value, "{'offset'[1", NULL, half_h);

		vec2 position = vec2_add(transform->position, body.offset);
		TileBodyInit(&body.body, &scene->map, type, position.x, position.y, half_w, half_h);

		EcsAddDataComponent(&scene->ecs, entity, COMPONENT_RIGID_BODY, &body);
	}
}

TileBodyType RigidBodyParseType(const char* str, size_t max_count)
{
	if (max_count > 0)
	{
		if (tb_strncasecmp(str, "dynamic", max_count) == 0) return TILE_BODY_DYNAMIC;
	}

	return TILE_BODY_STATIC;
}

RigidBodyFilter RigidBodyParseFilter(const char* str, size_t max_count)
{
	if (max_count > 0)
	{
		if (tb_strncasecmp(str, "world", max_count) == 0) return RIGID_BODY_FILTER_WORLD;
		if (tb_strncasecmp(str, "player", max_count) == 0) return RIGID_BODY_FILTER_PLAYER;
		if (tb_strncasecmp(str, "npc", max_count) == 0) return RIGID_BODY_FILTER_NPC;
		if (tb_strncasecmp(str, "door", max_count) == 0) return RIGID_BODY_FILTER_DOOR;
	}

	return RIGID_BODY_FILTER_NONE;
}
