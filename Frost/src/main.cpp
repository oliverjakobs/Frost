#include "Core/Application.h"

#include "Scene/SceneManager.h"
#include "Scene/EntityMananger.h"

class Frost : public Application
{
private:

public:
	Frost() : Application("config.json")
	{
		// ---------------| Config|------------------------------------------
		Renderer::EnableBlend(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		Renderer::SetClearColor(0.2f, 0.2f, 0.2f, 1.0f);

		EnableDebugMode(true);
		EnableVsync(false);

		// ---------------| Load resources|----------------------------------
		ResourceManager::Load("res/resources.json");
		ResourceManager::AddShader("instanced", new Shader("res/shader/instanced.vert", "res/shader/instanced.frag"));

		Scene* scene = new Scene("station", new TileMap("res/maps/station.json"));

		scene->AddEntity("player", "res/scripts/player.json");

		SceneManager::AddScene(scene);
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
			case KEY_F7:
				ToggleDebugMode();
				break;
			}
		}
	}

	void OnUpdate() override
	{
		SceneManager::OnUpdate();
	}

	void OnRender() override
	{
		SceneManager::OnRender();

		ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f));
		ImGui::Begin("FPS", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoInputs | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoScrollbar);
		ImGui::Text("Fps: %d", Timer::GetFPS());
		ImGui::End();
	}

	void OnRenderDebug() override
	{
		SceneManager::OnRenderDebug();

		// ImGui

#if 1
		//Entity
		ImGui::Begin("Player");

		unsigned int entity = SceneManager::GetActiveScene()->GetEntity("player");
		auto transform = SceneManager::GetActiveScene()->GetRegistry().get<TransformComponent>(entity);
		auto animation = SceneManager::GetActiveScene()->GetRegistry().get<AnimationComponent>(entity);

		ImGui::Text("Position: %4.2f, %4.2f", transform.position.x, transform.position.y);
		ImGui::Text("Current Animation: %s", animation.currentAnimation.c_str());

		ImGui::End();
#endif
	}
};


int main()
{
	Logger::SetFormat("[%T] [%^%l%$]: %v");
	Logger::SetLevel(LogLevel::Trace);

#if 1
	Frost* game = new Frost();

	game->Run();

	delete game;
#else
	

	system("Pause");
#endif

	return 0;
}