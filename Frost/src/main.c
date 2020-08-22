#include "Scenes/SceneManager.h"
#include "Scenes/SceneLoader.h"
#include "Scenes/SceneEditor.h"

#include "Console/Console.h"
#include "Console/Command.h"

#include "Inventory/Inventory.h"

Camera camera;
SceneManager scene_manager;
SceneEditor editor;
Console console;

int show_overlay;

Inventory inv;

void FrostExecuteConsoleCommand(Console* console, SceneManager* manager, SceneEditor* editor, char* cmd_buffer);

void OnInit(Application* app)
{
	/* ---------------| Config |------------------------------------------ */
	ignisEnableBlend(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	ignisSetClearColor((IgnisColorRGBA){ 0.2f, 0.2f, 0.2f, 1.0f });

	Renderer2DInit("res/shaders/renderer2D.vert", "res/shaders/renderer2D.frag");
	Primitives2DInit("res/shaders/primitives.vert", "res/shaders/primitives.frag");
	BatchRenderer2DInit("res/shaders/batchrenderer.vert", "res/shaders/batchrenderer.frag");
	FontRendererInit("res/shaders/font.vert", "res/shaders/font.frag");

	FontRendererBindFontColor(ResourceManagerGetFont(&app->resources, "gui"), IGNIS_WHITE);

	ApplicationEnableDebugMode(app, 1);
	ApplicationEnableVsync(app, 0);

	show_overlay = 1;

	CameraCreateOrtho(&camera, app->width / 2.0f, app->height / 2.0f, 0.0f, (float)app->width, (float)app->height);

	SceneManagerInit(&scene_manager, "res/templates/register.json", &app->resources, &camera, 32.0f, 4);
	SceneManagerChangeScene(&scene_manager, "scene");

	SceneEditorInit(&editor, 400.0f, 32.0f, 4);
	SceneEditorToggleActive(&editor);

	ConsoleInit(&console, ResourceManagerGetFont(scene_manager.resources, "gui"));

	InventoryInit(&inv, (vec2) { 0.0f, -camera.size.y / 2.0f }, 2, 9, 64.0f, 8.0f);
	inv.pos.x -= inv.size.x / 2.0f;
}

void OnDestroy(Application* app)
{
	SceneManagerDestroy(&scene_manager);

	FontRendererDestroy();
	Primitives2DDestroy();
	BatchRenderer2DDestroy();
	Renderer2DDestroy();
}

void OnEvent(Application* app, Event e)
{
	if (e.type == EVENT_WINDOW_RESIZE)
	{
		ApplicationSetViewport(app, 0, 0, e.window.width, e.window.height);
		CameraSetProjectionOrtho(&camera, (float)e.window.width, (float)e.window.height);
	}

	switch (EventKeyPressed(&e))
	{
	case KEY_ESCAPE:
		ApplicationClose(app);
		break;
	case KEY_F1:
		SceneEditorToggleActive(&editor);
		break;
	case KEY_F2:
		ConsoleToggleFocus(&console);
		break;
	case KEY_F5: 
		ApplicationPause(app);
		break;
	case KEY_F6:
		ApplicationToggleVsync(app);
		break;
	case KEY_F7:
		ApplicationToggleDebugMode(app);
		break;
	case KEY_F8:
		show_overlay = !show_overlay;
		break;
	}

	if (e.type == EVENT_CONSOLE_EXEC)
		FrostExecuteConsoleCommand(&console, &scene_manager, &editor, e.console.cmd);

	ConsoleOnEvent(&console, &e);

	SceneManagerOnEvent(&scene_manager, e);

	SceneEditorOnEvent(&editor, &scene_manager, e);
}

void OnUpdate(Application* app, float deltatime)
{
	if (!(editor.active || console.focus))
		SceneManagerOnUpdate(&scene_manager, deltatime);

	ConsoleOnUpdate(&console, deltatime);
	SceneEditorOnUpdate(&editor, &scene_manager, deltatime);

	InventoryUpdate(&inv, &camera, deltatime);
}

void OnRender(Application* app)
{
	SceneManagerOnRender(&scene_manager);

	SceneEditorOnRender(&editor, &scene_manager);

	InventoryRender(&inv, vec2_zero(), camera.projection);
}

void OnRenderDebug(Application* app)
{
	if (!editor.active)
		SceneManagerOnRenderDebug(&scene_manager);

	FontRendererStart(ApplicationGetScreenProjPtr(app));
	
	/* fps */
	FontRendererRenderTextFormat(8.0f, 20.0f, "FPS: %d", app->timer.fps);

	if (show_overlay)
	{
		/* Settings */
		FontRendererTextFieldBegin(app->width - 220.0f, 0.0f, 24.0f);

		FontRendererTextFieldLine("F1: Toggle edit mode");
		FontRendererTextFieldLine("F2: Open console");
		FontRendererTextFieldLine("F5: Pause/Unpause");
		FontRendererTextFieldLine("F6: Toggle Vsync");
		FontRendererTextFieldLine("F7: Toggle debug mode");
		FontRendererTextFieldLine("F8: Toggle overlay");

		/* Debug */
		FontRendererTextFieldBegin(app->width - 470.0f, 0.0f, 24.0f);

		FontRendererTextFieldLine("Scene: %s", scene_manager.scene_name);
		FontRendererTextFieldLine("------------------------");

		EntityID player = 0;
		FontRendererTextFieldLine("Player ID: %d", player);
		vec2 position = EcsGetEntityPosition(&scene_manager.ecs, player);
		FontRendererTextFieldLine("Position: %4.2f, %4.2f", position.x, position.y);
		FontRendererTextFieldLine("Precise Y: %f", position.y);
	}

	FontRendererFlush();

	ConsoleRender(&console, 0.0f, (float)app->height, (float)app->width, 32.0f, 8.0f, ApplicationGetScreenProjPtr(app));
}

void FrostExecuteConsoleCommand(Console* console, SceneManager* manager, SceneEditor* editor, char* cmd_buffer)
{
	console_cmd cmd = cmd_get_type(cmd_buffer);

	switch (cmd)
	{
	case CONSOLE_CMD_CHANGE:
	{
		char* args[1];
		char* spec = cmd_get_args(cmd_buffer, 6, args, 1);

		if (!spec) break;

		if (strcmp(spec, "scene") == 0)
		{
			SceneManagerChangeScene(manager, args[0]);
			SceneEditorReset(editor);
			ConsoleOut(console, "Changed Scene to %s", args[0]);
		}
		break;
	}
	case CONSOLE_CMD_CREATE:
	{
		char* args[2];
		char* spec = cmd_get_args(cmd_buffer, 6, args, 2);

		if (!spec) break;

		if (strcmp(spec, "entity") == 0)
		{
			if (!editor->active)
			{
				ConsoleOut(console, "Editmode needs to be active to create an entity.");
				break;
			}

			vec2 pos = CameraGetMousePosView(manager->camera, InputMousePositionVec2());

			if (SceneLoaderLoadTemplate(manager, args[0], EntityGetNextID(), pos, atoi(args[1])))
				ConsoleOut(console, "Created entity with template %s", args[0]);
		}
		break;
	}
	case CONSOLE_CMD_LIST:
	{
		char* spec = cmd_get_args(cmd_buffer, 4, NULL, 0);

		if (!spec) break;

		ConsoleOut(console, "%s", cmd_buffer);

		if (strcmp(spec, "scenes") == 0)
		{
			CLIB_STRMAP_ITERATE_FOR(&manager->scenes)
			{
				const char* name = clib_strmap_iter_get_key(iter);

				ConsoleOut(console, " - %s %s", name, (strcmp(name, manager->scene_name) == 0) ? "(active)" : "");
			}
		}
		else if (strcmp(spec, "entities") == 0)
		{
			for (size_t i = 0; i < EcsGetComponentList(&manager->ecs, COMPONENT_TEMPLATE)->list.used; ++i)
			{
				Template* templ = EcsGetOrderComponent(&manager->ecs, i, COMPONENT_TEMPLATE);

				if (templ)
					ConsoleOut(console, " - %d \t | %s", *(EntityID*)templ, templ->templ);
			}
		}
		else if (strcmp(spec, "templates") == 0)
		{
			CLIB_STRMAP_ITERATE_FOR(&manager->templates)
			{
				const char* name = clib_strmap_iter_get_key(iter);
				char* templ = clib_strmap_iter_get_value(iter);

				ConsoleOut(console, " - %s: %s", name, templ);
			}
		}
		else if (strcmp(spec, "res") == 0)
		{
			ConsoleOut(console, "Textures:");
			CLIB_DICT_ITERATE_FOR(&manager->resources->textures, iter)
			{
				ConsoleOut(console, " - %s", clib_dict_iter_get_key(iter));
			}

			ConsoleOut(console, "Fonts:");
			CLIB_DICT_ITERATE_FOR(&manager->resources->fonts, iter)
			{
				ConsoleOut(console, " - %s", clib_dict_iter_get_key(iter));
			}
		}
		break;
	}
	case CONSOLE_CMD_REMOVE:
		ConsoleOut(console, " > remove");
		break;
	case CONSOLE_CMD_SAVE:
	{
		char* spec = cmd_get_args(cmd_buffer, 4, NULL, 0);

		if (!spec) break;

		if (strcmp(spec, "scene") == 0)
		{
			char* path = clib_strmap_find(&manager->scenes, manager->scene_name);
			if (!path)
			{
				ConsoleOut(console, "Couldn't find path for %s", manager->scene_name);
				break;
			}

			SceneLoaderSaveScene(manager, path);
			ConsoleOut(console, "Saved scene (%s) to %s", manager->scene_name, path);
		}

		break;
	}
	default:
		ConsoleOut(console, "Unkown command");
		break;
	}

	ConsoleToggleFocus(console);
}

int main()
{
	Application app;

	ApplicationSetOnInitCallback(&app, OnInit);
	ApplicationSetOnDestroyCallback(&app, OnDestroy);
	ApplicationSetOnEventCallback(&app, OnEvent);
	ApplicationSetOnUpdateCallback(&app, OnUpdate);
	ApplicationSetOnRenderCallback(&app, OnRender);
	ApplicationSetOnRenderDebugCallback(&app, OnRenderDebug);

	ApplicationLoadConfig(&app, "config.json");

	ApplicationRun(&app);

	ApplicationDestroy(&app);

	return 0;
}