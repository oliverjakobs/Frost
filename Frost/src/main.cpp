#include <entt/entt.hpp>

#include "Core/Application.h"

#include "ECS/Components.h"

class Frost : public Application
{
private:
	entt::registry reg;

public:
	Frost() : Application("Frost", 1024, 800)
	{
		// ---------------| Config|------------------------------------------
		Renderer::EnableBlend(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		Renderer::SetClearColor(0.2f, 0.2f, 0.2f, 1.0f);

		enableDebugMode(true);
		enableVsync(true);

		// ---------------| Load resources|----------------------------------
		ResourceManager::AddShader("shader", new Shader("res/shader/shader.vert", "res/shader/shader.frag"));

		ResourceManager::AddImage("player", new Image("res/images/player.png", 40, 60, 4, 6));
		ResourceManager::AddImage("door", new Image("res/images/door.png", 46, 64));
		ResourceManager::AddImage("wall", new Image("res/images/door.png", 20, 200));
		ResourceManager::AddImage("tileset", new Image("res/images/tiles.png", 32.0f, 32.0f, 1, 5));

		ResourceManager::AddFont("blocky", new BitmapFont("res/images/blocky_font.png", 20.0f, 28.0f, 2.0f));

		auto entity = reg.create();
		reg.assign<ImageComponent>(entity, ResourceManager::GetImage("player"));
		reg.assign<TransformComponent>(entity, glm::vec2(50, 50), glm::vec2(0, 0));
		reg.assign<VelocityComponent>(entity, glm::vec2());
	}

	~Frost()
	{

	}
	
	void onEvent(Event& e)
	{
		if (e.GetEventType() == EventType::KeyPressed)
		{
			KeyPressedEvent& keyPressed = (KeyPressedEvent&)e;

			switch (keyPressed.GetKeyCode())
			{
			case KEY_ESCAPE:
				close();
				break;
			case KEY_F7:
				toggleDebugMode();
				break;

			//movement
			case KEY_W:
				reg.view<VelocityComponent>().each([](auto entity, auto &vel) { vel.velocity.y = 100; });
				break;
			case KEY_S:
				reg.view<VelocityComponent>().each([](auto entity, auto &vel) { vel.velocity.y = -100; });
				break;
			case KEY_D:
				reg.view<VelocityComponent>().each([](auto entity, auto &vel) { vel.velocity.x = 100; });
				break;
			case KEY_A:
				reg.view<VelocityComponent>().each([](auto entity, auto &vel) { vel.velocity.x = -100; });
				break;
			}
		}
	}

	void onUpdate() override
	{
		reg.view<TransformComponent, VelocityComponent>().each([](auto entity, auto& trans, auto &vel)
		{ 
			trans.position += vel.velocity * Timer::GetDeltaTime();
			vel.velocity = glm::vec2();
		});
	}

	void onRender() override
	{
		reg.view<ImageComponent, TransformComponent>().each([](auto entity, auto& img, auto& trans)
		{
			img.image->renderF(trans.position, 0, Renderer::GetView().mat, "shader");
		});
	}

	void onRenderDebug() const override
	{
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

	game->run();

	delete game;

	return 0;
}