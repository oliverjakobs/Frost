#include "Application/Application.h"

#include "Scenes/SceneManager.h"

#include "physics/collision.h"

Camera camera;
SceneManager scene_manager;

int show_overlay;

vec2 mouse;

aabb rects[] =
{
	{ {340.0f, 140.0f}, {32.0f, 64.0f} },

	{ {300.0f, 100.0f}, {40.0f, 40.0f} },
	{ {340.0f, 100.0f}, {40.0f, 40.0f} },
	{ {380.0f, 100.0f}, {40.0f, 40.0f} },
	{ {420.0f, 100.0f}, {40.0f, 40.0f} },
	{ {460.0f, 100.0f}, {40.0f, 40.0f} },
	{ {500.0f, 100.0f}, {40.0f, 40.0f} },
	{ {540.0f, 100.0f}, {40.0f, 40.0f} },
	{ {580.0f, 100.0f}, {40.0f, 40.0f} },
	{ {620.0f, 100.0f}, {40.0f, 40.0f} },
	{ {660.0f, 100.0f}, {40.0f, 40.0f} },
	{ {700.0f, 100.0f}, {40.0f, 40.0f} },

	{ {280.0f, 100.0f}, {20.0f, 120.0f} },
	{ {740.0f, 100.0f}, {20.0f, 120.0f} },

	{ {300.0f, 320.0f}, {4.0f, 4.0f} },
	{ {300.0f, 360.0f}, {160.0f, 40.0f} },
	{ {360.0f, 300.0f}, {40.0f, 160.0f} }
};

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

	show_overlay = 0;

	CameraCreateOrtho(&camera, app->width / 2.0f, app->height / 2.0f, 0.0f, (float)app->width, (float)app->height);
	SceneManagerInit(&scene_manager, "res/templates/register.json", &app->resources, &camera, 32.0f, 4);
	SceneManagerChangeScene(&scene_manager, "scene");
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
		show_overlay = !show_overlay;
		break;
	}

	SceneManagerOnEvent(&scene_manager, e);
}

void OnUpdate(Application* app, float deltaTime)
{
	// SceneManagerOnUpdate(&scene_manager, deltaTime);

	mouse = CameraGetMousePos(&camera, InputMousePositionVec2());
}

void OnRender(Application* app)
{
	// SceneManagerOnRender(&scene_manager);

	rect r = { { 500.0f, 200.0f}, { 700.0f, 440.0f} };

	vec2 ray_point = { 100.0f, 600.0f };
	vec2 ray_direction = vec2_sub(mouse, ray_point);

	vec2 contact_point;
	vec2 contact_normal;
	float t;

	Primitives2DStart(CameraGetViewProjectionPtr(&camera));

	for (int i = 0; i < sizeof(rects) / sizeof(rects[0]); ++i)
	{
		aabb r = rects[i];
		Primitives2DRenderRect(r.min.x, r.min.y, r.max.x, r.max.y, IGNIS_WHITE);
		// Primitives2DRenderRect(r.min.x, r.min.y, r.max.x - r.min.x, r.max.y - r.min.y, IGNIS_WHITE);
	}


	Primitives2DFlush();
}

void OnRenderDebug(Application* app)
{
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
		vec2 position = EcsGetEntityPosition(&scene_manager.scene->ecs, player);
		FontRendererTextFieldLine("Position: %4.2f, %4.2f", position.x, position.y);
		FontRendererTextFieldLine("Precise Y: %f", position.y);
	}

	FontRendererFlush();

	if (scene_manager.console_focus)
		ConsoleRender(&scene_manager.console, 0.0f, (float)app->height, (float)app->width, 32.0f, 8.0f, ApplicationGetScreenProjPtr(app));
}

void OnRenderGui(Application* app)
{
	SceneManagerOnRenderGui(&scene_manager);
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