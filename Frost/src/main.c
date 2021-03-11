#include "Frost/Frost.h"
#include "Frost/FrostDebugger.h"

#include "toolbox/tb_ini.h"
#include "toolbox/tb_file.h"

#include "GUI/Gui.h"

Scene scene;
SceneEditor scene_editor;

FrostDebugger debugger;

GuiManager gui;

int OnInit(Application* app)
{
	/* ---------------| Config |------------------------------------------ */
	FrostLoadIgnis((IgnisColorRGBA) { 0.2f, 0.2f, 0.2f, 1.0f }, GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	FrostLoadScene(&scene, "res/frost.ini", (float)app->width, (float)app->height);

	FrostLoadRenderer("config.ini");

	GuiInit(&gui, (float)app->width, (float)app->height, "res/fonts.json");
	FontRendererBindFontColor(GuiGetFont(&gui, "gui"), IGNIS_WHITE);

	FrostDebuggerInit(&debugger, 1, GuiGetFont(&gui, "gui"));
	FrostDebuggerBindScene(&debugger, &scene, &scene_editor);

	SceneEditorInit(&scene_editor, &scene, 400.0f, 4);
	SceneEditorSetMode(&scene_editor, SCENE_EDIT_MAP);

	glViewport(0, 0, app->width, app->height);

	return 1;
}

void OnDestroy(Application* app)
{
	SceneDestroy(&scene);

	FontRendererDestroy();
	Primitives2DDestroy();
	BatchRenderer2DDestroy();
	Renderer2DDestroy();

	GuiDestroy(&gui);
}

void OnEvent(Application* app, Event e)
{
	if (EventCheckType(&e, EVENT_WINDOW_RESIZE))
	{
		GuiSetViewport(&gui, (float)e.window.width, (float)e.window.height);
		glViewport(0, 0, e.window.width, e.window.height);
	}

	switch (EventKeyPressed(&e))
	{
	case KEY_ESCAPE:	ApplicationClose(app); break;
	case KEY_F1:		SceneEditorToggleWorldMode(&scene_editor); break;
	case KEY_F2:		SceneEditorToggleMapMode(&scene_editor); break;
	case KEY_F3:		ConsoleToggleFocus(&debugger.console); break;
	case KEY_F5:		ApplicationPause(app); break;
	case KEY_F6:		ApplicationToggleVsync(app); break;
	case KEY_F7:		ApplicationToggleDebug(app); break;
	case KEY_F8:		SceneEditorToggleGrid(&scene_editor); break;
	case KEY_F9:		FrostDebuggerToggleInfo(&debugger); break;
	}

	FrostDebuggerOnEvent(&debugger, e);

	SceneOnEvent(&scene, e);

	SceneEditorOnEvent(&scene_editor, e);
}

void OnUpdate(Application* app, float deltatime)
{
	if (debugger.console.focus)
		FrostDebuggerOnUpdate(&debugger, deltatime);
	else if (SceneEditorIsActive(&scene_editor))
		SceneEditorOnUpdate(&scene_editor, deltatime);
	else
		SceneOnUpdate(&scene, deltatime);
}

void OnRender(Application* app)
{
	SceneOnRender(&scene);

	SceneEditorOnRender(&scene_editor);

	if (scene_editor.mode != SCENE_EDIT_MAP)
		SceneOnRenderUI(&scene);
}

void OnRenderDebug(Application* app)
{
	if (!SceneEditorIsActive(&scene_editor)) SceneOnRenderDebug(&scene);

	FontRendererStart(GuiGetScreenProjPtr(&gui));
	
	/* fps */
	FontRendererRenderTextFormat(8.0f, 8.0f, "FPS: %d", app->timer.fps);

	/* Settings */
	FrostDebugRenderSettings(&debugger, app->width - 220.0f, 8.0f);

	/* Debug info */
	FrostDebugRenderInfo(&debugger, &scene, app->width - 470.0f, 8.0f);

	FontRendererFlush();

	FrostDebugRenderConsole(&debugger, (float)app->width, (float)app->height, GuiGetScreenProjPtr(&gui));
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

	ApplicationLoadConfig(&app, "config.ini");

	ApplicationRun(&app);

	ApplicationDestroy(&app);

	return 0;
}