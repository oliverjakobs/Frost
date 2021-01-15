#include "Frost/Frost.h"

Scene scene;
SceneEditor scene_editor;

Console console;

int show_debug_info;

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

	ConsoleInit(&console, ResourcesGetFont(&app->resources, "gui"));

	/* ecs */
	SceneInit(&scene, (float)app->width, (float)app->height, &app->resources, LoadEcs);

	InventorySystemInit(ResourcesGetTexture2D(&app->resources, "items"));

	SceneLoadScenes(&scene, "res/register.json", "scene");
	SceneEditorInit(&scene_editor, 400.0f, 32.0f, 4);
	SceneEditorToggleActive(&scene_editor);
}

void OnDestroy(Application* app)
{
	SceneDestroy(&scene);

	FontRendererDestroy();
	Primitives2DDestroy();
	BatchRenderer2DDestroy();
	Renderer2DDestroy();
}

void OnEvent(Application* app, Event e)
{
	if (EventCheckType(&e, EVENT_WINDOW_RESIZE))
		ApplicationSetViewport(app, 0, 0, e.window.width, e.window.height);

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
		FontRendererTextFieldLine("Size: %4.2f, %4.2f", SceneGetWidth(&scene), SceneGetHeight(&scene));
		FontRendererTextFieldLine("Gravity: %4.2f, %4.2f", scene.gravity.x, scene.gravity.y);
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
		FontRendererTextFieldLine("Position: %4.2f, %4.2f", scene.camera.position.x, scene.camera.position.y);
	}

	FontRendererFlush();

	ConsoleRenderBackground(&console, 0.0f, (float)app->height, (float)app->width, 32.0f, ApplicationGetScreenProjPtr(app));
	ConsoleRender(&console, 0.0f, (float)app->height, 8.0f, ApplicationGetScreenProjPtr(app));
}

#include "Frost/FrostParser.h"

#define TEST_MOUSE(str, type) assert(InputParseMouseButton(str, strlen(str)) == type)

/* TODO test keycodes */
void TestParser()
{
	MouseButton mouse = MOUSE_BUTTON_UNKNOWN;

	TEST_MOUSE("MOUSE_BUTTON_1", MOUSE_BUTTON_1);
	TEST_MOUSE("MOUSE_BUTTON_2", MOUSE_BUTTON_2);
	TEST_MOUSE("MOUSE_BUTTON_3", MOUSE_BUTTON_3);
	TEST_MOUSE("MOUSE_BUTTON_4", MOUSE_BUTTON_4);
	TEST_MOUSE("MOUSE_BUTTON_5", MOUSE_BUTTON_5);
	TEST_MOUSE("MOUSE_BUTTON_6", MOUSE_BUTTON_6);
	TEST_MOUSE("MOUSE_BUTTON_7", MOUSE_BUTTON_7);
	TEST_MOUSE("MOUSE_BUTTON_8", MOUSE_BUTTON_8);
	TEST_MOUSE("MOUSE_BUTTON_9", MOUSE_BUTTON_UNKNOWN);

	TEST_MOUSE("mouse_button_1", MOUSE_BUTTON_1);
	TEST_MOUSE("mouse_button_2", MOUSE_BUTTON_2);
	TEST_MOUSE("mouse_button_3", MOUSE_BUTTON_3);
	TEST_MOUSE("mouse_button_4", MOUSE_BUTTON_4);
	TEST_MOUSE("mouse_button_5", MOUSE_BUTTON_5);
	TEST_MOUSE("mouse_button_6", MOUSE_BUTTON_6);
	TEST_MOUSE("mouse_button_7", MOUSE_BUTTON_7);
	TEST_MOUSE("mouse_button_8", MOUSE_BUTTON_8);
	TEST_MOUSE("mouse_button_9", MOUSE_BUTTON_UNKNOWN);

	TEST_MOUSE("1", MOUSE_BUTTON_1);
	TEST_MOUSE("2", MOUSE_BUTTON_2);
	TEST_MOUSE("3", MOUSE_BUTTON_3);
	TEST_MOUSE("4", MOUSE_BUTTON_4);
	TEST_MOUSE("5", MOUSE_BUTTON_5);
	TEST_MOUSE("6", MOUSE_BUTTON_6);
	TEST_MOUSE("7", MOUSE_BUTTON_7);
	TEST_MOUSE("8", MOUSE_BUTTON_8);
	TEST_MOUSE("9", MOUSE_BUTTON_UNKNOWN);

	TEST_MOUSE("MOUSE_BUTTON_LEFT", MOUSE_BUTTON_LEFT);
	TEST_MOUSE("MOUSE_BUTTON_RIGHT", MOUSE_BUTTON_RIGHT);
	TEST_MOUSE("MOUSE_BUTTON_MIDDLE", MOUSE_BUTTON_MIDDLE);
	TEST_MOUSE("MOUSE_BUTTON_CENTER", MOUSE_BUTTON_UNKNOWN);

	TEST_MOUSE("mouse_button_left", MOUSE_BUTTON_LEFT);
	TEST_MOUSE("mouse_button_right", MOUSE_BUTTON_RIGHT);
	TEST_MOUSE("mouse_button_middle", MOUSE_BUTTON_MIDDLE);
	TEST_MOUSE("mouse_button_center", MOUSE_BUTTON_UNKNOWN);

	TEST_MOUSE("mouSe_bUttOn_lEft", MOUSE_BUTTON_LEFT);
	TEST_MOUSE("moUse_butTon_rigHt", MOUSE_BUTTON_RIGHT);
	TEST_MOUSE("Mouse_Button_Middle", MOUSE_BUTTON_MIDDLE);
	TEST_MOUSE("mousE_Button_ceNter", MOUSE_BUTTON_UNKNOWN);

	TEST_MOUSE("LEFT", MOUSE_BUTTON_LEFT);
	TEST_MOUSE("RIGHT", MOUSE_BUTTON_RIGHT);
	TEST_MOUSE("MIDDLE", MOUSE_BUTTON_MIDDLE);
	TEST_MOUSE("CENTER", MOUSE_BUTTON_UNKNOWN);

	TEST_MOUSE("lEft", MOUSE_BUTTON_LEFT);
	TEST_MOUSE("Right", MOUSE_BUTTON_RIGHT);
	TEST_MOUSE("miDDle", MOUSE_BUTTON_MIDDLE);
	TEST_MOUSE("center", MOUSE_BUTTON_UNKNOWN);
}

int main()
{
	TestParser();

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