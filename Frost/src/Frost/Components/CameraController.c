#include "CameraController.h"

#include "Frost/FrostEcs.h"

#include "json/tb_json.h"

void CameraControllerLoad(Scene* scene, EntityID entity, char* json)
{
	tb_json_element element;
	tb_json_read(json, &element, "{'camera'");
	if (element.error == TB_JSON_OK)
	{
		CameraController comp;
		comp.camera = scene->camera;
		comp.smooth = tb_json_float((char*)element.value, "{'smooth'", NULL, 0.0f);
		comp.scene_w = scene->width;
		comp.scene_h = scene->height;

		EcsAddDataComponent(&scene->ecs, entity, COMPONENT_CAMERA, &comp);
	}
}
