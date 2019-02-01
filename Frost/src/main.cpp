#include "Tilemap.h"

#include <iostream>

#include "Scrapbook\Input.h"
#include "Scrapbook\ResourceManager.h"

#include "ECS\Entity.h"
#include "ECS\PlayerComponent.h"
#include "ECS\ImageComponent.h"

#include "Scrapbook\Util\utils.h"

#include "Scene\Scene.h"

using namespace sb;

class Frost : public Scrapbook
{
private:
	Scene* scene;
public:
	Frost() : Scrapbook("TileMap", 1024, 800)
	{
		Renderer::EnableBlend(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		// load resources
		ResourceManager::Load();
		ResourceManager::AddShader("shader", new Shader("res/shader/shader.vert", "res/shader/shader.frag"));

		setClearColor(0.2f, 0.2f, 0.2f, 1.0f);
		setDebugMode(true);
		
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
		scene = new Scene(getWidth(), getHeight(), new TileMap("res/images/box.png", "res/maps/map.txt"));

		Entity* entity = new Entity("player", 400, 300, 20, 20);
		entity->addComponent(new PhysicsComponent(scene->getMap()->createBody(400, 300, 40, 60)));
		entity->addComponent(new ImageComponent(new Image("res/images/player.png", 40, 60)));
		entity->addComponent(new PlayerComponent(400, 800));

		Entity* door = new Entity("door", 512, 64, 20, 20);
		door->addComponent(new ImageComponent(new Image("res/images/player.png", 46, 64)));

		scene->addEntity(entity);
		scene->addEntity(door);
	}

	~Frost()
	{
		ResourceManager::Free();
	}

	void onInput() override
	{
		if (Input::KeyPressed(GLFW_KEY_ESCAPE))
			close();

		if (Input::KeyPressed(GLFW_KEY_F7))
			toggleDebugMode();
		
		if (Input::KeyPressed(GLFW_KEY_F1))
			glViewport(0.0f, 0.0f, getWidth(), getHeight());

		if (Input::KeyPressed(GLFW_KEY_F2))
			glViewport(0.0f, 0.0f, getWidth() / 2.0f, getHeight() / 2.0f);
		
		scene->onInput();
	}

	void onUpdate() override
	{
		Input::OnUpdate();

		setTitle(m_data.title + "  |  " + std::to_string(Timer::GetFPS()));

		scene->onUpdate();
	}

	void onRender() override
	{
		scene->onRender();
	}

	void onRenderDebug() override
	{
		scene->onRenderDebug();
	}
};

int main()
{
	Frost* game = new Frost();

	game->run();

	delete game;
	
	return 0;
}