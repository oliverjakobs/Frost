#include "Application.h"

#include "Scene/SceneManager.h"

class Frost : public Application
{
private:
	SceneManager m_sceneManager;

public:
	Frost() : Application("config.json")
	{
		// ---------------| Config|------------------------------------------
		Renderer::EnableBlend(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		Renderer::SetClearColor(0.2f, 0.2f, 0.2f, 1.0f);

		EnableDebugMode(true);
		EnableVsync(false);
		ToggleImGui();

		// ---------------| Load resources|----------------------------------
		ResourceManager::Load("res/resources.json");


		// ---------------| Scenes |-----------------------------------------
		m_sceneManager.RegisterScene("station", "res/scenes/station.json");
		m_sceneManager.RegisterScene("station2", "res/scenes/station2.json");

		m_sceneManager.ChangeScene("station");
	}

	~Frost()
	{

	}
	
	void OnEvent(Event& e) override
	{
		if (e.GetEventType() == EventType::KeyPressed)
		{
			KeyPressedEvent& keyPressed = (KeyPressedEvent&)e;

			switch (keyPressed.GetKeyCode())
			{
			case KEY_ESCAPE:
				Close();
				break;
			case KEY_F5:
				Pause();
				break;
			case KEY_F7:
				ToggleDebugMode();
				break;
			case KEY_F8:
				ToggleImGui();
				break;
			//case KEY_1:
			//	EventHandler::Throw<ChangeSceneEvent>("station");
			//	break;
			//case KEY_2:
			//	EventHandler::Throw<ChangeSceneEvent>("station2");
			//	break;
			}
		}

		m_sceneManager.OnEvent(e);
	}

	void OnUpdate() override
	{
		m_sceneManager.OnUpdate();
	}

	void OnRender() override
	{
		m_sceneManager.OnRender();

		// FPS
		ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f));
		ImGui::Begin("FPS", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoInputs | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoScrollbar);
		ImGui::Text("Fps: %d", Timer::GetFPS());
		ImGui::End();
	}

	void OnRenderDebug() override
	{
		m_sceneManager.OnRenderDebug();
	}

	void OnImGui() override
	{
		m_sceneManager.OnImGui();

		//Application
		ImGui::Begin("Application");

		ImGui::Text("Window size: %d, %d", GetWidth(), GetHeight());

		ImGui::Separator();

		ImGui::Text("View:");
		ImGui::Text("Position: %4.2f, %4.2f", View::GetX(), View::GetY());
		ImGui::Text("Dimension: %4.2f, %4.2f", View::GetWidth(), View::GetHeight());

		ImGui::End();

		//Entity
		ImGui::Begin("Player");

		Entity* entity = m_sceneManager.GetScene()->GetEntity("player");
		auto animation = entity->GetComponent<AnimationComponent>();

		ImGui::Text("Position: %4.2f, %4.2f", entity->GetPosition().x, entity->GetPosition().y);
		ImGui::Text("Direction: %s", Direction::ToString(entity->GetDirection()).c_str());
		ImGui::Text("Current Animation: %s", animation->GetCurrent().c_str());

		ImGui::End();
	}
}; 

#define RUN_GAME 1

int main()
{
	Logger::SetFormat("[%T] [%^%l%$] %v");
	Logger::SetLevel(LogLevel::Trace);

#if RUN_GAME
	Frost* game = new Frost();

	game->Run();

	delete game;
#else

	system("Pause");
#endif

	return 0;
}