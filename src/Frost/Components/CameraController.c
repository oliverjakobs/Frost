#include "CameraController.h"

#include "Frost/Frost.h"

#include "toolbox/tb_json.h"
#include "toolbox/toolbox.h"

void CameraControllerLoad(char* json, Ecs* ecs, EcsEntityID entity, Scene* scene)
{
	tb_json_element element;
	tb_json_read(json, &element, "{'camera'");
	if (element.error == TB_JSON_OK)
	{
		CameraController comp =
		{
			&scene->camera,
			SceneGetWidth(scene),
			SceneGetHeight(scene),
			tb_json_float(element.value, "{'smooth'", NULL, 0.0f)
		};

		EcsAddDataComponent(ecs, entity, COMPONENT_CAMERA, &comp);
	}
}

void CameraControllerMoveConstrained(CameraController* controller, vec2 pos, float constraint_ratio)
{
	if (!controller) return;

	vec2 center = CameraGetCenter(controller->camera);
	vec2 constraint = vec2_mult(controller->camera->size, constraint_ratio);
	vec2 smooth = vec2_mult(constraint, controller->smooth);

	center.x = tb_clampf(center.x, pos.x - smooth.x, pos.x + smooth.x);
	center.y = tb_clampf(center.y, pos.y - smooth.y, pos.y + smooth.y);

	// constraint
	center.x = tb_clampf(center.x, constraint.x, controller->scene_size.x - constraint.x);
	center.y = tb_clampf(center.y, constraint.y, controller->scene_size.y - constraint.y);

	CameraSetCenterOrtho(controller->camera, center);
}
