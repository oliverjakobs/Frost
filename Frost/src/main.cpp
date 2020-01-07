#include "Application.h"

#include "SceneManager.h"

#include "json/TemplateLoader.h"

using namespace ignis;

class Frost : public Application
{
private:
	std::shared_ptr<SceneManager> m_sceneManager;

public:
	Frost() 
		: Application("Frost", 1024, 800)
	{
		// ---------------| Config |------------------------------------------
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		//glEnable(GL_DEPTH_TEST);
		glClearColor(0.2f, 0.2f, 0.2f, 1.0f);

		Renderer2D::Init(std::make_shared<Shader>("res/shaders/renderer2D.vert", "res/shaders/renderer2D.frag"));
		Primitives2D::Init(std::make_shared<Shader>("res/shaders/lines.vert", "res/shaders/lines.frag"));

		FontRenderer::Init(std::make_shared<Shader>("res/shaders/font.vert", "res/shaders/font.frag"));
		FontRenderer::AddFont("font", std::make_shared<Font>("res/fonts/OpenSans.ttf", 32.0f));

		EnableDebugMode(true);
		EnableImGui(true);
		EnableVsync(false);

		m_sceneManager = std::make_shared<SceneManager>(std::make_shared<OrthographicCamera>(glm::vec3(m_width / 2.0f, m_height / 2.0f, 0.0f), glm::vec2(m_width, m_height)));

		m_sceneManager->LoadScene("res/templates/scenes/scene.json");
	}

	~Frost()
	{
		Renderer2D::Destroy();
		FontRenderer::Destroy();
		Primitives2D::Destroy();
	}
	
	void OnEvent(const Event& e) override
	{
		if (e.GetType() == EventType::KeyPressed)
		{
			switch (((KeyPressedEvent&)e).GetKeyCode())
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
		if (deltaTime > 0.4f) return;

		m_sceneManager->OnUpdate(deltaTime);
	}

	void OnRender() override
	{
		m_sceneManager->OnRender();
	}

	void OnRenderDebug() override
	{
		m_sceneManager->OnRenderDebug();

		// debug info
		FontRenderer::RenderText("font", obelisk::format("FPS: %d", m_timer.FPS), 0.0f, 32.0f, ignisScreenMat(), WHITE);
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