#include "Application.hpp"

#include "SceneManager.hpp"

#include "Background.h"

#include "json/TemplateLoader.hpp"

class Frost : public Application
{
private:
	SceneManager m_sceneManager;
	ResourceManager m_resources;
	Camera m_camera;

	IgnisFont* m_font;

	Background m_background;
public:
	Frost() : Application("Frost", 1024, 800)
	{
		// ---------------| Config |------------------------------------------
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		//glEnable(GL_DEPTH_TEST);
		glClearColor(0.2f, 0.2f, 0.2f, 1.0f);

		Renderer2DInit("res/shaders/renderer2D.vert", "res/shaders/renderer2D.frag");
		Primitives2DInit("res/shaders/lines.vert", "res/shaders/lines.frag");
		BatchRenderer2DInit("res/shaders/batchrenderer.vert", "res/shaders/batchrenderer.frag");
		FontRendererInit("res/shaders/font.vert", "res/shaders/font.frag");

		ResourceManagerInit(&m_resources, "res/index.json");

		m_font = ResourceManagerGetFont(&m_resources, "font");

		EnableDebugMode(true);
		EnableImGui(true);
		EnableVsync(false);

		CameraCreateOrtho(&m_camera, glm::vec3(m_width / 2.0f, m_height / 2.0f, 0.0f), glm::vec2(m_width, m_height));
		SceneManagerInit(&m_sceneManager, &m_resources, &m_camera, 32.0f, 4);
		SceneManagerRegisterScenes(&m_sceneManager, "res/templates/scenes/register.json");
		SceneManagerChangeScene(&m_sceneManager, "scene");

		BackgroundInit(&m_background, 4);
		BackgroundPushLayer(&m_background, ResourceManagerGetTexture(&m_resources, "bg_layer_0"), 0.0f, 288.0f, 1088.0f, 600.0f, 1.0f);
		BackgroundPushLayer(&m_background, ResourceManagerGetTexture(&m_resources, "bg_layer_1"), 0.0f, 288.0f, 1088.0f, 600.0f, 0.5f);
		BackgroundPushLayer(&m_background, ResourceManagerGetTexture(&m_resources, "bg_layer_2"), 0.0f, 288.0f, 1088.0f, 600.0f, 0.3f);
		BackgroundPushLayer(&m_background, ResourceManagerGetTexture(&m_resources, "bg_layer_3"), 0.0f, 288.0f, 1088.0f, 600.0f, 0.0f);
	}

	~Frost()
	{
		BackgroundClear(&m_background);

		FontRendererDestroy();
		Primitives2DDestroy();
		BatchRenderer2DDestroy();
		Renderer2DDestroy();

		ResourceManagerDestroy(&m_resources);
	}
	
	void OnEvent(const Event& e) override
	{
		if (e.Type == EventType::KeyPressed)
		{
			switch (((KeyPressedEvent&)e).KeyCode)
			{
			case KEY_ESCAPE:
				Close();
				break;
			case KEY_F5:
				Pause();
				break;
			case KEY_F6:
				ToggleVsync();
				break;
			case KEY_F7:
				ToggleDebugMode();
				break;
			case KEY_F8:
				ToggleImGui();
				break;
			}
		}

		SceneManagerOnEvent(&m_sceneManager, e);
	}

	void OnUpdate(float deltaTime) override
	{
		// discard frames that took to long
		// if (deltaTime > 0.4f) return;

		BackgroundUpdate(&m_background, m_camera.position.x - m_camera.size.x / 2.0f, deltaTime);

		SceneManagerOnUpdate(&m_sceneManager, deltaTime);
	}

	void OnRender() override
	{
		BackgroundRender(&m_background, CameraGetViewProjectionPtr(&m_camera));

		SceneManagerOnRender(&m_sceneManager);
	}

	void OnRenderDebug() override
	{
		SceneManagerOnRenderDebug(&m_sceneManager);

		// debug info
		FontRendererRenderText(m_font, obelisk::format("FPS: %d", m_timer.FPS).c_str(), 0.0f, 32.0f, GetScreenMatPtr(), IGNIS_WHITE);
	}

	void OnImGui() override
	{
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
		
		auto player = SceneGetEntity(m_sceneManager.scene.get(), "player", 1);
		 
		ImGui::Text("Name: %s", player->GetName().c_str());
		auto position = player->GetPosition();
		ImGui::Text("Position: %4.2f, %4.2f", position.x, position.y);
		ImGui::Text("Precise Y: %f", position.y);
		
		ImGui::Separator();
		
		ImGui::Text("Scene: %s", m_sceneManager.sceneName.c_str());

		ImGui::End();

		SceneManagerOnImGui(&m_sceneManager);
	}
}; 

#define RUN_GAME 1

int main()
{
#if RUN_GAME
	Frost* game = new Frost();

	game->Run();

	delete game;
#else

	system("Pause");
#endif

	return 0;
}