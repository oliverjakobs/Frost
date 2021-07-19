#include "Movement.h"

#include "Frost/FrostParser.h"

void MovementLoad(char* ini, Ecs* ecs, EcsEntityID entity)
{
    tb_ini_element element;
    tb_ini_query(ini, "movement", NULL, &element);
    if (element.error == TB_INI_OK)
    {
        Movement comp;

        comp.direction = MOVEMENT_LEFT;
        comp.speed = tb_ini_query_float(element.start, NULL, "speed", 0.0f);
        comp.bounce = tb_ini_query_float(element.start, NULL, "bounce", 0.0f);

        EcsAddDataComponent(ecs, entity, COMPONENT_MOVEMENT, &comp);
    }
}
