#include "Core/Application.h"

#include "TilePhysics/Tilemap.h"

#include "Font/FontRenderer.h"

#include "Maths/Visibility.h"

#include "Utility/range.h"

#include "Utility/String/StringHash.h"

#include "Log/Logger.h"

#include "ecsComponents/TransformComponent.h"


glm::vec2 screenToWorld(const glm::vec2& pos, const View& v) 
{
	return glm::vec2(pos.x, v.h - pos.y);
}

class Frost : public Application
{
private:


public:
	Frost() : Application("Frost", 1024, 800)
	{
		// ---------------| Config|------------------------------------------
		Renderer::EnableBlend(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		Renderer::SetClearColor(0.2f, 0.2f, 0.2f, 1.0f);

		setDebugMode(true);
		enableVsync(false);

		// ---------------| Load resources|----------------------------------
		ResourceManager::AddShader("shader", new Shader("res/shader/shader.vert", "res/shader/shader.frag"));

		ResourceManager::AddImage("player", new Image("res/images/player.png", 40, 60, 4, 6));
		ResourceManager::AddImage("door", new Image("res/images/door.png", 46, 64));
		ResourceManager::AddImage("wall", new Image("res/images/door.png", 20, 200));
		ResourceManager::AddImage("tileset", new Image("res/images/tiles.png", 32.0f, 32.0f, 1, 5));

		FontRenderer::AddFont("blocky", new BitmapFont("res/images/blocky_font.png", 20.0f, 28.0f, 2.0f));

		DEBUG_TRACE("TransformComponent: {0}", TransformComponent::ID);
	}

	~Frost()
	{

	}

	void onInput() override
	{
		if (Input::KeyPressed(GLFW_KEY_ESCAPE))
			close();

		if (Input::KeyPressed(GLFW_KEY_F7))
			toggleDebugMode();
	}

	void onUpdate() override
	{

	}

	void onRender() override
	{

	}

	void onRenderDebug() const override
	{
		// Debug Info
		FontRenderer::RenderText("blocky", stringf("FPS: %d", Timer::GetFPS()), 2.0f, getHeight() - 30.0f, Renderer::GetScreenView(), "shader");
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