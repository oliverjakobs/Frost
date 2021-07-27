#include "SceneSaver.h"

#include "Memory.h"
#include "FrostParser.h"

#include "toolbox/tb_file.h"

static int SceneSaveToFile(Scene* scene, FILE* const file, char* ini)
{
    if (!ini) return 0;

    tb_ini_copy_till(file, ini, "map", "tiles");

    return SceneSaveMap(scene, file) && SceneSaveState(scene, file);
}

int SceneSave(Scene* scene, const char* path)
{
    FILE* file = fopen(path, "rb");
    char* ini = tb_file_readf_alloc(file, FrostMalloc, FrostFree);
    file = freopen(path, "wb", file);

    if (!file)
    {
        MINIMAL_ERROR("[Scenes] Failed to open save file (%s)\n", path);
        return 0;
    }

    if (!ini)
    {
        MINIMAL_ERROR("[Scenes] Failed to read save file (%s)\n", path);
        fclose(file);
        return 0;
    }

    int result = SceneSaveToFile(scene, file, ini);
    FrostFree(ini);
    fclose(file);

    if (!result)
    {
        MINIMAL_ERROR("[Scenes] Failed to save scene\n");
        return 0;
    }

    return 1;
}

int SceneSaveMap(Scene* scene, FILE* const stream)
{
    fprintf(stream, "tiles = {");

    size_t rows = scene->map.rows;
    size_t cols = scene->map.cols;

    for (size_t index = 0; index < (rows * cols); ++index)
    {
        if (index % cols == 0) fprintf(stream, TB_INI_NEWLINE "  ");
        fprintf(stream, "%*d", 2, scene->map.tiles[index].id);
        if (index < ((rows * cols) - 1)) fprintf(stream, ",");
    }

    fprintf(stream, TB_INI_NEWLINE "}" TB_INI_NEWLINE);
    return 1;
}

int SceneSaveState(Scene* scene, FILE* const stream)
{
    EcsList* list = EcsGetComponentList(&scene->ecs, COMPONENT_TEMPLATE);
    for (size_t index = 0; index < EcsListSize(list); ++index)
    {
        EcsEntityID entity = EcsListEntityAt(list, index);
        Template* templ = EcsListComponentAt(list, index);
        vec2 pos = GetEntityPosition(&scene->ecs, entity);
        ZIndex layer = EntityGetZIndex(&scene->ecs, entity);

        tb_ini_write_property(stream, NULL, NULL); /* spacer */
        tb_ini_write_section(stream, "template.%d", entity);
        tb_ini_write_property(stream, "path", templ->path);
        tb_ini_write_property(stream, "x", "%.2f", pos.x);
        tb_ini_write_property(stream, "y", "%.2f", pos.y);
        tb_ini_write_property(stream, "layer", "%d", layer);
        if (templ->variant >= 0)
            tb_ini_write_property(stream, "variant", "%d", templ->variant);
    }

    return 1;
}
