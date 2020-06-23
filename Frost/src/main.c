#include "Application/Application.h"

#include "Scenes/SceneManager.h"

#include "gui/gui.h"

#include "Console/Console.h"

Camera camera;
SceneManager scene_manager;

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

	gui_init((float)app->width, (float)app->height);
	gui_set_font(ResourceManagerGetFont(&app->resources, "gui"), IGNIS_WHITE);

	ApplicationEnableDebugMode(app, 1);
	ApplicationEnableVsync(app, 0);
	ApplicationShowGui(app, 1);

	CameraCreateOrtho(&camera, app->width / 2.0f, app->height / 2.0f, 0.0f, (float)app->width, (float)app->height);
	SceneManagerInit(&scene_manager, "res/templates/register.json", &app->resources, &camera, 32.0f, 4);
	SceneManagerChangeScene(&scene_manager, "scene");
}

void OnDestroy(Application* app)
{
	gui_free();

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
		CameraSetProjectionOrtho(scene_manager.camera, (float)e.window.width, (float)e.window.height);
	}

	switch (EventKeyPressed(&e))
	{
	case KEY_ESCAPE:
		ApplicationClose(app);
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
		ApplicationToggleGui(app);
		break;
	}

	SceneManagerOnEvent(&scene_manager, e);
}

void OnUpdate(Application* app, float deltaTime)
{
	SceneManagerOnUpdate(&scene_manager, deltaTime);
}

void OnRender(Application* app)
{
	SceneManagerOnRender(&scene_manager);
}

void OnRenderDebug(Application* app)
{
	SceneManagerOnRenderDebug(&scene_manager);

	/* fps */
	FontRendererStart(ApplicationGetScreenProjPtr(app));
	FontRendererRenderTextFormat(8.0f, 20.0f, "FPS: %d", app->timer.fps);
	FontRendererFlush();

	if (scene_manager.console_focus)
		ConsoleRender(&scene_manager.console, 0.0f, (float)app->height, (float)app->width, 32.0f, 8.0f, ApplicationGetScreenProjPtr(app));
}

void OnRenderGui(Application* app)
{
	gui_start();

	/* Settings */
	if (gui_begin_align(GUI_HALIGN_RIGHT, GUI_VALIGN_TOP, 220.0f, 140.0f, 8.0f, GUI_BG_FILL))
	{
		gui_text("F1: Toggle edit mode");
		gui_text("F2: Open console");
		gui_text("F5: Pause/Unpause");
		gui_text("F6: Toggle Vsync");
		gui_text("F7: Toggle debug mode");
		gui_text("F8: Toggle Gui");
	}
	gui_end();

	/* Debug */
	if (gui_begin(app->width - 470.0f, 0.0f, 250.0f, 128.0f, 8.0f, GUI_BG_FILL))
	{
		gui_text("Scene: %s", scene_manager.scene_name);
		gui_separator();

		char* name = "player";
		gui_text("Name: %s", name);
		vec2 position = EntityGetPosition(name, &scene_manager.scene->components);
		gui_text("Position: %4.2f, %4.2f", position.x, position.y);
		gui_text("Precise Y: %f", position.y);
	}
	gui_end();
	
	SceneManagerOnRenderGui(&scene_manager);

	gui_render(ApplicationGetScreenProjPtr(app));
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
	ApplicationSetOnRenderGuiCallback(&app, OnRenderGui);

	ApplicationLoadConfig(&app, "config.json");

	ApplicationRun(&app);

	ApplicationDestroy(&app);

	return 0;
}