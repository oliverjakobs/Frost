#include "CameraController.h"

#include "Frost/FrostParser.h"
#include "toolbox/tb_algorithm.h"

void CameraControllerLoad(char* ini, Ecs* ecs, EcsEntityID entity, Scene* scene)
{
    tb_ini_element element;
    tb_ini_query(ini, "camera", NULL, &element);
    if (element.error == TB_INI_OK)
    {
        CameraController comp;
        comp.camera = &scene->camera;
        comp.scene_size.x = SceneGetWidth(scene);
        comp.scene_size.y = SceneGetHeight(scene);
        comp.smooth = tb_ini_float(element.start, NULL, "smooth", 0.0f);

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
