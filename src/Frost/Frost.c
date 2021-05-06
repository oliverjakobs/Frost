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

int FrostLoadMinimal(MinimalApp* app, const char* path)
{
	char* config = tb_file_read(path, "rb");

	if (!config)
	{
		MINIMAL_ERROR("Failed to read config (%s)", path);
		return 0;
	}

	tb_ini_element section;
	tb_ini_query(config, "app", &section);

	if (section.type == TB_INI_ERROR)
	{
		MINIMAL_ERROR("Failed to parse config (%s)", tb_ini_get_error_desc(section.error));
		return 0;
	}

	char title[APPLICATION_STR_LEN];
	tb_ini_query_string(section.start, ".title", title, APPLICATION_STR_LEN);

	int w = tb_ini_query_int(section.start, ".width", 0);
	int h = tb_ini_query_int(section.start, ".height", 0);

	char gl_version[APPLICATION_VER_STR_LEN];
	tb_ini_query_string(section.start, ".opengl", gl_version, APPLICATION_VER_STR_LEN);

	MinimalBool status = MinimalLoad(app, title, w, h, gl_version);
	if (status)
	{
		/* apply settings */
		tb_ini_query(config, "options", &section);
		if (section.type != TB_INI_ERROR)
		{
			MinimalEnableDebug(app, tb_ini_query_bool(section.start, ".debug", 0));
			MinimalEnableVsync(app, tb_ini_query_bool(section.start, ".vsync", 0));
		}

		MINIMAL_INFO("[Minimal] Version: %s", MinimalGetVersionString());
	}

	free(config);

	return status;
}

int FrostLoadIgnis(IgnisColorRGBA clear_color, GLenum blend_s, GLenum blend_d)
{
	/* ingis initialization */
	ignisSetAllocator(FrostGetAllocator(), tb_mem_malloc, tb_mem_realloc, tb_mem_free);
	ignisSetErrorCallback(FrostIgnisErrorCallback);

	int debug = 0;
#ifdef _DEBUG
	debug = 1;
#endif

	if (!ignisInit(debug))
	{
		MINIMAL_ERROR("[IGNIS] Failed to initialize Ignis");
		return 0;
	}

	ignisEnableBlend(blend_s, blend_d);
	ignisSetClearColor(clear_color);

	MINIMAL_INFO("[OpenGL] Version: %s",		ignisGetGLVersion());
	MINIMAL_INFO("[OpenGL] Vendor: %s",			ignisGetGLVendor());
	MINIMAL_INFO("[OpenGL] Renderer: %s",		ignisGetGLRenderer());
	MINIMAL_INFO("[OpenGL] GLSL Version: %s",	ignisGetGLSLVersion());
	MINIMAL_INFO("[Ignis] Version: %s",			IgnisGetVersionString());

	return 1;
}

int FrostLoadScene(Scene* scene, float w, float h, const char* start)
{
	SceneInit(scene, (vec2) { w, h }, SceneLoad, NULL, FrostGetAllocator());
	FrostLoadEcs(&scene->ecs);

	glViewport(0, 0, (GLsizei)w, (GLsizei)h);

	SceneChangeActive(scene, start, 0);

	return 1;
}

int FrostLoadRenderer(const char* path)
{
	char* config = tb_file_read_alloc(path, "rb", FrostMalloc, FrostFree);

	char vert[APPLICATION_PATH_LEN];
	char frag[APPLICATION_PATH_LEN];

	tb_ini_element section;
	/* renderer2D */
	tb_ini_query(config, "renderer2D", &section);
	tb_ini_query_string(section.start, ".vert", vert, APPLICATION_PATH_LEN);
	tb_ini_query_string(section.start, ".frag", frag, APPLICATION_PATH_LEN);
	Renderer2DInit(vert, frag);

	/* primitives2D */
	tb_ini_query(config, "primitives2D", &section);
	tb_ini_query_string(section.start, ".vert", vert, APPLICATION_PATH_LEN);
	tb_ini_query_string(section.start, ".frag", frag, APPLICATION_PATH_LEN);
	Primitives2DInit(vert, frag);

	/* batchrenderer2D */
	tb_ini_query(config, "batchrenderer2D", &section);
	tb_ini_query_string(section.start, ".vert", vert, APPLICATION_PATH_LEN);
	tb_ini_query_string(section.start, ".frag", frag, APPLICATION_PATH_LEN);
	BatchRenderer2DInit(vert, frag);

	/* fontrenderer */
	tb_ini_query(config, "fontrenderer", &section);
	tb_ini_query_string(section.start, ".vert", vert, APPLICATION_PATH_LEN);
	tb_ini_query_string(section.start, ".frag", frag, APPLICATION_PATH_LEN);
	FontRendererInit(vert, frag);

	FrostFree(config);

	return 1;
}

EcsDataComponentLoader data_comps[] =
{
	[COMPONENT_TRANSFORM] =		{ sizeof(Transform),		NULL },
	[COMPONENT_RIGID_BODY] =	{ sizeof(RigidBody),		NULL },
	[COMPONENT_STATE] =			{ sizeof(EntityState),		NULL },
	[COMPONENT_MOVEMENT] =		{ sizeof(Movement),			NULL },
	[COMPONENT_SPRITE] =		{ sizeof(Sprite),			NULL },
	[COMPONENT_ANIMATOR] =		{ sizeof(Animator),			AnimatorFree },
	[COMPONENT_CAMERA] =		{ sizeof(CameraController),	NULL },
	[COMPONENT_PLAYER] =		{ sizeof(Player),			NULL },
	[COMPONENT_INVENTORY] =		{ sizeof(Inventory),		InventoryFree },
	[COMPONENT_INTERACTABLE] =	{ sizeof(Interactable),		NULL },
	[COMPONENT_ITEM] =			{ sizeof(Item),				NULL }
};

EcsOrderComponentLoader order_comps[] =
{
	[COMPONENT_TEMPLATE] =	{ sizeof(const char*),	NULL,	NULL },
	[COMPONENT_Z_INDEX] =	{ sizeof(ZIndex),		NULL,	ZIndexCmp }
};

EcsUpdateSystem update_systems[] =
{
	[UPDATE_STATE] =		{ EntityStateSystem },
	[UPDATE_PHYSICS] =		{ PhysicsSystem },
	[UPDATE_PLAYER] =		{ PlayerSystem },
	[UPDATE_ANIMATION] =	{ AnimationSystem },
	[UPDATE_INVENTORY] =	{ InventoryUpdateSystem },
	[UPDATE_INTERACTION] =	{ InteractionSystem }
};

EcsRenderSystem render_systems[] =
{
	[RENDER_SPRITE] =		{ ECS_RENDER_STAGE_PRIMARY, RenderSystem },
	[RENDER_DEBUG] =		{ ECS_RENDER_STAGE_DEBUG,	DebugRenderSystem },
	[RENDER_INVENTORY] =	{ ECS_RENDER_STAGE_UI,		InventoryRenderSystem }
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

	return 1;
}
