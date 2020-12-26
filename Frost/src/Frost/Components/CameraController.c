#include "CameraController.h"

#include "Frost/Frost.h"

#include "toolbox/tb_json.h"

void CameraControllerLoad(Scene* scene, EcsEntityID entity, char* json)
{
	tb_json_element element;
	tb_json_read(json, &element, "{'camera'");
	if (element.error == TB_JSON_OK)
	{
		CameraController comp;
		comp.smooth = tb_json_float(element.value, "{'smooth'", NULL, 0.0f);
		comp.camera = scene->camera;
		comp.scene_w = scene->width;
		comp.scene_h = scene->height;

		EcsAddDataComponent(&scene->ecs, entity, COMPONENT_CAMERA, &comp);
	}
}

void CameraControllerMoveConstrained(CameraController* controller, vec2 pos, float constraint)
{
	if (!controller) return;

	float center_x = controller->camera->position.x;
	float center_y = controller->camera->position.y;

	float constraint_x = controller->camera->size.x * constraint;
	float constraint_y = controller->camera->size.y * constraint;

	float smooth_w = constraint_x * controller->smooth;
	float smooth_h = constraint_y * controller->smooth;

	if (pos.x > controller->camera->position.x + smooth_w)
		center_x = pos.x - smooth_w;
	if (pos.x < controller->camera->position.x - smooth_w)
		center_x = pos.x + smooth_w;

	if (pos.y > controller->camera->position.y + smooth_h)
		center_y = pos.y - smooth_h;
	if (pos.y < controller->camera->position.y - smooth_h)
		center_y = pos.y + smooth_h;

	// constraint
	if (center_x < constraint_x)
		center_x = constraint_x;
	if (center_x > controller->scene_w - constraint_x)
		center_x = controller->scene_w - constraint_x;

	if (center_y < constraint_y)
		center_y = constraint_y;
	if (center_y > controller->scene_h - constraint_y)
		center_y = controller->scene_h - constraint_y;

	controller->camera->position.x = center_x;
	controller->camera->position.y = center_y;

	CameraUpdateViewOrtho(controller->camera);
}
