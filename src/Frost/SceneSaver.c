#include "SceneSaver.h"

#include "FrostParser.h"
#include "Debug/Memory.h"

#include "toolbox/tb_file.h"

static int SceneSaveToFile(Scene* scene, const char* path, SceneSaveError(*save_func)(Scene*, tb_jwrite_control*))
{
	tb_jwrite_control jwc;
	tb_jwrite_open(&jwc, path, TB_JWRITE_OBJECT, TB_JWRITE_NEWLINE);
	tb_jwrite_set_float_prec(&jwc, 2);

	SceneSaveError error = save_func(scene, &jwc);

	tb_jwrite_error jwrite_error = tb_jwrite_close(&jwc);

	if (jwrite_error != TB_JWRITE_OK)
	{
		MINIMAL_ERROR("[JWRITE] Error: %s at function call %d", tb_jwrite_error_string(jwrite_error), tb_jwrite_error_pos(&jwc));
		return 0;
	}

	if (error != SCENE_LOAD_OK)
	{
		MINIMAL_ERROR("[Scenes] %s.\n", SceneLoadErrorDesc(error));
		return 0;
	}

	return 1;
}

int SceneSave(Scene* scene, const char* path)
{
	char* json = tb_file_read_alloc(path, "rb", DebugMalloc, DebugFree);

	if (!json)
	{
		MINIMAL_ERROR("[Scenes] Couldn't read scene template: %s", path);
		return 0;
	}

	/* get map path */
	char map_path[APPLICATION_PATH_LEN];
	tb_json_string(json, "{'map'", map_path, APPLICATION_PATH_LEN, NULL);

	DebugFree(json);

	if (!SceneSaveToFile(scene, map_path, SceneSaveMap)) return 0;
	return 1;
}

/* TODO: implement map saving */
SceneSaveError SceneSaveMap(Scene* scene, tb_jwrite_control* jwc)
{
	return SCENE_SAVE_OK;
}