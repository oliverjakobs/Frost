#include "RigidBody.h"

#include "Frost/FrostParser.h"

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
		TileBodyType type = tb_json_parse(element.value, "{'type'", NULL, FrostParseRigidBodyType);

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
				body.filter |= FrostParseRigidBodyFilter(filter_bit.value, filter_bit.bytelen);
			}
		}

		body.offset.x = tb_json_float(element.value, "{'offset'[0", NULL, 0.0f);
		body.offset.y = tb_json_float(element.value, "{'offset'[1", NULL, half_h);

		vec2 position = vec2_add(transform->position, body.offset);
		TileBodyInit(&body.body, type, position.x, position.y, half_w, half_h);

		if (type == TILE_BODY_DYNAMIC) TileBodySetSensor(&body.body, 2.0f, 2.0f);

		EcsAddDataComponent(&scene->ecs, entity, COMPONENT_RIGID_BODY, &body);
	}
}
