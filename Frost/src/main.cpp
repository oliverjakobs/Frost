#include "Application/Application.h"

#include "SceneManager.h"

#include "ImGuiBinding/ImGuiRenderer.hpp"

static mat4 SCREEN_MAT;

void SetViewport(int x, int y, int w, int h)
{
	SCREEN_MAT = mat4_ortho((float)x, (float)w, (float)h, (float)y, -1.0f, 1.0f);

	glViewport(x, y, w, h);
}

const float* GetScreenMatPtr()
{
	return &SCREEN_MAT.v[0];
}

SceneManager scene_manager;
ResourceManager resources;
Camera camera;

IgnisFont* font;

void OnEvent(Application* app, const Event e)
{
	if (e.type == EVENT_WINDOW_RESIZE)
	{
		SetViewport(0, 0, e.window.width, e.window.height);
	}

	if (e.type == EVENT_KEY_PRESSED)
	{
		switch (e.key.keycode)
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
		case KEY_1:
			SceneManagerChangeScene(&scene_manager, "scene");
			break;
		case KEY_2:
			SceneManagerChangeScene(&scene_manager, "scene2");
			break;
		}
	}

	SceneManagerOnEvent(&scene_manager, e);
}

void OnUpdate(Application* app, float deltaTime)
{
	// discard frames that took to long
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

	// debug info
	FontRendererRenderTextFormat(0.0f, 32.0f, GetScreenMatPtr(), "FPS: %d", app->timer.fps);
}

void OnRenderGui(Application* app)
{
	ImGuiRenderer::Begin();

	ImGui::Begin("Settings");

	ImGui::Text("FPS: %d", app->timer.fps);

	ImGui::Text("F5: Pause/Unpause");
	ImGui::Text("F6: Toggle Vsync");
	ImGui::Text("F7: Toggle debug mode");
	ImGui::Text("F8: Toggle ImGui");

	ImGui::Separator();

	ImGui::Text("F1: Toggle edit mode");
	ImGui::Text("DEL: Remove Trees");

	ImGui::End();

	// ----
	ImGui::Begin("DEBUG");

	auto player = SceneGetEntity(scene_manager.scene, "player", 1);

	if (player != nullptr)
	{
		ImGui::Text("Name: %s", player->name);
		auto position = EcsEntityGetPosition(player);
		ImGui::Text("Position: %4.2f, %4.2f", position.x, position.y);
		ImGui::Text("Precise Y: %f", position.y);

		ImGui::Separator();
	}

	ImGui::Text("Scene: %s", scene_manager.scene_name);

	ImGui::End();

	if (scene_manager.editmode)
	{
		ImGui::Begin("Editor");

		ImGui::Text("Hovered Entity: %s", scene_manager.hover == nullptr ? "null" : scene_manager.hover->name);

		ImGui::Checkbox("Show grid", &scene_manager.showgrid);

		ImGui::Separator();

		for (size_t i = 0; i < scene_manager.scene->max_layer; i++)
		{
			if (scene_manager.scene->layers[i].size > 0)
			{
				char buffer[16];
				snprintf(buffer, sizeof(buffer), "Layer: %zu", i);
				ImGui::RadioButton(buffer, &scene_manager.layer, (int)i);
			}
		}

		ImGui::End();
	}

	SceneManagerOnRenderGui(&scene_manager);

	ImGuiRenderer::End();
}


#define RUN_GAME 1

int main()
{
#if RUN_GAME

	Application* app = new Application();
	
	ApplicationLoad(app, "Frost", 1024, 800, 4, 4);

	// ---------------| Config |------------------------------------------
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);

	SCREEN_MAT = mat4_ortho(0.0f, (float)app->width, (float)app->height, 0.0f, -1.0f, 1.0f);

	Renderer2DInit("res/shaders/renderer2D.vert", "res/shaders/renderer2D.frag");
	Primitives2DInit();
	BatchRenderer2DInit("res/shaders/batchrenderer.vert", "res/shaders/batchrenderer.frag");
	FontRendererInit("res/shaders/font.vert", "res/shaders/font.frag");

	ResourceManagerInit(&resources, "res/index.json");
	FontRendererBindFont(ResourceManagerGetFont(&resources, "font"));

	font = ResourceManagerGetFont(&resources, "font");

	ApplicationEnableDebugMode(app, true);
	ApplicationEnableVsync(app, false);
	ApplicationShowGui(app, true);

	// initialize imgui
	ImGuiRenderer::Init(app->window, ImGuiConfigFlags_NavEnableKeyboard | ImGuiConfigFlags_DockingEnable);

	CameraCreateOrtho(&camera, { app->width / 2.0f, app->height / 2.0f, 0.0f }, { (float)app->width, (float)app->height });
	SceneManagerInit(&scene_manager, &resources, &camera, 32.0f, 4);
	SceneManagerRegisterScenes(&scene_manager, "res/templates/scenes/register.json");
	SceneManagerChangeScene(&scene_manager, "scene");

	ApplicationSetOnEventCallback(app, OnEvent);
	ApplicationSetOnUpdateCallback(app, OnUpdate);
	ApplicationSetOnRenderCallback(app, OnRender);
	ApplicationSetOnRenderDebugCallback(app, OnRenderDebug);
	ApplicationSetOnRenderGuiCallback(app, OnRenderGui);

	ApplicationRun(app);

	FontRendererDestroy();
	Primitives2DDestroy();
	BatchRenderer2DDestroy();
	Renderer2DDestroy();

	ResourceManagerDestroy(&resources);

	ApplicationDestroy(app);

	delete app;
#else

	system("Pause");
#endif

	return 0;
}