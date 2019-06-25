#include "Core/Application.h"

#include "Scene/SceneManager.h"
#include "Scene/EntityMananger.h"

class Frost : public Application
{
private:

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

		auto entity = EntityManager::CreateEntity(scene, "res/scripts/entity.xml");
		scene->GetRegistry().assign<PhysicsComponent>(entity, scene->GetMap()->createBody(400, 300, 20, 30, BodyTypeDynamic), glm::vec2(0.0f, 30.0f));
		scene->GetRegistry().assign<CameraComponent>(entity, Rect(glm::vec2(), scene->GetMap()->getDimension() * scene->GetMap()->getTileSize()), glm::vec2(0.0f, 30.0f));
		scene->GetRegistry().assign<ImageComponent>(entity, ResourceManager::GetImage("player"));
		scene->GetRegistry().assign<AnimationComponent>(entity, std::map<std::string, Animation>
		{
			AnimationDef("idle", Animation(0, 4, 0.2f)),
			AnimationDef("walk", Animation(6, 6, 0.125f)),
			AnimationDef("jump", Animation(12, 3, 0.3f)),
			AnimationDef("fall", Animation(18, 2, 0.4f))
		});

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