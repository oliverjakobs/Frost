#include "Application/Application.h"

#include "SceneManager.h"

#include "gui/gui.h"

#include "Console.h"

SceneManager scene_manager;
Camera camera;

Console console;

void OnInit(Application* app)
{
	/* ---------------| Config |------------------------------------------ */
	ignisEnableBlend(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);

	Renderer2DInit("res/shaders/renderer2D.vert", "res/shaders/renderer2D.frag");
	Primitives2DInit("res/shaders/primitives.vert", "res/shaders/primitives.frag");
	BatchRenderer2DInit("res/shaders/batchrenderer.vert", "res/shaders/batchrenderer.frag");
	FontRendererInit("res/shaders/font.vert", "res/shaders/font.frag");

	FontRendererBindFont(ResourceManagerGetFont(&app->resources, "gui"), IGNIS_WHITE);

	gui_init((float)app->width, (float)app->height);
	gui_set_font(ResourceManagerGetFont(&app->resources, "gui"), IGNIS_WHITE);

	ApplicationEnableDebugMode(app, 1);
	ApplicationEnableVsync(app, 0);
	ApplicationShowGui(app, 1);

	CameraCreateOrtho(&camera, (vec3) { app->width / 2.0f, app->height / 2.0f, 0.0f }, (vec2) { (float)app->width, (float)app->height });
	SceneManagerInit(&scene_manager, &app->resources, &camera, 32.0f, 4);
	SceneManagerRegisterScenes(&scene_manager, "res/templates/scenes/register.json");
	SceneManagerChangeScene(&scene_manager, "scene");

	SceneManagerSaveScene(&scene_manager, scene_manager.scene, "res/templates/scenes/save.json");

	ConsoleInit(&console);
}

void OnDestroy(Application* app)
{
	gui_free();

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
	}

	if (e.type == EVENT_KEY_PRESSED)
	{
		switch (e.key.keycode)
		{
		case KEY_ESCAPE:
			ApplicationClose(app);
			break;
		case KEY_F2:
			ConsoleFocus(&console);
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
		/*
		case KEY_1:
			SceneManagerChangeScene(&scene_manager, "scene");
			break;
		case KEY_2:
			SceneManagerChangeScene(&scene_manager, "scene2");
			break;
		*/
		}
	}

	ConsoleOnEvent(&console, &e);
	SceneManagerOnEvent(&scene_manager, e);
}

void OnUpdate(Application* app, float deltaTime)
{
	if (console.focus)
		return;

	/* discard frames that took to long */
	// if (deltaTime > 0.4f) return;
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

	if (console.focus)
		FontRendererRenderTextFormat(8.0f, app->height - 8.0f, "> %.*s", console.cusor_pos, console.cmd_buffer);

	FontRendererFlush();
}

void OnRenderGui(Application* app)
{
	gui_start();

	/* Debug */
	if (gui_begin_align(GUI_HALIGN_RIGHT, GUI_VALIGN_BOTTOM, 250.0f, 105.0f, 8.0f, GUI_BG_FILL))
	{
		gui_text("Scene: %s", scene_manager.scene_name);
		gui_text("Layer: %d", scene_manager.editor.layer);
		gui_separator();
		EcsEntity* player = SceneGetEntity(scene_manager.scene, "player", 1);
		if (player)
		{
			gui_text("Name: %s", player->name);
			vec2 position = EcsEntityGetPosition(player);
			gui_text("Position: %4.2f, %4.2f", position.x, position.y);
			gui_text("Precise Y: %f", position.y);
		}
	}
	gui_end();

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

	/*
	if (scene_manager.editmode)
	{
		if (gui_begin_align(GUI_HALIGN_LEFT, GUI_VALIGN_BOTTOM, 220.0f, 140.0f, 8.0f, GUI_BG_FILL))
		{
			gui_text("Hovered Entity: %s", scene_manager.editor.hover ? scene_manager.editor.hover->name : "null");
		}
		gui_end();
	}
	*/
	
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