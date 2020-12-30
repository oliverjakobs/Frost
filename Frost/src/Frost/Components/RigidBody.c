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
		RigidBody body;
		body.type = (RigidBodyType)tb_json_int(element.value, "{'type'", NULL, RIGID_BODY_STATIC);
		body.filter = (RigidBodyFilter)tb_json_int(element.value, "{'filter'", NULL, RIGID_BODY_FILTER_NONE);

		float half_w = tb_json_float(element.value, "{'halfsize'[0", NULL, transform->size.x / 2.0f);
		float half_h = tb_json_float(element.value, "{'halfsize'[1", NULL, transform->size.y / 2.0f);

		body.offset.x = tb_json_float(element.value, "{'offset'[0", NULL, 0.0f);
		body.offset.y = tb_json_float(element.value, "{'offset'[1", NULL, half_h);

		vec2 position = vec2_add(transform->position, body.offset);
		TileBodyInit(&body.body, &scene->map, position.x, position.y, half_w, half_h);

		EcsAddDataComponent(&scene->ecs, entity, COMPONENT_RIGID_BODY, &body);
	}
}
