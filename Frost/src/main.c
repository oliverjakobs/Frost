#include "Frost/Frost.h"
#include "Frost/FrostDebugger.h"

#include "Graphics/FontManager.h"

Scene scene;
SceneEditor scene_editor;

FontManager fonts;

Console console;

FrostDebugger debugger;

mat4 screen_projection;

static void set_viewport(int x, int y, int w, int h)
{
	screen_projection = mat4_ortho((float)x, (float)w, (float)h, (float)y, -1.0f, 1.0f);
	glViewport(x, y, w, h);
}

int OnInit(Application* app)
{
	/* ---------------| Config |------------------------------------------ */
	FrostLoadIgnis((IgnisColorRGBA) { 0.2f, 0.2f, 0.2f, 1.0f }, GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	FontManagerInit(&fonts, "res/fonts.json");

	Renderer2DInit("res/shaders/renderer2D.vert", "res/shaders/renderer2D.frag");
	Primitives2DInit("res/shaders/primitives.vert", "res/shaders/primitives.frag");
	BatchRenderer2DInit("res/shaders/batchrenderer.vert", "res/shaders/batchrenderer.frag");
	FontRendererInit("res/shaders/font.vert", "res/shaders/font.frag");

	FontRendererBindFontColor(FontManagerGetFont(&fonts, "gui"), IGNIS_WHITE);

	ApplicationEnableDebugMode(app, 1);
	ApplicationEnableVsync(app, 0);

	FrostDebuggerInit(&debugger);
	ForstDebuggerShow(&debugger, 1);

	ConsoleInit(&console, FontManagerGetFont(&fonts, "gui"));

	/* ecs */
	SceneInit(&scene, (vec2){(float)app->width, (float)app->height}, LoadEcs);

	SceneLoadScenes(&scene, "res/register.json", "scene");
	SceneEditorInit(&scene_editor, 400.0f, 32.0f, 4);
	SceneEditorToggleActive(&scene_editor);

	set_viewport(0, 0, app->width, app->height);

	return 1;
}

void OnDestroy(Application* app)
{
	SceneDestroy(&scene);

	FontRendererDestroy();
	Primitives2DDestroy();
	BatchRenderer2DDestroy();
	Renderer2DDestroy();

	FontManagerDestroy(&fonts);
}

void OnEvent(Application* app, Event e)
{
	if (EventCheckType(&e, EVENT_WINDOW_RESIZE))
		set_viewport(0, 0, e.window.width, e.window.height);

	switch (EventKeyPressed(&e))
	{
	case KEY_ESCAPE:	ApplicationClose(app); break;
	case KEY_F1:		SceneEditorToggleActive(&scene_editor); break;
	case KEY_F2:		ConsoleToggleFocus(&console); break;
	case KEY_F5:		ApplicationPause(app); break;
	case KEY_F6:		ApplicationToggleVsync(app); break;
	case KEY_F7:		ApplicationToggleDebugMode(app); break;
	case KEY_F8:		SceneEditorToggleGrid(&scene_editor); break;
	case KEY_F9:		FrostDebuggerToggleDisplay(&debugger); break;
	}

	if (EventCheckType(&e, EVENT_CONSOLE_EXEC))
		FrostExecuteConsoleCommand(&console, &scene, &scene_editor, e.console.cmd);

	ConsoleOnEvent(&console, &e);

	SceneOnEvent(&scene, e);

	SceneEditorOnEvent(&scene_editor, &scene, e);
}

void OnUpdate(Application* app, float deltatime)
{
	if (console.focus)
		ConsoleOnUpdate(&console, deltatime);
	else if (scene_editor.active)
		SceneEditorOnUpdate(&scene_editor, &scene, deltatime);
	else
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

	FontRendererStart(screen_projection.v);
	
	/* fps */
	FontRendererRenderTextFormat(8.0f, 8.0f, "FPS: %d", app->timer.fps);

	/* Settings */
	FrostDebugRenderSettings(&debugger, app->width - 220.0f, 8.0f);

	/* Debug info */
	FrostDebugRenderInfo(&debugger, &scene, app->width - 470.0f, 8.0f);

	FontRendererFlush();

	ConsoleRenderBackground(&console, 0.0f, (float)app->height, (float)app->width, 32.0f, screen_projection.v);
	ConsoleRender(&console, 0.0f, (float)app->height, 8.0f, screen_projection.v);
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