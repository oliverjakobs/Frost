#include "Core/Application.h"

#include "Scene/SceneManager.h"
#include "Scene/EntityMananger.h"

class Frost : public Application
{
private:
	unsigned int entity;

public:
	Frost() : Application("Frost", 1024, 800)
	{
		// ---------------| Config|------------------------------------------
		Renderer::EnableBlend(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		Renderer::SetClearColor(0.2f, 0.2f, 0.2f, 1.0f);

		EnableDebugMode(true);
		EnableVsync(false);

		// ---------------| Load resources|----------------------------------
		ResourceManager::Load("res/resources.xml");

		Scene* scene = new Scene("station", new TileMap(ResourceManager::GetImage("tileset"), "res/maps/station1.txt"));

		entity = EntityManager::CreateEntity(scene, "res/scripts/entity.xml");

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

		////Entity
		//ImGui::Begin("Entity");

		//auto trans = SceneManager::GetActiveScene()->GetRegistry().get<TransformComponent>(entity);

		//ImGui::Text("Position: %f, %f", trans.position.x, trans.position.y);

		//ImGui::End();
	}
};


int main()
{
	Logger::SetFormat("[%T] [%^%l%$]: %v");
	Logger::SetLevel(LogLevel::Trace);

	Frost* game = new Frost();

	game->Run();

	delete game;

	return 0;
}