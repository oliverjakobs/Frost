#include "FrostDebugger.h"

void FrostDebuggerInit(FrostDebugger* debugger)
{
	debugger->show = 0;
}

void ForstDebuggerShow(FrostDebugger* debugger, int b)
{
	debugger->show = b;
}

void FrostDebuggerToggleDisplay(FrostDebugger* debugger)
{
	debugger->show = !debugger->show;
}

void FrostDebugRenderSettings(const FrostDebugger* debugger, float x, float y)
{
	if (!debugger->show) return;

	FontRendererTextFieldBegin(x, y, 8.0f);

	FontRendererTextFieldLine("F1: Toggle edit mode");
	FontRendererTextFieldLine("F2: Open console");
	FontRendererTextFieldLine("F5: Pause/Unpause");
	FontRendererTextFieldLine("F6: Toggle Vsync");
	FontRendererTextFieldLine("F7: Toggle debug mode");
	FontRendererTextFieldLine("F8: Toggle editor grid");
	FontRendererTextFieldLine("F9: Toggle overlay");
}

void FrostDebugRenderInfo(const FrostDebugger* debugger, const Scene* scene, float x, float y)
{
	if (!debugger->show) return;

	FontRendererTextFieldBegin(x, y, 8.0f);

	FontRendererTextFieldLine("Scene: %s", scene->name);
	FontRendererTextFieldLine("Size: %4.2f, %4.2f", SceneGetWidth(scene), SceneGetHeight(scene));
	FontRendererTextFieldLine("Gravity: %4.2f, %4.2f", scene->gravity.x, scene->gravity.y);
	FontRendererTextFieldLine("------------------------");

	EcsEntityID player = 0;
	FontRendererTextFieldLine("Player ID: %d", player);
	FontRendererTextFieldLine("State: %s", EntityGetStateString(&scene->ecs, player));
	vec2 position = GetEntityPosition(&scene->ecs, player);
	FontRendererTextFieldLine("Position: %4.2f, %4.2f", position.x, position.y);
	FontRendererTextFieldLine("Precise Y: %f", position.y);
	FontRendererTextFieldLine("------------------------");

	FontRendererTextFieldLine("Camera:");
	FontRendererTextFieldLine("Position: %4.2f, %4.2f", scene->camera.position.x, scene->camera.position.y);
}
