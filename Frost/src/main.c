#include "Frost/Frost.h"

#include "tile/tile_renderer.h"

Camera camera;

Ecs ecs;
Scene scene;
SceneEditor scene_editor;

Console console;

int show_debug_info;

TileMap map;
TileRenderer renderer;
IgnisTexture2D* tile_tex;

void OnInit(Application* app)
{
	/* ---------------| Config |------------------------------------------ */
	ignisEnableBlend(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	ignisSetClearColor((IgnisColorRGBA){ 0.2f, 0.2f, 0.2f, 1.0f });

	Renderer2DInit("res/shaders/renderer2D.vert", "res/shaders/renderer2D.frag");
	Primitives2DInit("res/shaders/primitives.vert", "res/shaders/primitives.frag");
	BatchRenderer2DInit("res/shaders/batchrenderer.vert", "res/shaders/batchrenderer.frag");
	FontRendererInit("res/shaders/font.vert", "res/shaders/font.frag");

	FontRendererBindFontColor(ResourcesGetFont(&app->resources, "gui"), IGNIS_WHITE);

	ApplicationEnableDebugMode(app, 1);
	ApplicationEnableVsync(app, 0);

	show_debug_info = 1;

	CameraCreateOrtho(&camera, app->width / 2.0f, app->height / 2.0f, 0.0f, (float)app->width, (float)app->height);

	ConsoleInit(&console, ResourcesGetFont(&app->resources, "gui"));
	InventorySystemInit(ResourcesGetTexture2D(&app->resources, "items"), &camera, 64.0f, 8.0f);

	/* ecs */
	SceneInit(&scene, &camera, &app->resources, LoadEcs);
	SceneLoadScenes(&scene, "res/register.json", "scene");
	SceneEditorInit(&scene_editor, 400.0f, 32.0f, 4);
	SceneEditorToggleActive(&scene_editor);

	TileID tiles[] =
	{
		1, 2, 1, 2, 1, 2, 1, 2,
		5, 6, 5, 6, 5, 6, 5, 6,
		3, 3, 3, 3, 3, 3, 3, 3,
		3, 3, 3, 3, 3, 3, 3, 3,
		3, 3, 3, 3, 3, 3, 3, 3,
		3, 3, 3, 3, 3, 3, 3, 3,
		3, 3, 3, 3, 3, 3, 3, 3,
		3, 3, 3, 3, 3, 3, 3, 3,
		3, 3, 3, 3, 3, 3, 3, 3,
	};

	TileMapLoad(&map, tiles, 8, 9, 32.0f);
	TileRendererInit(&renderer, &map);

	tile_tex = ResourcesGetTexture2D(&app->resources, "tiles");
}

void OnDestroy(Application* app)
{
	TileRendererDestroy(&renderer);
	TileMapDestroy(&map);

	SceneDestroy(&scene);

	FontRendererDestroy();
	Primitives2DDestroy();
	BatchRenderer2DDestroy();
	Renderer2DDestroy();
}

void OnEvent(Application* app, Event e)
{
	if (EventCheckType(&e, EVENT_WINDOW_RESIZE))
	{
		ApplicationSetViewport(app, 0, 0, e.window.width, e.window.height);
		CameraSetProjectionOrtho(&camera, (float)e.window.width, (float)e.window.height);
	}

	switch (EventKeyPressed(&e))
	{
	case KEY_ESCAPE:	ApplicationClose(app); break;
	case KEY_F1:		SceneEditorToggleActive(&scene_editor); break;
	case KEY_F2:		ConsoleToggleFocus(&console); break;
	case KEY_F5:		ApplicationPause(app); break;
	case KEY_F6:		ApplicationToggleVsync(app); break;
	case KEY_F7:		ApplicationToggleDebugMode(app); break;
	case KEY_F8:		SceneEditorToggleGrid(&scene_editor); break;
	case KEY_F9:		show_debug_info = !show_debug_info; break;
	}

	if (EventCheckType(&e, EVENT_CONSOLE_EXEC))
		FrostExecuteConsoleCommand(&console, &scene, &scene_editor, e.console.cmd);

	ConsoleOnEvent(&console, &e);

	SceneOnEvent(&scene, e);

	SceneEditorOnEvent(&scene_editor, &scene, e);
}

void OnUpdate(Application* app, float deltatime)
{
	ConsoleOnUpdate(&console, deltatime);

	if (!console.focus)
		SceneEditorOnUpdate(&scene_editor, &scene, deltatime);

	if (!(scene_editor.active || console.focus))
		SceneOnUpdate(&scene, deltatime);
}

void OnRender(Application* app)
{
	SceneOnRender(&scene);
	TileMapRender(&renderer, tile_tex, vec2_zero(), camera.viewProjection);

	SceneEditorOnRender(&scene_editor, &scene);

	SceneOnRenderUI(&scene);
}

void OnRenderDebug(Application* app)
{
	if (!scene_editor.active)
		SceneOnRenderDebug(&scene);

	FontRendererStart(ApplicationGetScreenProjPtr(app));
	
	/* fps */
	FontRendererRenderTextFormat(8.0f, 20.0f, "FPS: %d", app->timer.fps);

	if (show_debug_info)
	{
		/* Settings */
		FontRendererTextFieldBegin(app->width - 220.0f, 0.0f, 24.0f);

		FontRendererTextFieldLine("F1: Toggle edit mode");
		FontRendererTextFieldLine("F2: Open console");
		FontRendererTextFieldLine("F5: Pause/Unpause");
		FontRendererTextFieldLine("F6: Toggle Vsync");
		FontRendererTextFieldLine("F7: Toggle debug mode");
		FontRendererTextFieldLine("F8: Toggle editor grid");
		FontRendererTextFieldLine("F9: Toggle overlay");

		/* Debug */
		FontRendererTextFieldBegin(app->width - 470.0f, 0.0f, 24.0f);

		FontRendererTextFieldLine("Scene: %s", scene.name);
		FontRendererTextFieldLine("Size: %4.2f, %4.2f", scene.width, scene.height);
		FontRendererTextFieldLine("------------------------");

		EcsEntityID player = 0;
		FontRendererTextFieldLine("Player ID: %d", player);
		FontRendererTextFieldLine("State: %s", EntityGetStateString(&scene.ecs, player));
		vec2 position = GetEntityPosition(&scene.ecs, player);
		FontRendererTextFieldLine("Position: %4.2f, %4.2f", position.x, position.y);
		FontRendererTextFieldLine("Precise Y: %f", position.y);

		FontRendererTextFieldLine("");
		
		FontRendererTextFieldLine("Camera:");
		FontRendererTextFieldLine("------------------------");
		FontRendererTextFieldLine("Position: %4.2f, %4.2f", camera.position.x, camera.position.y);
	}

	FontRendererFlush();

	ConsoleRender(&console, 0.0f, (float)app->height, (float)app->width, 32.0f, 8.0f, ApplicationGetScreenProjPtr(app));
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