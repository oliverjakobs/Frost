#include "Frost.h"

#include "Minimal/Application.h"

#include "ECS/EcsLoader.h"

#include "toolbox/tb_ini.h"
#include "toolbox/tb_file.h"

static void FrostIgnisErrorCallback(ignisErrorLevel level, const char* desc)
{
    switch (level)
    {
    case IGNIS_WARN:     MINIMAL_WARN("%s", desc); break;
    case IGNIS_ERROR:    MINIMAL_ERROR("%s", desc); break;
    case IGNIS_CRITICAL: MINIMAL_CRITICAL("%s", desc); break;
    }
}

int FrostLoadGraphics(IgnisColorRGBA clear_color, GLenum blend_s, GLenum blend_d)
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

    /* renderer */
    Renderer2DInit("res/shaders/renderer2D.vert", "res/shaders/renderer2D.frag");
    Primitives2DInit("res/shaders/primitives.vert", "res/shaders/primitives.frag");
    BatchRenderer2DInit("res/shaders/batchrenderer.vert", "res/shaders/batchrenderer.frag");
    FontRendererInit("res/shaders/font.vert", "res/shaders/font.frag");

    return MINIMAL_OK;
}

void FrostDestroyGraphics()
{
    FontRendererDestroy();
    Primitives2DDestroy();
    BatchRenderer2DDestroy();
    Renderer2DDestroy();
}

int FrostLoadGui(uint32_t w, uint32_t h, const char* font)
{
    if (!GuiInit((float)w, (float)h, FrostMemoryGetAllocator()))
    {
        MINIMAL_ERROR("[GUI] Failed to initialize gui");
        return MINIMAL_FAIL;
    }

    GuiAddFont("font", "res/fonts/OpenSans.ttf", 32.0);
    GuiAddFont("gui", "res/fonts/ProggyTiny.ttf", 24.0);

    FontRendererBindFontColor(GuiGetFont(font), IGNIS_WHITE);

    return MINIMAL_OK;
}

void FrostDestroyGui()
{
    GuiDestroy();
}

static void FrostGetGLVersion(const char* version_str, int* major, int* minor)
{
    const char* sep = ".";
    const char* major_str = version_str;
    const char* minor_str = version_str + strcspn(version_str, sep) + 1;

    if (major_str && major) *major = atoi(major_str);
    if (minor_str && minor) *minor = atoi(minor_str);
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
        FrostFree(config);
        return MINIMAL_FAIL;
    }

    char title[APPLICATION_STR_LEN];
    tb_ini_string(section.start, NULL, "title", title, APPLICATION_STR_LEN);

    int w = tb_ini_int(section.start, NULL, "width", 0);
    int h = tb_ini_int(section.start, NULL, "height", 0);

    char gl_version[APPLICATION_VER_STR_LEN];
    tb_ini_string(section.start, NULL, "opengl", gl_version, APPLICATION_VER_STR_LEN);

    int gl_major, gl_minor;
    FrostGetGLVersion(gl_version, &gl_major, &gl_minor);

    /* apply settings */
    tb_ini_query(config, "options", NULL, &section);
    if (section.error == TB_INI_OK)
    {
        app->debug = tb_ini_bool(section.start, NULL, "debug", 0);
        app->vsync = tb_ini_bool(section.start, NULL, "vsync", 0);
    }

    FrostFree(config);

    if (!MinimalLoad(app, title, w, h, gl_major, gl_minor))
    {
        MINIMAL_ERROR("Failed to load minimal");
        return MINIMAL_FAIL;
    }

    MINIMAL_INFO("[GLFW] Version: %s",        glfwGetVersionString());
    MINIMAL_INFO("[OpenGL] Version: %s",      ignisGetGLVersion());
    MINIMAL_INFO("[OpenGL] Vendor: %s",       ignisGetGLVendor());
    MINIMAL_INFO("[OpenGL] Renderer: %s",     ignisGetGLRenderer());
    MINIMAL_INFO("[OpenGL] GLSL Version: %s", ignisGetGLSLVersion());
    MINIMAL_INFO("[Ignis] Version: %s",       ignisGetVersionString());
    
    return MINIMAL_OK;
}

int FrostLoadScene(Scene* scene, uint32_t w, uint32_t h, const char* start)
{
    SceneInit(scene, (float)w, (float)h, SceneLoad, NULL, FrostMemoryGetAllocator());
    FrostLoadEcs(&scene->ecs);

    glViewport(0, 0, w, h);

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

