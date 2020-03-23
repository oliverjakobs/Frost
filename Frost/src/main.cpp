#include "Application.hpp"

#include "SceneManager.hpp"

class Frost : public Application
{
private:
	std::shared_ptr<SceneManager> m_sceneManager;

	ignis_font m_font;

	ignis_texture* m_texture1;
	ignis_texture* m_texture2;

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

		ignisLoadFont(&m_font, "res/fonts/OpenSans.ttf", 32.0f);

		EnableDebugMode(true);
		EnableImGui(true);
		EnableVsync(false);

		auto camera = std::make_shared<OrthographicCamera>(glm::vec3(m_width / 2.0f, m_height / 2.0f, 0.0f), glm::vec2(m_width, m_height));
		m_sceneManager = std::make_shared<SceneManager>(camera, 32.0f, 4);
		m_sceneManager->RegisterScenes("res/templates/scenes/register.json");
		m_sceneManager->ChangeScene("scene");

		m_texture1 = ignisCreateTexture("res/textures/box.png", 1, 1, true);
		m_texture2 = ignisCreateTexture("res/textures/door.png", 1, 1, true);
	}

	~Frost()
	{
		ignisDeleteFont(&m_font);

		FontRendererDestroy();
		Primitives2DDestroy();
		BatchRenderer2DDestroy();
		Renderer2DDestroy();

		ignisDestroyTexture(m_texture1);
		ignisDestroyTexture(m_texture2);
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

		m_sceneManager->OnEvent(e);
	}

	void OnUpdate(float deltaTime) override
	{
		// discard frames that took to long
		// if (deltaTime > 0.4f) return;

		m_sceneManager->OnUpdate(deltaTime);
	}

	void OnRender() override
	{
		m_sceneManager->OnRender();

		BatchRenderer2DStart(m_sceneManager->GetCamera()->GetViewProjectionPtr());

		int texture = 0;
		for (float x = 32.0f; x < m_sceneManager->GetScene()->GetWidth(); x += 96.0f)
		{
			BatchRenderer2DRenderTexture((texture++ % 2 == 0) ? m_texture1 : m_texture2, x, 32.0f, 64.0f, 64.0f);
		}

		BatchRenderer2DFlush();
	}

	void OnRenderDebug() override
	{
		m_sceneManager->OnRenderDebug();

		// debug info
		FontRendererRenderText(&m_font, obelisk::format("FPS: %d", m_timer.FPS).c_str(), 0.0f, 32.0f, GetScreenMatPtr(), IGNIS_WHITE);
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
		
		auto player = m_sceneManager->GetScene()->GetEntity("player");
		 
		ImGui::Text("Name: %s", player->GetName().c_str());
		auto position = player->GetPosition();
		ImGui::Text("Position: %4.2f, %4.2f", position.x, position.y);
		ImGui::Text("Precise Y: %f", position.y);
		
		ImGui::Separator();
		
		ImGui::Text("Scene: %s", m_sceneManager->GetSceneName().c_str());

		ImGui::End();

		m_sceneManager->OnImGui();
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