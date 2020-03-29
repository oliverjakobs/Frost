#include "Application.hpp"

#include "SceneManager.hpp"
#include <glm/gtc/matrix_transform.hpp>

#include "ImGuiBinding/ImGuiRenderer.hpp"

#include "Obelisk/Obelisk.hpp"

static glm::mat4 SCREEN_MAT = glm::mat4(1.0f);

void SetViewport(int x, int y, int w, int h)
{
	SCREEN_MAT = glm::ortho((float)x, (float)w, (float)h, (float)y);

	glViewport(x, y, w, h);
}

const float* GetScreenMatPtr()
{
	return &SCREEN_MAT[0][0];
}

struct Game : public Application
{
	SceneManager sceneManager;
	ResourceManager resources;
	Camera camera;

	IgnisFont* font;
};

void OnEvent(Application* app, const Event& e)
{
	if (e.Type == EventType::WindowResize)
	{
		auto& resize = (WindowResizeEvent&)e;
		SetViewport(0, 0, resize.Width, resize.Height);
	}

	if (e.Type == EventType::KeyPressed)
	{
		switch (((KeyPressedEvent&)e).KeyCode)
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
			SceneManagerChangeScene(&((Game*)app)->sceneManager, "scene");
			break;
		case KEY_2:
			SceneManagerChangeScene(&((Game*)app)->sceneManager, "scene2");
			break;
		}
	}

	SceneManagerOnEvent(&((Game*)app)->sceneManager, e);
}

void OnUpdate(Application* app, float deltaTime)
{
	// discard frames that took to long
	// if (deltaTime > 0.4f) return;

	SceneManagerOnUpdate(&((Game*)app)->sceneManager, deltaTime);
}

void OnRender(Application* app)
{
	SceneManagerOnRender(&((Game*)app)->sceneManager);
}

void OnRenderDebug(Application* app)
{
	SceneManagerOnRenderDebug(&((Game*)app)->sceneManager);

	// debug info
	FontRendererRenderText(((Game*)app)->font, obelisk::format("FPS: %d", app->timer.fps).c_str(), 0.0f, 32.0f, GetScreenMatPtr(), IGNIS_WHITE);
}

void OnRenderGui(Application* app)
{
	ImGuiRenderer::Begin();

	ImGui::Begin("Settings");

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

	auto player = SceneGetEntity(((Game*)app)->sceneManager.scene, "player", 1);

	if (player != nullptr)
	{
		ImGui::Text("Name: %s", player->name.c_str());
		auto position = EntityGetPosition(player);
		ImGui::Text("Position: %4.2f, %4.2f", position.x, position.y);
		ImGui::Text("Precise Y: %f", position.y);

		ImGui::Separator();
	}

	ImGui::Text("Scene: %s", ((Game*)app)->sceneManager.scene_name);

	ImGui::End();

	SceneManagerOnImGui(&((Game*)app)->sceneManager);

	ImGuiRenderer::End();
}


#define RUN_GAME 1

int main()
{
#if RUN_GAME

	Game* game = new Game();
	
	ApplicationLoad(game, "Frost", 1024, 800, 4, 4);

	// ---------------| Config |------------------------------------------
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);

	SCREEN_MAT = glm::ortho(0.0f, (float)game->width, (float)game->height, 0.0f);

	Renderer2DInit("res/shaders/renderer2D.vert", "res/shaders/renderer2D.frag");
	Primitives2DInit("res/shaders/lines.vert", "res/shaders/lines.frag");
	BatchRenderer2DInit("res/shaders/batchrenderer.vert", "res/shaders/batchrenderer.frag");
	FontRendererInit("res/shaders/font.vert", "res/shaders/font.frag");

	ResourceManagerInit(&game->resources, "res/index.json");

	game->font = ResourceManagerGetFont(&game->resources, "font");

	ApplicationEnableDebugMode(game, true);
	ApplicationEnableVsync(game, false);
	ApplicationShowGui(game, true);

	// initialize imgui
	ImGuiRenderer::Init(game->window, ImGuiConfigFlags_NavEnableKeyboard | ImGuiConfigFlags_DockingEnable);

	CameraCreateOrtho(&game->camera, glm::vec3(game->width / 2.0f, game->height / 2.0f, 0.0f), glm::vec2(game->width, game->height));
	SceneManagerInit(&game->sceneManager, &game->resources, &game->camera, 32.0f, 4);
	SceneManagerRegisterScenes(&game->sceneManager, "res/templates/scenes/register.json");
	SceneManagerChangeScene(&game->sceneManager, "scene");

	ApplicationSetOnEventCallback(game, OnEvent);
	ApplicationSetOnUpdateCallback(game, OnUpdate);
	ApplicationSetOnRenderCallback(game, OnRender);
	ApplicationSetOnRenderDebugCallback(game, OnRenderDebug);
	ApplicationSetOnRenderGuiCallback(game, OnRenderGui);

	ApplicationRun(game);

	FontRendererDestroy();
	Primitives2DDestroy();
	BatchRenderer2DDestroy();
	Renderer2DDestroy();

	ResourceManagerDestroy(&game->resources);

	ApplicationDestroy(game);

	delete game;
#else

	system("Pause");
#endif

	return 0;
}