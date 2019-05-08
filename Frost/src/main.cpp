#include "Core/Application.h"

#include "ecsComponents/TransformComponent.h"
#include "ecsComponents/ImageComponent.h"

#include "Random/XorShift128.h"
#include "Random/MersenneTwister.h"

#include "Console/Console.h"

class Frost : public Application
{
private:
	Console console;
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

		Console::Init(getContext());
	}

	~Frost()
	{

	}

	void onInput() override
	{
		if (Input::KeyPressed(KEY_ESCAPE))
			close();

		if (Input::KeyPressed(KEY_F7))
			toggleDebugMode();

		if (Input::KeyPressed(KEY_F5))
			Console::Toggle(&console);
	}

	void onUpdate() override
	{
		Console::Update();
	}

	void onRender() override
	{

	}

	void onRenderDebug() const override
	{
		// Debug Info
		Renderer::RenderString(ResourceManager::GetFont("blocky"), stringf("FPS: %d", Timer::GetFPS()), 2.0f, getHeight() - 30.0f, Renderer::GetScreenView(), "shader");

		Console::Render(2.0f, 2.0f);
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