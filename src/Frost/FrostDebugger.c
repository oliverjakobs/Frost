#include "FrostDebugger.h"

void FrostDebuggerInit(FrostDebugger* debugger, int show_info, IgnisFont* font)
{
	debugger->show_info = show_info;
	ConsoleInit(&debugger->console, debugger, FrostExecuteConsoleCommand, font);
}

void FrostDebuggerBindScene(FrostDebugger* debugger, Scene* scene, SceneEditor* editor)
{
	debugger->scene = scene;
	debugger->editor = editor;
}

void ForstDebuggerShowInfo(FrostDebugger* debugger, int b)	{ debugger->show_info = b; }
void FrostDebuggerToggleInfo(FrostDebugger* debugger)		{ debugger->show_info = !debugger->show_info; }

MinimalBool FrostDebuggerOnEvent(FrostDebugger* debugger, const MinimalEvent* e)
{
	return ConsoleOnEvent(&debugger->console, e);
}

void FrostDebuggerOnUpdate(FrostDebugger* debugger, float deltatime)
{
	ConsoleOnUpdate(&debugger->console, deltatime);
}

static void FrostDebugRenderInfo(const FrostDebugger* debugger, const Scene* scene, float x, float y)
{
	FontRendererTextFieldBegin(x, y, 8.0f);

	FontRendererTextFieldLine("Memory: %llu bytes", FrostMemoryGetBytes());
	// FontRendererTextFieldLine("Peak: %llu bytes",		FrostMemGetPeak());
	FontRendererTextFieldLine("------------------------");

	FontRendererTextFieldLine("Scene: %s", scene->path);
	FontRendererTextFieldLine("Size: %4.2f, %4.2f", SceneGetWidth(scene), SceneGetHeight(scene));
	FontRendererTextFieldLine("Gravity: %4.2f, %4.2f", scene->gravity.x, scene->gravity.y);
	FontRendererTextFieldLine("------------------------");

	EcsEntityID player = 1;
	FontRendererTextFieldLine("Player ID: %d", player);
	FontRendererTextFieldLine("State: %s", EntityGetStateString(&scene->ecs, player));
	vec2 position = GetEntityPosition(&scene->ecs, player);
	FontRendererTextFieldLine("Position: %4.2f, %4.2f", position.x, position.y);
	FontRendererTextFieldLine("Precise Y: %f", position.y);
	FontRendererTextFieldLine("------------------------");

	FontRendererTextFieldLine("Camera:");
	FontRendererTextFieldLine("Position: %4.2f, %4.2f", scene->camera.position.x, scene->camera.position.y);
}

void FrostDebuggerOnRenderUI(FrostDebugger* debugger, const MinimalApp* app)
{
	FontRendererStart(GuiGetScreenProjPtr());

	/* fps */
	FontRendererRenderTextFormat(8.0f, 8.0f, "FPS: %d", app->timer.fps);

	if (debugger->show_info)
	{
		/* Settings */
		FontRendererTextFieldBegin(GuiGetScreenWidth() - 220.0f, 8.0f, 8.0f);

		FontRendererTextFieldLine("F1: Toggle edit mode");
		FontRendererTextFieldLine("F2: Toggle map edit mode");
		FontRendererTextFieldLine("F3: Open console");
		FontRendererTextFieldLine("F6: Toggle Vsync");
		FontRendererTextFieldLine("F7: Toggle debug mode");
		FontRendererTextFieldLine("F8: Toggle editor grid");
		FontRendererTextFieldLine("F9: Toggle overlay");

		/* Debug info */
		FrostDebugRenderInfo(debugger, debugger->scene, GuiGetScreenWidth() - 480.0f, 8.0f);
	}

	FontRendererFlush();

	ConsoleRender(&debugger->console, 0.0f, GuiGetScreenHeight(), GuiGetScreenWidth(), 32.0f, 8.0f, GuiGetScreenProjPtr());
}

typedef enum
{
	CONSOLE_CMD_NONE = -1,
	CONSOLE_CMD_CHANGE,
	CONSOLE_CMD_CREATE,
	CONSOLE_CMD_LIST,
	CONSOLE_CMD_REMOVE,
	CONSOLE_CMD_SAVE,
	CONSOLE_CMD_RELOAD
} ConsoleCmd;

static ConsoleCmd _CmdGetType(const char* buffer)
{
	ConsoleCmd cmd = CONSOLE_CMD_NONE;
	size_t cmd_length = strlen(buffer);

	switch (buffer[0])
	{
	case 'c':
		if (cmd_length > 1)
			switch (buffer[1])
			{
			case 'h': cmd = ConsoleCheckCMDKeyword(buffer, "change", CONSOLE_CMD_CHANGE); break;
			case 'r': cmd = ConsoleCheckCMDKeyword(buffer, "create", CONSOLE_CMD_CREATE); break;
			}
		break;
	case 'l': cmd = ConsoleCheckCMDKeyword(buffer, "list", CONSOLE_CMD_LIST); break;
	case 'r':
		if (cmd = ConsoleCheckCMDKeyword(buffer, "remove", CONSOLE_CMD_REMOVE)) break;
		if (cmd = ConsoleCheckCMDKeyword(buffer, "reload", CONSOLE_CMD_RELOAD)) break;
		break;
	case 's': cmd = ConsoleCheckCMDKeyword(buffer, "save", CONSOLE_CMD_SAVE); break;
	}

	return cmd;
}

void FrostExecuteConsoleCommand(Console* console, FrostDebugger* debugger, const char* cmd_buffer)
{
	Scene* scene = debugger->scene;
	SceneEditor* editor = debugger->editor;

	ConsoleCmd cmd = _CmdGetType(cmd_buffer);

	switch (cmd)
	{
	case CONSOLE_CMD_CHANGE:
	{
		char* args[1];
		char* spec = ConsoleGetCMDArgs(cmd_buffer, 6, args, 1);

		if (!spec) break;

		if (strcmp(spec, "scene") == 0)
		{
			SceneChangeActive(scene, args[0], 0);
			SceneEditorReset(editor);
			ConsoleOut(console, "Changed Scene to %s", args[0]);
		}
		break;
	}
	case CONSOLE_CMD_RELOAD:
	{
		char* spec = ConsoleGetCMDArgs(cmd_buffer, 6, NULL, 0);
		if (!spec) break;

		if (strcmp(spec, "scene") == 0)
		{
			SceneChangeActive(scene, scene->path, 1);
			SceneEditorReset(editor);
			ConsoleOut(console, "Reloaded Scene.");
		}
		break;
	}
	case CONSOLE_CMD_CREATE:
	{
		char* args[3];
		char* spec = ConsoleGetCMDArgs(cmd_buffer, 6, args, 3);

		if (!spec) break;

		if (strcmp(spec, "entity") == 0)
		{
			if (!SceneEditorIsActive(editor))
			{
				ConsoleOut(console, "Editmode needs to be active to create an entity.");
				break;
			}

			vec2 mouse = { 0 };
			MinimalGetCursorPos(&mouse.x, &mouse.y);
			vec2 pos = CameraGetMousePosView(&scene->camera, mouse);

			if (SceneLoadTemplate(scene, atoi(args[0]), args[1], pos, atoi(args[2]), 0))
				ConsoleOut(console, "Created entity with template %s", args[1]);
		}
		break;
	}
	case CONSOLE_CMD_LIST:
	{
		char* spec = ConsoleGetCMDArgs(cmd_buffer, 4, NULL, 0);

		if (!spec) break;

		ConsoleOut(console, "%s", cmd_buffer);

		if (strcmp(spec, "entities") == 0)
		{
			EcsList* list = EcsGetComponentList(&scene->ecs, COMPONENT_TEMPLATE);
			for (size_t index = 0; index < EcsListSize(list); ++index)
			{
				const char** templ = EcsListComponentAt(list, index);
				ConsoleOut(console, " - %d \t | %s", EcsListEntityAt(list, index), *templ);
			}
		}
		/*
		else if (strcmp(spec, "fonts") == 0)
		{
			for (tb_hashmap_iter* iter = tb_hashmap_iterator(&scene->resources->fonts); iter; iter = tb_hashmap_iter_next(&scene->resources->fonts, iter))
			{
				ConsoleOut(console, " - %s", tb_hashmap_iter_get_key(iter));
			}
		}
		*/
		else if (strcmp(spec, "res") == 0)
		{
			for (tb_hashmap_iter* iter = tb_hashmap_iterator(&scene->res.textures); iter; iter = tb_hashmap_iter_next(&scene->res.textures, iter))
			{
				ConsoleOut(console, " - %s", tb_hashmap_iter_get_key(iter));
			}
		}
		break;
	}
	case CONSOLE_CMD_REMOVE:
		ConsoleOut(console, " > remove");
		break;
	case CONSOLE_CMD_SAVE:
	{
		char* spec = ConsoleGetCMDArgs(cmd_buffer, 4, NULL, 0);

		if (!spec) break;

		if (strcmp(spec, "scene") == 0)
		{
			SceneSave(scene, scene->path);
			ConsoleOut(console, "Saved scene to %s", scene->path);
		}
		break;
	}
	default:
		ConsoleOut(console, "Unkown command: %s", cmd_buffer);
		break;
	}

	ConsoleToggleFocus(console);
}