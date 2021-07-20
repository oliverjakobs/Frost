#include "Player.h"

#include "Frost/FrostParser.h"

void PlayerLoad(char* ini, Ecs* ecs, EcsEntityID entity)
{
    tb_ini_element element;
    tb_ini_query(ini, "player", NULL, &element);
    if (element.error == TB_INI_OK)
    {
        Player comp;
        comp.move_left =  MINIMAL_KEY_A;
        comp.move_right = MINIMAL_KEY_D;
        comp.jump =       MINIMAL_KEY_SPACE;

        EcsAddDataComponent(ecs, entity, COMPONENT_PLAYER, &comp);
    }
}
