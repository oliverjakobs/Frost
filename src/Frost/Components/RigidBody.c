#include "RigidBody.h"

#include "Frost/FrostParser.h"

void RigidBodyLoad(char* ini, Scene* scene, EcsEntityID entity)
{
    Transform* transform = EcsGetDataComponent(&scene->ecs, entity, COMPONENT_TRANSFORM);
    if (!transform)
    {
        MINIMAL_ERROR("[ECS] RigidBody requires Transform");
        return;
    }

    tb_ini_element element;
    tb_ini_query(ini, "rigidbody", NULL, &element);
    if (element.error == TB_INI_OK)
    {
        TileBodyType type = tb_ini_parse(element.start, NULL, "type", FrostParseRigidBodyType);

        float half_w = tb_ini_float(element.start, NULL, "half_width", transform->size.x / 2.0f);
        float half_h = tb_ini_float(element.start, NULL, "half_height", transform->size.y / 2.0f);

        RigidBody body;
        body.filter = RIGID_BODY_FILTER_NONE;

        tb_ini_element filter;
        tb_ini_csv(element.start, NULL, "filter", &filter);
        if (filter.error == TB_INI_OK)
        {
            tb_ini_element filter_bit;
            char* value = filter.start;
            for (size_t i = 0; i < filter.len; ++i)
            {
                value = tb_ini_csv_step(value, &filter_bit);
                body.filter |= FrostParseRigidBodyFilter(filter_bit.start, filter_bit.len);
            }
        }

        body.offset.x = tb_ini_float(element.start, NULL, "offset_x", 0.0f);
        body.offset.y = tb_ini_float(element.start, NULL, "offset_y", half_h);

        vec2 position = vec2_add(transform->position, body.offset);
        TileBodyInit(&body.body, type, position.x, position.y, half_w, half_h);

        if (type == TILE_BODY_DYNAMIC) TileBodySetSensor(&body.body, 2.0f, 2.0f);

        EcsAddDataComponent(&scene->ecs, entity, COMPONENT_RIGID_BODY, &body);
    }
}
