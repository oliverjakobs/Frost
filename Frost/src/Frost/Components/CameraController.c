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
		CameraController comp;
		comp.smooth = tb_json_float(element.value, "{'smooth'", NULL, 0.0f);
		comp.camera = &scene->camera;
		comp.scene_w = SceneGetWidth(scene);
		comp.scene_h = SceneGetHeight(scene);

		EcsAddDataComponent(ecs, entity, COMPONENT_CAMERA, &comp);
	}
}

void CameraControllerMoveConstrained(CameraController* controller, vec2 pos, float constraint)
{
	if (!controller) return;

	vec2 center = CameraGetCenter(controller->camera);

	float constraint_x = controller->camera->size.x * constraint;
	float constraint_y = controller->camera->size.y * constraint;

	float smooth_w = constraint_x * controller->smooth;
	float smooth_h = constraint_y * controller->smooth;

	center.x = tb_clampf(center.x, pos.x - smooth_w, pos.x + smooth_w);
	center.y = tb_clampf(center.y, pos.y - smooth_h, pos.y + smooth_h);

	// constraint
	center.x = tb_clampf(center.x, constraint_x, controller->scene_w - constraint_x);
	center.y = tb_clampf(center.y, constraint_y, controller->scene_h - constraint_y);

	CameraSetCenter(controller->camera, center);
}
