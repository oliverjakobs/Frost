#include "Frost.h"

#include "Application/Logger.h"

#include "toolbox/tb_ini.h"
#include "toolbox/tb_json.h"
#include "toolbox/tb_file.h"

static void FrostIgnisErrorCallback(ignisErrorLevel level, const char* desc)
{
	switch (level)
	{
	case IGNIS_WARN:		DEBUG_WARN("%s", desc); break;
	case IGNIS_ERROR:		DEBUG_ERROR("%s", desc); break;
	case IGNIS_CRITICAL:	DEBUG_CRITICAL("%s", desc); break;
	}
}

int FrostLoadIgnis(IgnisColorRGBA clear_color, GLenum blend_s, GLenum blend_d)
{
	/* ingis initialization */
	ignisSetErrorCallback(FrostIgnisErrorCallback);

	int debug = 0;
#ifdef _DEBUG
	debug = 1;
#endif

	if (!ignisInit(debug))
	{
		DEBUG_ERROR("[IGNIS] Failed to initialize Ignis");
		glfwTerminate();
		return 0;
	}

	ignisEnableBlend(blend_s, blend_d);
	ignisSetClearColor(clear_color);

	DEBUG_INFO("[OpenGL] Version: %s", ingisGetGLVersion());
	DEBUG_INFO("[OpenGL] Vendor: %s", ingisGetGLVendor());
	DEBUG_INFO("[OpenGL] Renderer: %s", ingisGetGLRenderer());
	DEBUG_INFO("[OpenGL] GLSL Version: %s", ingisGetGLSLVersion());

	return 1;
}

int FrostLoadScene(Scene* scene, const char* path, float w, float h)
{
	SceneInit(scene, (vec2) { w, h }, SceneLoad, NULL, LoadEcs);

	char* config = tb_file_read(path, "rb", NULL);

	tb_ini_element section;
	char* scenes = tb_ini_query(config, "scenes", &section);

	for (size_t i = 0; i < section.len; ++i)
	{
		tb_ini_element element;
		scenes = tb_ini_query_section(scenes, "", &element);

		char scene_name[APPLICATION_STR_LEN];
		tb_ini_get_name(&element, scene_name, APPLICATION_STR_LEN);

		char scene_path[APPLICATION_PATH_LEN];
		tb_ini_to_string(&element, scene_path, APPLICATION_STR_LEN);

		if (!SceneRegisterScene(scene, scene_name, scene_path))
			DEBUG_WARN("[Scenes] Failed to add scene: %s (%s)", scene_name, scene_path);
	}

	free(config);

	SceneChangeActive(scene, "scene", 0);

	return 1;
}

int FrostLoadRenderer(const char* path)
{
	char* config = tb_file_read(path, "rb", NULL);

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

	free(config);

	return 1;
}


