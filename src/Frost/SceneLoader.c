#include "SceneLoader.h"

#include "Memory.h"
#include "FrostParser.h"

#include "toolbox/tb_file.h"
#include "toolbox/tb_ini.h"

static IgnisTexture2D* SceneLoadTexture2D(char* ini, char* name, Resources* res)
{
    tb_ini_element texture;
    tb_ini_query(ini, name, NULL, &texture);

    if (texture.error != TB_INI_OK) return NULL;

    char path[APPLICATION_PATH_LEN];
    tb_ini_string(texture.start, NULL, "path", path, APPLICATION_PATH_LEN);

    int rows = tb_ini_int(texture.start, NULL, "rows", 1);
    int cols = tb_ini_int(texture.start, NULL, "cols", 1);

    return ResourcesLoadTexture2D(res, path, rows, cols);
}

int SceneLoad(Scene* scene, const char* path)
{
    if (!scene) return 0;

    char* ini = tb_file_read_alloc(path, "rb", FrostMalloc, FrostFree);

    if (!ini)
    {
        MINIMAL_ERROR("[Scenes] Couldn't read scene template: %s", path);
        return 0;
    }

    scene->gravity.x = 0.0f;
    scene->gravity.y = tb_ini_float(ini, "scene", "gravity", 0.0f);

    /* load item atlas */
    if ((scene->item_atlas = SceneLoadTexture2D(ini, "item_atlas", &scene->res)) == NULL)
        MINIMAL_WARN("[Scenes] Could not load item atlas.");

    /* load tile set */
    if ((scene->tile_set = SceneLoadTexture2D(ini, "tile_set", &scene->res)) == NULL)
        MINIMAL_WARN("[Scenes] Could not load tile set.");

    /* load background */
    if (!SceneLoadBackground(scene, ini))
    {
        MINIMAL_WARN("[Scenes] Failed to load background.");
        FrostFree(ini);
        return 0;
    }

    /* load map */
    if (!SceneLoadMap(scene, ini))
    {
        MINIMAL_WARN("[Scenes] Failed to load map.");
        FrostFree(ini);
        return 0;
    }

    /* load state */
    if (!SceneLoadState(scene, ini))
    {
        MINIMAL_WARN("[Scenes] Failed to load save state.");
        FrostFree(ini);
        return 0;
    }

    FrostFree(ini);
    return 1;
}

int SceneLoadTemplate(Scene* scene, EcsEntityID entity, const char* templ, vec2 pos, int z_index, int variant)
{
    char json[FROST_TEMPLATE_SIZE];
    size_t size = tb_file_read_buf(templ, "rb", json, FROST_TEMPLATE_SIZE);
    if (size == FROST_TEMPLATE_SIZE) MINIMAL_WARN("[Scenes] Template buffer may be too small");

    if (size == 0)
    {
        MINIMAL_WARN("[Scenes] Couldn't read template (%s)", templ);
        return 0;
    }

    EcsEntityUseID(entity);

    /* Load components */
    TemplateLoad(&scene->ecs, entity, templ, variant);
    EntityStateLoad(&scene->ecs, entity, ENTITY_STATE_NULL);

    TransformLoad(json, scene, entity, pos);
    RigidBodyLoad(json, scene, entity);
    SpriteLoad(json, scene, entity, z_index, variant);
    AnimatorLoad(json, scene, entity);
    MovementLoad(json, scene, entity);
    CameraControllerLoad(json, scene, entity);
    PlayerLoad(json, scene, entity);
    InventoryLoad(json, scene, entity);
    InteractableLoad(json, scene, entity);

    return 1;
}

static void* SceneStreamTiles(void* stream, TileID* id)
{
    tb_ini_element element;
    stream = tb_ini_csv_step(stream, &element);
    *id = tb_ini_element_to_int(&element);
    return stream;
}

static void* SceneStreamTileTypes(void* stream, TileType* type)
{
    tb_ini_element element;
    stream = tb_ini_csv_step(stream, &element);
    *type = FrostParseTileType(element.start, element.len);
    return stream;
}

int SceneLoadBackground(Scene* scene, char* ini)
{
    tb_ini_element background;
    tb_ini_query(ini, "background", NULL, &background);

    if (background.error == TB_INI_OK)
    {
        float x = tb_ini_float(background.start, NULL, "x", 0.0f);
        float y = tb_ini_float(background.start, NULL, "y", 0.0f);

        float w = tb_ini_float(background.start, NULL, "w", 0.0f);
        float h = tb_ini_float(background.start, NULL, "h", 0.0f);

        tb_ini_query(ini, "background.layers", NULL, &background);

        scene->background = BackgroundInit(background.len);

        tb_ini_element layer;
        char* layer_start = background.start;
        for (int i = 0; i < background.len; i++)
        {
            layer_start = tb_ini_property_next(layer_start, &layer);
            if (layer.error == TB_INI_OK)
            {
                char path[APPLICATION_PATH_LEN];
                tb_ini_name(&layer, path, APPLICATION_PATH_LEN);

                IgnisTexture2D texture;
                if (!ignisCreateTexture2D(&texture, path, 1, 1, 1, NULL))
                {
                    MINIMAL_WARN("Failed to load background layer %d (%s)", i, path);
                    continue;
                }

                float parallax = tb_ini_element_to_float(&layer);
                scene->background = BackgroundPushLayer(scene->background, texture, x, y, w, h, parallax);
            }
        }
    }
    return 1;
}

int SceneLoadMap(Scene* scene, char* ini)
{
    tb_ini_element map;
    tb_ini_query(ini, "map", NULL, &map);

    size_t rows = tb_ini_int(map.start, NULL, "rows", 0);
    size_t cols = tb_ini_int(map.start, NULL, "cols", 0);

    float tile_size = tb_ini_float(map.start, NULL, "tile_size", 0.0f);
     
    if (!TileMapLoad(&scene->map, rows, cols, tile_size, scene->allocator))
    {
        MINIMAL_WARN("[Scenes] Failed to load tile map.");
        return 0;
    }

    tb_ini_element types;
    tb_ini_csv(map.start, NULL, "types", &types);

    if (types.error != TB_INI_OK || !TileMapStreamTypes(&scene->map, types.start, SceneStreamTileTypes, types.len))
    {
        MINIMAL_WARN("[Scenes] Failed to stream tile types.");
        TileMapDestroy(&scene->map);
        return 0;
    }

    tb_ini_element tiles;
    tb_ini_csv(map.start, NULL, "tiles", &tiles);

    if (types.error != TB_INI_OK || !TileMapStreamTiles(&scene->map, tiles.start, SceneStreamTiles, tiles.len))
    {
        MINIMAL_WARN("[Scenes] Failed to stream tiles.");
        TileMapDestroy(&scene->map);
        return 0;
    }

    TileRendererBindMap(&scene->renderer, &scene->map);

    TileMapSetBorder(&scene->map, TILE_BOTTOM, 1);
    TileMapSetBorder(&scene->map, TILE_LEFT, 1);
    TileMapSetBorder(&scene->map, TILE_RIGHT, 1);

    return 1;
}

int SceneLoadState(Scene* scene, char* ini)
{
    tb_ini_element template;
    while ((ini = tb_ini_group_next(ini, "template", &template)) != NULL)
    {
        EcsEntityID entity = atoi(template.name);

        char path[APPLICATION_PATH_LEN];
        tb_ini_string(template.start, NULL, "path", path, APPLICATION_PATH_LEN);

        vec2 pos;
        pos.x = tb_ini_float(template.start, NULL, "x", 0.0f);
        pos.y = tb_ini_float(template.start, NULL, "y", 0.0f);

        int z_index = tb_ini_int(template.start, NULL, "layer", 0);
        int variant = tb_ini_int(template.start, NULL, "variant", -1);

        /* Load Template */
        if (!SceneLoadTemplate(scene, entity, path, pos, z_index, variant))
        {
            MINIMAL_WARN("[Scenes] Failed to load template %s", path);
        }
    }

    return 1;
}
