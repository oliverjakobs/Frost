#include "Frost/Frost.h"
#include "Frost/FrostEcs.h"
#include "Frost/AnimationConditions.h"

#include "Inventory/Inventory.h"

Camera camera;
SceneManager scene_manager;
SceneEditor editor;
Console console;

int show_overlay;

Inventory inv;
IgnisTexture2D* items;

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

	show_overlay = 1;

	CameraCreateOrtho(&camera, app->width / 2.0f, app->height / 2.0f, 0.0f, (float)app->width, (float)app->height);

	SceneManagerInit(&scene_manager, "res/templates/register.json", &app->resources, &camera, 32.0f, 4);
	SceneEditorInit(&editor, 400.0f, 32.0f, 4);
	ConsoleInit(&console, ResourcesGetFont(scene_manager.resources, "gui"));

	/* animation conditions */
	AnimationConditionsRegisterCondition("condition_jump", AnimationConditionJump);
	AnimationConditionsRegisterCondition("condition_fall", AnimationConditionFall);
	AnimationConditionsRegisterCondition("condition_walk", AnimationConditionWalk);
	AnimationConditionsRegisterCondition("condition_idle", AnimationConditionIdle);

	/* ecs */
	EcsAddUpdateSystem(&scene_manager.ecs, PhysicsSystem);
	EcsAddUpdateSystem(&scene_manager.ecs, PlayerSystem);
	EcsAddUpdateSystem(&scene_manager.ecs, AnimationSystem);
	EcsAddUpdateSystem(&scene_manager.ecs, InteractionSystem);
	EcsAddRenderSystem(&scene_manager.ecs, RenderSystem);
	EcsAddRenderSystem(&scene_manager.ecs, DebugRenderSystem);

	EcsRegisterDataComponent(&scene_manager.ecs, sizeof(Transform), NULL);
	EcsRegisterDataComponent(&scene_manager.ecs, sizeof(RigidBody), NULL);
	EcsRegisterDataComponent(&scene_manager.ecs, sizeof(Movement), NULL);
	EcsRegisterDataComponent(&scene_manager.ecs, sizeof(Sprite), NULL);
	EcsRegisterDataComponent(&scene_manager.ecs, sizeof(Animator), AnimatorFree);
	EcsRegisterDataComponent(&scene_manager.ecs, sizeof(CameraController), NULL);
	EcsRegisterDataComponent(&scene_manager.ecs, sizeof(Interaction), NULL);
	EcsRegisterDataComponent(&scene_manager.ecs, sizeof(Interactor), NULL);

	EcsRegisterOrderComponent(&scene_manager.ecs, sizeof(Template), TemplateCmp);
	EcsRegisterOrderComponent(&scene_manager.ecs, sizeof(ZIndex), ZIndexCmp);

	/* setup starting state */
	SceneManagerChangeActive(&scene_manager, "scene");
	SceneEditorToggleActive(&editor);

	InventoryInit(&inv, (vec2) { 0.0f, -camera.size.y / 2.0f }, 2, 9, 64.0f, 8.0f);
	inv.pos.x -= inv.size.x / 2.0f;
	InventorySetCellContent(&inv, 3, 3);
	InventorySetCellContent(&inv, 4, 1);
	InventorySetCellContent(&inv, 9, 2);
	InventorySetCellContent(&inv, 17, 3);

	items = ResourcesGetTexture2D(scene_manager.resources, "items");
}

void OnDestroy(Application* app)
{
	InventoryFree(&inv);

	SceneManagerDestroy(&scene_manager);

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
		SceneEditorToggleGrid(&editor);
		break;
	case KEY_F9:
		show_overlay = !show_overlay;
		break;
	}

	if (EventCheckType(&e, EVENT_CONSOLE_EXEC))
		FrostExecuteConsoleCommand(&console, &scene_manager, &editor, e.console.cmd);

	ConsoleOnEvent(&console, &e);

	SceneManagerOnEvent(&scene_manager, e);

	SceneEditorOnEvent(&editor, &scene_manager, e);
}

void OnUpdate(Application* app, float deltatime)
{
	ConsoleOnUpdate(&console, deltatime);

	if (!console.focus)
		SceneEditorOnUpdate(&editor, &scene_manager, deltatime);

	if (!(editor.active || console.focus))
		SceneManagerOnUpdate(&scene_manager, deltatime);

	InventoryUpdate(&inv, &camera, deltatime);
}

void OnRender(Application* app)
{
	SceneManagerOnRender(&scene_manager);

	SceneEditorOnRender(&editor, &scene_manager);

	InventoryRender(&inv, camera.projection);
	InventoryRenderContent(&inv, items, camera.projection);
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
		FontRendererTextFieldLine("F8: Toggle editor grid");
		FontRendererTextFieldLine("F9: Toggle overlay");

		/* Debug */
		FontRendererTextFieldBegin(app->width - 470.0f, 0.0f, 24.0f);

		FontRendererTextFieldLine("Scene: %s", scene_manager.scene_name);
		FontRendererTextFieldLine("------------------------");

		EntityID player = 0;
		FontRendererTextFieldLine("Player ID: %d", player);
		vec2 position = GetEntityPosition(&scene_manager.ecs, player);
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