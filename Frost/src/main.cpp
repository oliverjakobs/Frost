#include "Core/Application.h"

#include "ecsComponents/TransformComponent.h"
#include "ecsComponents/ImageComponent.h"

class Frost : public Application
{
private:

public:
	Frost() : Application("Frost", 1024, 800)
	{
		// ---------------| Config|------------------------------------------
		Renderer::EnableBlend(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		Renderer::SetClearColor(0.2f, 0.2f, 0.2f, 1.0f);

		enableDebugMode(true);
		enableVsync(false);

		// ---------------| Load resources|----------------------------------
		ResourceManager::AddShader("shader", new Shader("res/shader/shader.vert", "res/shader/shader.frag"));

		ResourceManager::AddImage("player", new Image("res/images/player.png", 40, 60, 4, 6));
		ResourceManager::AddImage("door", new Image("res/images/door.png", 46, 64));
		ResourceManager::AddImage("wall", new Image("res/images/door.png", 20, 200));
		ResourceManager::AddImage("tileset", new Image("res/images/tiles.png", 32.0f, 32.0f, 1, 5));

		ResourceManager::AddFont("blocky", new BitmapFont("res/images/blocky_font.png", 20.0f, 28.0f, 2.0f));

		DEBUG_TRACE("TransformComponent: {0}", TransformComponent::ID);
		DEBUG_TRACE("ImageComponent:     {0}", ImageComponent::ID);
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
			}
		}
	}

	void onUpdate() override
	{

	}

	void onRender() const override
	{

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