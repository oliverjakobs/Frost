#include "Core/Application.h"

#include "ECS/Systems.h"

class Frost : public Application
{
private:
	entt::registry reg;

	TileMap* map;
public:
	Frost() : Application("Frost", 1024, 800)
	{
		// ---------------| Config|------------------------------------------
		Renderer::EnableBlend(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		Renderer::SetClearColor(0.2f, 0.2f, 0.2f, 1.0f);

		EnableDebugMode(true);
		EnableVsync(false);

		// ---------------| Load resources|----------------------------------
		ResourceManager::AddShader("shader", new Shader("res/shader/shader.vert", "res/shader/shader.frag"));

		ResourceManager::AddImage("player", new Image("res/images/player.png", 40, 60, 4, 6));
		ResourceManager::AddImage("door", new Image("res/images/door.png", 46, 64));
		ResourceManager::AddImage("wall", new Image("res/images/door.png", 20, 200));
		ResourceManager::AddImage("tileset", new Image("res/images/tiles.png", 32.0f, 32.0f, 1, 5));

		ResourceManager::AddFont("blocky", new BitmapFont("res/images/blocky_font.png", 20.0f, 28.0f, 2.0f));

		map = new TileMap(ResourceManager::GetImage("tileset"), "res/maps/station1.txt");

		auto entity = reg.create();
		reg.assign<TransformComponent>(entity, glm::vec2(400, 300), glm::vec2(0, 0));
		reg.assign<MovementComponent>(entity, 400.0f, 800.0f);
		reg.assign<PhysicsComponent>(entity, map->createBody(400, 300, 20, 30, BodyTypeDynamic), glm::vec2(0.0f, 30.0f));
		reg.assign<CameraComponent>(entity, Rect(glm::vec2(), map->getDimension() * map->getTileSize()), glm::vec2(0.0f, 30.0f));
		reg.assign<ImageComponent>(entity, ResourceManager::GetImage("player"));
		reg.assign<AnimationComponent>(entity, std::map<std::string, Animation>
		{
			AnimationDef("idle", Animation(0, 4, 0.2f)),
			AnimationDef("walk", Animation(6, 6, 0.125f)),
			AnimationDef("jump", Animation(12, 3, 0.3f)),
			AnimationDef("fall", Animation(18, 2, 0.4f))
		});
	}

	~Frost()
	{
		delete map;
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
		map->onUpdate();

		PlayerSystem::Tick(reg);
		TilePhysicsSystem::Tick(reg);
		AnimationSystem::Tick(reg);
	}

	void OnRender() override
	{
		map->onRender();

		ImageRenderSystem::Tick(reg);
	}

	void OnRenderDebug() const override
	{
		map->onRenderDebug();

		// Debug Info
		ImGui::Begin("Info");
		ImGui::Text("FPS: %d", Timer::GetFPS());
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