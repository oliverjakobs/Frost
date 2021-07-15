#include "Frost.h"

#include "ECS/EcsLoader.h"

#include "toolbox/tb_ini.h"
#include "toolbox/tb_json.h"
#include "toolbox/tb_file.h"

static void FrostIgnisErrorCallback(ignisErrorLevel level, const char* desc)
{
    switch (level)
    {
    case IGNIS_WARN:		MINIMAL_WARN("%s", desc); break;
    case IGNIS_ERROR:		MINIMAL_ERROR("%s", desc); break;
    case IGNIS_CRITICAL:	MINIMAL_CRITICAL("%s", desc); break;
    }
}

int FrostLoadIgnis(char* config, IgnisColorRGBA clear_color, GLenum blend_s, GLenum blend_d)
{
    /* ingis initialization */
    ignisSetAllocator(FrostMemoryGetAllocator(), tb_mem_malloc, tb_mem_realloc, tb_mem_free);
    ignisSetErrorCallback(FrostIgnisErrorCallback);

    int debug = 0;
#ifdef _DEBUG
    debug = 1;
#endif

    if (!ignisInit(debug))
    {
        MINIMAL_ERROR("[IGNIS] Failed to initialize Ignis");
        return MINIMAL_FAIL;
    }

    ignisEnableBlend(blend_s, blend_d);
    ignisSetClearColor(clear_color);

    tb_ini_element section;
    char vert[APPLICATION_PATH_LEN];
    char frag[APPLICATION_PATH_LEN];

    /* renderer2D */
    tb_ini_query(config, "renderer", NULL, &section);
    tb_ini_query_string(section.start, NULL, "vert", vert, APPLICATION_PATH_LEN);
    tb_ini_query_string(section.start, NULL, "frag", frag, APPLICATION_PATH_LEN);
    Renderer2DInit(vert, frag);

    /* primitives2D */
    tb_ini_query(config, "renderer.primitives", NULL, &section);
    tb_ini_query_string(section.start, NULL, "vert", vert, APPLICATION_PATH_LEN);
    tb_ini_query_string(section.start, NULL, "frag", frag, APPLICATION_PATH_LEN);
    Primitives2DInit(vert, frag);

    /* batchrenderer2D */
    tb_ini_query(config, "renderer.batch", NULL, &section);
    tb_ini_query_string(section.start, NULL, "vert", vert, APPLICATION_PATH_LEN);
    tb_ini_query_string(section.start, NULL, "frag", frag, APPLICATION_PATH_LEN);
    BatchRenderer2DInit(vert, frag);

    /* fontrenderer */
    tb_ini_query(config, "renderer.font", NULL, &section);
    tb_ini_query_string(section.start, NULL, "vert", vert, APPLICATION_PATH_LEN);
    tb_ini_query_string(section.start, NULL, "frag", frag, APPLICATION_PATH_LEN);
    FontRendererInit(vert, frag);

    return MINIMAL_OK;
}

int FrostLoadGui(char* config, float w, float h)
{
    if (!GuiInit(w, h, FrostMemoryGetAllocator())) return MINIMAL_FAIL;

    tb_ini_element font;
    while ((config = tb_ini_query_group(config, "font", &font)) != NULL)
    {
        char font_name[APPLICATION_STR_LEN];
        tb_ini_name(&font, font_name, APPLICATION_STR_LEN);

        char font_path[APPLICATION_PATH_LEN];
        tb_ini_query_string(font.start, NULL, "path", font_path, APPLICATION_STR_LEN);

        float font_size = tb_ini_query_float(font.start, NULL, "size", 0.0f);
        GuiAddFont(font_name, font_path, font_size);
    }

    return MINIMAL_OK;
}

int FrostLoad(MinimalApp* app, const char* path)
{
    char* config = tb_file_read_alloc(path, "rb", FrostMalloc, FrostFree);

    if (!config)
    {
        MINIMAL_ERROR("Failed to read config (%s)", path);
        return MINIMAL_FAIL;
    }

    tb_ini_element section;
    tb_ini_query(config, "app", NULL, &section);

    if (section.error != TB_INI_OK)
    {
        MINIMAL_ERROR("Failed to parse config (%s)", tb_ini_get_error_desc(section.error));
        MinimalFree(config);
        return MINIMAL_FAIL;
    }

    char title[APPLICATION_STR_LEN];
    tb_ini_query_string(section.start, NULL, "title", title, APPLICATION_STR_LEN);

    int w = tb_ini_query_int(section.start, NULL, "width", 0);
    int h = tb_ini_query_int(section.start, NULL, "height", 0);

    char gl_version[APPLICATION_VER_STR_LEN];
    tb_ini_query_string(section.start, NULL, "opengl", gl_version, APPLICATION_VER_STR_LEN);

    MinimalSetAllocator(FrostMemoryGetAllocator(), tb_mem_malloc, tb_mem_free);
    MinimalLoadCB load = MinimalSetLoadCallback(app, NULL);
    if (!MinimalLoad(app, title, w, h, gl_version))
    {
        MINIMAL_ERROR("Failed to load minimal");
        MinimalFree(config);
        return MINIMAL_FAIL;
    }

    /* load ignis */
    FrostLoadIgnis(config, IGNIS_DARK_GREY, GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    /* init gui manager */
    FrostLoadGui(config, (float)w, (float)h);

    int status = load ? load(app, w, h) : MINIMAL_OK;

    /* apply settings */
    tb_ini_query(config, "options", NULL, &section);
    if (section.error == TB_INI_OK)
    {
        MinimalEnableDebug(app, tb_ini_query_bool(section.start, NULL, "debug", 0));
        MinimalEnableVsync(app, tb_ini_query_bool(section.start, NULL, "vsync", 0));
    }

    FrostFree(config);

    MINIMAL_INFO("[Minimal] Version: %s",     MinimalGetVersionString());
    MINIMAL_INFO("[OpenGL] Version: %s",      ignisGetGLVersion());
    MINIMAL_INFO("[OpenGL] Vendor: %s",       ignisGetGLVendor());
    MINIMAL_INFO("[OpenGL] Renderer: %s",     ignisGetGLRenderer());
    MINIMAL_INFO("[OpenGL] GLSL Version: %s", ignisGetGLSLVersion());
    MINIMAL_INFO("[Ignis] Version: %s",       ignisGetVersionString());
    
    return status;
}

void FrostDestroy(MinimalApp* app)
{
    FontRendererDestroy();
    Primitives2DDestroy();
    BatchRenderer2DDestroy();
    Renderer2DDestroy();

    GuiDestroy();
}

int FrostLoadScene(Scene* scene, float w, float h, const char* start)
{
    SceneInit(scene, (vec2) { w, h }, SceneLoad, NULL, FrostMemoryGetAllocator());
    FrostLoadEcs(&scene->ecs);

    glViewport(0, 0, (GLsizei)w, (GLsizei)h);

    SceneChangeActive(scene, start, 0);

    return 1;
}

EcsDataComponentLoader data_comps[] =
{
    [COMPONENT_TRANSFORM] =     { 0, sizeof(Transform),         NULL },
    [COMPONENT_RIGID_BODY] =    { 0, sizeof(RigidBody),         NULL },
    [COMPONENT_STATE] =         { 0, sizeof(EntityState),       NULL },
    [COMPONENT_MOVEMENT] =      { 0, sizeof(Movement),          NULL },
    [COMPONENT_SPRITE] =        { 0, sizeof(Sprite),            NULL },
    [COMPONENT_ANIMATOR] =      { 0, sizeof(Animator),          AnimatorRelease },
    [COMPONENT_CAMERA] =        { 0, sizeof(CameraController),  NULL },
    [COMPONENT_PLAYER] =        { 0, sizeof(Player),            NULL },
    [COMPONENT_INVENTORY] =     { 0, sizeof(Inventory),         InventoryRelease },
    [COMPONENT_INTERACTABLE] =  { 0, sizeof(Interactable),      NULL },
    [COMPONENT_ITEM] =          { 0, sizeof(Item),              NULL }
};

EcsOrderComponentLoader order_comps[] =
{
    [COMPONENT_TEMPLATE] =  { 0, sizeof(Template),  TemplateRelease,    NULL },
    [COMPONENT_Z_INDEX] =   { 0, sizeof(ZIndex),    NULL,               ZIndexCmp }
};

EcsUpdateSystem update_systems[] =
{
    [UPDATE_STATE] =        { EntityStateSystem },
    [UPDATE_PHYSICS] =      { PhysicsSystem },
    [UPDATE_PLAYER] =       { PlayerSystem },
    [UPDATE_ANIMATION] =    { AnimationSystem },
    [UPDATE_INVENTORY] =    { InventoryUpdateSystem },
    [UPDATE_INTERACTION] =  { InteractionSystem }
};

EcsRenderSystem render_systems[] =
{
    [RENDER_SPRITE] =       { ECS_RENDER_STAGE_PRIMARY, RenderSystem },
    [RENDER_DEBUG] =        { ECS_RENDER_STAGE_DEBUG,   DebugRenderSystem },
    [RENDER_INVENTORY] =    { ECS_RENDER_STAGE_UI,      InventoryRenderSystem }
};

int FrostLoadEcs(Ecs* ecs)
{
    size_t update_size = ECS_SIZEOF_LOADER(update_systems);
    size_t render_size = ECS_SIZEOF_LOADER(render_systems);

    size_t data_size = ECS_SIZEOF_LOADER(data_comps);
    size_t order_size = ECS_SIZEOF_LOADER(order_comps);

    EcsReserveSystems(ecs, update_size, render_size);
    EcsLoadUpdateSystems(ecs, update_systems, update_size);
    EcsLoadRenderSystems(ecs, render_systems, render_size);

    EcsReserveComponents(ecs, data_size, order_size);
    EcsLoadDataComponents(ecs, data_comps, data_size);
    EcsLoadOrderComponents(ecs, order_comps, order_size);

    return MINIMAL_OK;
}

