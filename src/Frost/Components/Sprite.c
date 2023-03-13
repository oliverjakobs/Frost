#include "Sprite.h"

#include "Frost/FrostParser.h"

void SpriteLoad(char* ini, Scene* scene, EcsEntityID entity, int z_index, int variant)
{
    Transform* transform = EcsGetDataComponent(&scene->ecs, entity, COMPONENT_TRANSFORM);
    if (!transform)
    {
        MINIMAL_ERROR("[ECS] Sprite requires Transform");
        return;
    }

    tb_ini_element element;
    tb_ini_query(ini, "sprite", NULL, &element);
    if (element.error == TB_INI_OK)
    {
        Sprite sprite;
        sprite.flip = SPRITE_FLIP_NONE;
        sprite.width = tb_ini_float(element.start, NULL, "width", transform->size.x);
        sprite.height = tb_ini_float(element.start, NULL, "height", transform->size.y);

        sprite.frame = FrostMatchVariant(element.start, NULL, "frame", variant, 0);

        /* load texture */
        char path[APPLICATION_PATH_LEN];
        tb_ini_string(element.start, NULL, "path", path, APPLICATION_PATH_LEN);

        int rows = tb_ini_int(element.start, NULL, "rows", 1);
        int cols = tb_ini_int(element.start, NULL, "cols", 1);

        sprite.texture = ResourcesLoadTexture2D(&scene->res, path, rows, cols);

        if (sprite.texture)
        {
            EcsAddDataComponent(&scene->ecs, entity, COMPONENT_SPRITE, &sprite);
            EcsAddOrderComponent(&scene->ecs, entity, COMPONENT_Z_INDEX, &z_index);
        }
        else
        {
            MINIMAL_ERROR("[Scenes] Failed to load texture: %s", path);
        }
    }
}

IgnisRect SpriteGetSrcRect(const Sprite* sprite)
{
    IgnisRect src = ignisGetTexture2DSrcRect(sprite->texture, sprite->frame);

    if (sprite->flip == SPRITE_FLIP_HORIZONTAL || sprite->flip == SPRITE_FLIP_BOTH)
    {
        src.x += src.w;
        src.w = -src.w;
    }

    if (sprite->flip == SPRITE_FLIP_VERTICAL || sprite->flip == SPRITE_FLIP_BOTH)
    {
        src.y += src.h;
        src.h = -src.h;
    }

    return src;
}
