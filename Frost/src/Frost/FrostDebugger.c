#include "FrostDebugger.h"

void FrostShowDebugInfo(const Scene* scene, vec2 screen_size)
{
	/* Settings */
	FontRendererTextFieldBegin(screen_size.x - 220.0f, 0.0f, 24.0f);

	FontRendererTextFieldLine("F1: Toggle edit mode");
	FontRendererTextFieldLine("F2: Open console");
	FontRendererTextFieldLine("F5: Pause/Unpause");
	FontRendererTextFieldLine("F6: Toggle Vsync");
	FontRendererTextFieldLine("F7: Toggle debug mode");
	FontRendererTextFieldLine("F8: Toggle editor grid");
	FontRendererTextFieldLine("F9: Toggle overlay");

	/* Debug */
	FontRendererTextFieldBegin(screen_size.x - 470.0f, 0.0f, 24.0f);

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

	FontRendererTextFieldLine("");

	FontRendererTextFieldLine("Camera:");
	FontRendererTextFieldLine("------------------------");
	FontRendererTextFieldLine("Position: %4.2f, %4.2f", scene->camera.position.x, scene->camera.position.y);
}
