#include "Player.h"

#include "Frost/FrostParser.h"

void PlayerLoad(char* ini, Scene* scene, EcsEntityID entity)
{
    tb_ini_element element;
    tb_ini_query(ini, "player", NULL, &element);
    if (element.error == TB_INI_OK)
    {
        Player comp;
        comp.move_left =  GLFW_KEY_A;
        comp.move_right = GLFW_KEY_D;
        comp.jump =       GLFW_KEY_SPACE;

        EcsAddDataComponent(&scene->ecs, entity, COMPONENT_PLAYER, &comp);
    }
}
