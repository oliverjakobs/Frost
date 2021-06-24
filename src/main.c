#include "Frost/Frost.h"
#include "Frost/FrostDebugger.h"

#include "toolbox/tb_ini.h"
#include "toolbox/tb_file.h"

Scene scene;
SceneEditor scene_editor;

FrostDebugger debugger;

MinimalBool OnLoad(MinimalApp* app, uint32_t w, uint32_t h)
{
	FontRendererBindFontColor(GuiGetFont("gui"), IGNIS_WHITE);

	FrostLoadScene(&scene, (float)w, (float)h, "res/scenes/scene.json");
	FrostDebuggerInit(&debugger, 1, GuiGetFont("gui"));
	FrostDebuggerBindScene(&debugger, &scene, &scene_editor);

	SceneEditorInit(&scene_editor, &scene, 400.0f, 4, SCENE_EDIT_NONE);

	/* temp event registration */
	EcsEventSubscribe(INTERACTION_TOGGLE_DOOR, InteractionToggleDoor);
	EcsEventSubscribe(INTERACTION_OPEN_INVENTORY, InteractionOpenInventory);

	return MINIMAL_OK;
}

void OnDestroy(MinimalApp* app)
{
	SceneDestroy(&scene);
	FrostDestroy(app);
}

MinimalBool OnEvent(MinimalApp* app, const MinimalEvent* e)
{
	if (MinimalCheckEventType(e, MINIMAL_EVENT_WINDOW_SIZE))
	{
		GuiSetViewport((float)e->lParam, (float)e->rParam);
		glViewport(0, 0, e->lParam, e->rParam);
	}

	switch (MinimalEventKeyPressed(e))
	{
	case MINIMAL_KEY_ESCAPE:	MinimalClose(app); break;
	case MINIMAL_KEY_F1:		SceneEditorToggleWorldMode(&scene_editor); break;
	case MINIMAL_KEY_F2:		SceneEditorToggleMapMode(&scene_editor); break;
	case MINIMAL_KEY_F3:		ConsoleToggleFocus(&debugger.console); break;
	case MINIMAL_KEY_F6:		MinimalToggleVsync(app); break;
	case MINIMAL_KEY_F7:		MinimalToggleDebug(app); break;
	case MINIMAL_KEY_F8:		SceneEditorToggleGrid(&scene_editor); break;
	case MINIMAL_KEY_F9:		FrostDebuggerToggleInfo(&debugger); break;
	}

	if (scene_editor.mode == SCENE_EDIT_MAP)		MinimalSetTitle(app, "Frost | Map Editor");
	else if (scene_editor.mode == SCENE_EDIT_WORLD)	MinimalSetTitle(app, "Frost | World Editor");
	else											MinimalSetTitle(app, "Frost");

	if (FrostDebuggerOnEvent(&debugger, e)) return MINIMAL_OK;

	SceneEditorOnEvent(&scene_editor, e);
	SceneOnEvent(&scene, e);

	return MINIMAL_OK;
}

void OnUpdate(MinimalApp* app, float deltatime)
{
	if (debugger.console.focus)						FrostDebuggerOnUpdate(&debugger, deltatime);
	else if (SceneEditorIsActive(&scene_editor))	SceneEditorOnUpdate(&scene_editor, deltatime);
	else											SceneOnUpdate(&scene, deltatime);
}

void OnRender(MinimalApp* app)
{
	SceneOnRender(&scene);
	SceneEditorOnRender(&scene_editor);
}

void OnRenderDebug(MinimalApp* app)
{
	if (!SceneEditorIsActive(&scene_editor)) SceneOnRenderDebug(&scene);
}

void OnRenderUI(MinimalApp* app)
{
	if (!SceneEditorIsActive(&scene_editor)) SceneOnRenderUI(&scene);

	FrostDebuggerOnRenderUI(&debugger, app);
}

static void ClearBuffer() { glClear(GL_COLOR_BUFFER_BIT); }

int main()
{
	MinimalApp app = { 0 };

	MinimalSetLoadCallback(&app, OnLoad);
	MinimalSetDestroyCallback(&app, OnDestroy);
	MinimalSetEventCallback(&app, OnEvent);
	MinimalSetUpdateCallback(&app, OnUpdate);
	MinimalSetRenderCallback(&app, OnRender);
	MinimalSetRenderDebugCallback(&app, OnRenderDebug);
	MinimalSetRenderUICallback(&app, OnRenderUI);

	if (FrostLoad(&app, "config.ini"))
		MinimalRun(&app, ClearBuffer);

	MinimalDestroy(&app);

	size_t bytes = DebugMemoryGetBytes();
	if (bytes != 0) MINIMAL_WARN("%llu bytes not freed", bytes);

	return 0;
}