#include "SceneSaver.h"

#include "Memory.h"
#include "FrostParser.h"

#include "toolbox/tb_file.h"

static int SceneSaveToFile(Scene* scene, const char* path, SceneSaveError(*save_func)(Scene*, FILE* const))
{
    /*
    FILE* stream = fopen(path, "wb");

    if (!stream)
    {
        MINIMAL_ERROR("[Scenes] Failed to open file %s.\n",path);
        return 0;
    }
    */

    SceneSaveError error = save_func(scene, stdout);

    // fclose(stream);

    if (error != SCENE_SAVE_OK)
    {
        MINIMAL_ERROR("[Scenes] %s.\n", "error");
        return 0;
    }

    return 1;
}

int SceneSave(Scene* scene, const char* path)
{
    char* ini = tb_file_read_alloc(path, "rb", FrostMalloc, FrostFree);

    if (!ini)
    {
        MINIMAL_ERROR("[Scenes] Couldn't read scene template: %s", path);
        return 0;
    }

    /* get map path */
    char map_path[APPLICATION_PATH_LEN];
    tb_ini_string(ini, "scene", "map", map_path, APPLICATION_PATH_LEN);

    /* get save path */
    char save_path[APPLICATION_PATH_LEN];
    tb_ini_string(ini, "scene", "save", save_path, APPLICATION_PATH_LEN);

    FrostFree(ini);

    if (!SceneSaveToFile(scene, map_path, SceneSaveMap)) return 0;
    if (!SceneSaveToFile(scene, save_path, SceneSaveState)) return 0;
    return 1;
}

/* TODO: implement map saving */
SceneSaveError SceneSaveMap(Scene* scene, FILE* const stream)
{
    return SCENE_SAVE_OK;
}

SceneSaveError SceneSaveState(Scene* scene, FILE* const stream)
{
    EcsList* list = EcsGetComponentList(&scene->ecs, COMPONENT_TEMPLATE);
    for (size_t index = 0; index < EcsListSize(list); ++index)
    {
        EcsEntityID entity = EcsListEntityAt(list, index);
        Template* templ = EcsListComponentAt(list, index);
        vec2 pos = GetEntityPosition(&scene->ecs, entity);
        ZIndex layer = EntityGetZIndex(&scene->ecs, entity);
        
        tb_ini_write_section(stream, "template.%d", entity);
        tb_ini_write_property(stream, "path", templ->path);
        tb_ini_write_property(stream, "x", "%.2f", pos.x);
        tb_ini_write_property(stream, "y", "%.2f", pos.y);
        tb_ini_write_property(stream, "layer", "%d", layer);
        if (templ->variant >= 0)
            tb_ini_write_property(stream, "variant", "%d", templ->variant);

        tb_ini_write_property(stream, NULL, NULL);
    }

    return SCENE_SAVE_OK;
}
