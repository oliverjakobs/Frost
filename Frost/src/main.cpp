#include "Core/Application.h"

#include "Scene/SceneManager.h"
#include "Scene/EntityMananger.h"

class Frost : public Application
{
private:
	ScriptSystem* luaSystem;

public:
	Frost() : Application("Frost", 1024, 800)
	{
		// ---------------| Config|------------------------------------------
		Renderer::EnableBlend(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		Renderer::SetClearColor(0.2f, 0.2f, 0.2f, 1.0f);

		EnableDebugMode(true);
		EnableVsync(false);

		// ---------------| Load resources|----------------------------------
		ResourceManager::Load("res/resources.json");

		Scene* scene = new Scene("station", new TileMap(ResourceManager::GetImage("tileset"), "res/maps/station1.txt"));

		luaSystem = new ScriptSystem(scene->GetRegistry());

		scene->AddEntity("player", "res/scripts/player.json");

		SceneManager::AddScene(scene);
	}

	~Frost()
	{
		delete luaSystem;
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
			
		luaSystem->Tick(SceneManager::GetActiveScene()->GetRegistry());
	}

	void OnRender() override
	{
		SceneManager::OnRender();
	}

	void OnRenderDebug() override
	{
		SceneManager::OnRenderDebug();

		// Debug Info
		ImGui::Begin("Info");
		ImGui::Text("FPS: %d", Timer::GetFPS());

		static bool select = false;
		ImGui::Checkbox("Vsync", &select);
		EnableVsync(select);
		ImGui::End();

		//Entity
		ImGui::Begin("Player");

		unsigned int entity = SceneManager::GetActiveScene()->GetEntity("player");
		auto transform = SceneManager::GetActiveScene()->GetRegistry().get<TransformComponent>(entity);
		auto animation = SceneManager::GetActiveScene()->GetRegistry().get<AnimationComponent>(entity);

		ImGui::Text("Position: %4.2f, %4.2f", transform.position.x, transform.position.y);
		ImGui::Text("Current Animation: %s", animation.currentAnimation.c_str());

		ImGui::End();
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