#include "SceneLoader.h"

#include "Memory.h"
#include "FrostParser.h"

#include "toolbox/tb_file.h"
#include "toolbox/tb_ini.h"

static int SceneLoadFromFile(Scene* scene, const char* path, SceneLoadError(*load_func)(Scene*, char*))
{
    char* json = tb_file_read_alloc(path, "rb", FrostMalloc, FrostFree);
    if (!json)
    {
        MINIMAL_ERROR("[Scenes] Failed to read file (%s).", path);
        return 0;
    }

    SceneLoadError error = load_func(scene, json);
    FrostFree(json);

    if (error != SCENE_LOAD_OK)
    {
        MINIMAL_ERROR("[Scenes] %s.\n", SceneLoadErrorDesc(error));
        return 0;
    }

    return 1;
}

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

    tb_ini_element element;
    tb_ini_query(ini, "scene", NULL, &element);

    /* load map */
    char map_path[APPLICATION_PATH_LEN];
    tb_ini_string(element.start, NULL, "map", map_path, APPLICATION_PATH_LEN);

    if (!SceneLoadFromFile(scene, map_path, SceneLoadMap))
    {
        FrostFree(ini);
        return 0;
    }

    scene->gravity.x = 0.0f;
    scene->gravity.y = tb_ini_float(element.start, NULL, "gravity", 0.0f);

    /* load item atlas */
    if ((scene->item_atlas = SceneLoadTexture2D(ini, "item_atlas", &scene->res)) == NULL)
        MINIMAL_WARN("[Scenes] Could not load item atlas.");

    /* load tile set */
    if ((scene->tile_set = SceneLoadTexture2D(ini, "tile_set", &scene->res)) == NULL)
        MINIMAL_WARN("[Scenes] Could not load tile set.");

    /* load background */
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

                float parallax = tb_ini_element_to_float(&layer, 0.0f);
                scene->background = BackgroundPushLayer(scene->background, texture, x, y, w, h, parallax);
            }
        }
    }

    /* load save */
    char save_path[APPLICATION_PATH_LEN];
    tb_ini_string(element.start, NULL, "save", save_path, APPLICATION_PATH_LEN);

    FrostFree(ini);

    return SceneLoadFromFile(scene, save_path, SceneLoadSaveState);
}

int SceneLoadTemplate(Scene* scene, const char* templ, vec2 pos, int z_index, int variant)
{
    char json[FROST_TEMPLATE_SIZE];
    size_t size = tb_file_read_buf(templ, "rb", json, FROST_TEMPLATE_SIZE);
    if (size == FROST_TEMPLATE_SIZE) MINIMAL_WARN("[Scenes] Template buffer may be too small");

    if (size == 0)
    {
        MINIMAL_WARN("[Scenes] Couldn't read template (%s)", templ);
        return 0;
    }

    EcsEntityID entity = EcsEntityGetNextID();

    /* Load components */
    TemplateLoad(&scene->ecs, entity, templ);
    EntityStateLoad(&scene->ecs, entity, ENTITY_STATE_NULL);

    TransformLoad(json, &scene->ecs, entity, pos);
    RigidBodyLoad(json, &scene->ecs, entity);
    SpriteLoad(json, &scene->ecs, entity, &scene->res, z_index, variant);
    AnimatorLoad(json, &scene->ecs, entity);
    MovementLoad(json, &scene->ecs, entity);
    CameraControllerLoad(json, &scene->ecs, entity, scene);
    PlayerLoad(json, &scene->ecs, entity);
    InventoryLoad(json, &scene->ecs, entity, scene->camera.size);
    InteractableLoad(json, &scene->ecs, entity);

    return 1;
}

SceneLoadError SceneLoadTextures(Scene* scene, char* json)
{

    return SCENE_LOAD_OK;
}

static void* SceneStreamTiles(void* stream, TileID* id)
{
    tb_ini_element element;
    stream = tb_ini_csv_step(stream, &element);
    *id = atoi(element.start);
    return stream;
}

static void* SceneStreamTileTypes(void* stream, TileType* type)
{
    tb_ini_element element;
    stream = tb_ini_csv_step(stream, &element);
    *type = FrostParseTileType(element.start, element.len);
    return stream;
}

SceneLoadError SceneLoadMap(Scene* scene, char* ini)
{
    tb_ini_element map;
    tb_ini_query(ini, "map", NULL, &map);

    size_t rows = tb_ini_int(map.start, NULL, "rows", 0);
    size_t cols = tb_ini_int(map.start, NULL, "cols", 0);

    float tile_size = tb_ini_float(map.start, NULL, "tile", 0.0f);
     
    if (!TileMapLoad(&scene->map, rows, cols, tile_size, scene->allocator)) return SCENE_LOAD_MAP_ERROR;

    tb_ini_element types;
    tb_ini_csv(map.start, NULL, "types", &types);

    if (types.error != TB_INI_OK || !TileMapStreamTypes(&scene->map, types.start, SceneStreamTileTypes, types.len))
    {
        TileMapDestroy(&scene->map);
        return SCENE_LOAD_STREAM_TYPES_ERROR;
    }

    tb_ini_element tiles;
    tb_ini_csv(ini, "tiles", "layer0", &tiles);

    if (types.error != TB_INI_OK || !TileMapStreamTiles(&scene->map, tiles.start, SceneStreamTiles, tiles.len))
    {
        TileMapDestroy(&scene->map);
        return SCENE_LOAD_STREAM_MAP_ERROR;
    }

    TileRendererBindMap(&scene->renderer, &scene->map);

    TileMapSetBorder(&scene->map, TILE_BOTTOM, 1);
    TileMapSetBorder(&scene->map, TILE_LEFT, 1);
    TileMapSetBorder(&scene->map, TILE_RIGHT, 1);

    return SCENE_LOAD_OK;
}

SceneLoadError SceneLoadSaveState(Scene* scene, char* ini)
{
    tb_ini_element template;
    while ((ini = tb_ini_group_next(ini, "template", &template)) != NULL)
    {
        char path[APPLICATION_PATH_LEN];
        tb_ini_string(template.start, NULL, "path", path, APPLICATION_PATH_LEN);

        vec2 pos;
        pos.x = tb_ini_float(template.start, NULL, "x", 0.0f);
        pos.y = tb_ini_float(template.start, NULL, "y", 0.0f);

        int z_index = tb_ini_int(template.start, NULL, "layer", 0);
        int variant = tb_ini_int(template.start, NULL, "variant", 0);

        /* Load Template */
        if (!SceneLoadTemplate(scene, path, pos, z_index, variant))
        {
            MINIMAL_WARN("[Scenes] Failed to load template %s", path);
        }
    }

    return SCENE_LOAD_OK;
}

const char* SceneLoadErrorDesc(SceneLoadError error)
{
    switch (error)
    {
    case SCENE_LOAD_OK:					return "No error";
    case SCENE_LOAD_READ_ERROR:			return "Failed to read file";
    case SCENE_LOAD_MAP_ERROR:			return "Failed to load map";
    case SCENE_LOAD_MAP_SIZE_ERROR:		return "Map size or tile size can not be zero";
    case SCENE_LOAD_STREAM_MAP_ERROR:	return "Failed to stream tiles";
    case SCENE_LOAD_STREAM_TYPES_ERROR: return "Failed to stream tile types";
    default:							return "Unkown error";
    }
}
