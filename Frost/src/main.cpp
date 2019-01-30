#include "Tilemap.h"

#include <iostream>

#include "Input.h"

#include "ECS\Entity.h"
#include "ECS\PlayerComponent.h"
#include "ECS\ImageComponent.h"

#include "Scrapbook\Util\utils.h"

using namespace sb;

class TileMapExample : public Scrapbook
{
private:
	TileMap* map;
	Entity* entity;

public:
	TileMapExample() : Scrapbook("TileMap", 1024, 800)
	{
		setClearColor(0.2f, 0.2f, 0.2f, 1.0f);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glfwSwapInterval(0);

		glfwSetKeyCallback(getContext(), [](GLFWwindow* window, int key, int scancode, int action, int mods)
		{
			switch (action)
			{
			case GLFW_PRESS:
				Input::SetKeyState(key, true, false);
				break;
			case GLFW_RELEASE:
				Input::SetKeyState(key, false, true);
				break;
			}
		});

		// ------------------------------------------------
		Renderer::Init(0.0f, 0.0f, (float)m_data.width, (float)m_data.height);
		ResourceManager::AddShader("shader", new Shader("res/shader/shader.vert", "res/shader/shader.frag"));

		map = new TileMap("res/images/box.png", "res/maps/map.txt");

		entity = new Entity("player", 400, 300, 20, 20);

		entity->addComponent(new PhysicsComponent(map->createBody(400, 300, 30, 60)));
		entity->addComponent(new ImageComponent(new Image("res/images/player.png", 40, 60)));
		entity->addComponent(new PlayerComponent(400, 800));
	}

	void onInput() override
	{

		if (Input::KeyPressed(GLFW_KEY_ESCAPE))
			glfwSetWindowShouldClose(getContext(), true);
		
		if (Input::KeyPressed(GLFW_KEY_F1))
			glViewport(0.0f, 0.0f, getWidth(), getHeight());

		if (Input::KeyPressed(GLFW_KEY_F2))
			glViewport(0.0f, 0.0f, getWidth() / 2.0f, getHeight() / 2.0f);

		entity->onInput();
	}

	void onUpdate() override
	{
		Input::OnUpdate();

		setTitle(m_data.title + "  |  " + std::to_string(Timer::GetFPS()));
		
		map->onUpdate();
		entity->onUpdate();
	}

	void onRender() override
	{
		map->onRender();

		entity->onRender();
	}
};

int main()
{
	TileMapExample game;

	game.run();

	return 0;
}