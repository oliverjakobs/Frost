
#include "TilePhysics/Body.h"

#include "ECS/Entity.h"
#include "ECS/PlayerComponent.h"
#include "ECS/ImageComponent.h"
#include "ECS/InteractionComponent.h"

#include "Scene/SceneManager.h"

#include "Font/BitmapFont.h"

class Frost : public Scrapbook
{
private:
	Font* font;
public:
	Frost() : Scrapbook("TileMap", 1024, 800)
	{
		Renderer::EnableBlend(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		Renderer::SetClearColor(0.2f, 0.2f, 0.2f, 1.0f);

		// load resources
		ResourceManager::AddShader("shader", new Shader("res/shader/shader.vert", "res/shader/shader.frag"));

		setDebugMode(true);
		enableVsync(false);
		
		// ------------------------------------------------
		Scene* scene = new Scene(getWidthF(), getHeightF(), new TileMap("res/images/tiles.png", "res/maps/station.txt"));

		Entity* entity = new Entity("player", 400, 300, 20, 20);
		entity->addComponent(new PhysicsComponent(scene->getMap()->createBody(400, 300, 20, 30, BodyTypeDynamic)));
		entity->addComponent(new AnimationComponent(new Image("res/images/player.png", 40, 60, 4, 6),
			{
				AnimationDef("idle", new Animation(0, 4, 0.2f)),
				AnimationDef("walk", new Animation(6, 6, 0.125f)),
				AnimationDef("jump", new Animation(12, 3, 0.3f)),
				AnimationDef("fall", new Animation(18, 2, 0.4f))
			}));
		entity->addComponent(new PlayerComponent(400, 800));

		Entity* door = new Entity("door", 512, 64, 20, 20);
		door->addComponent(new ImageComponent(new Image("res/images/door.png", 46, 64)));
		door->addComponent(new InteractionComponent(40.0f, GLFW_KEY_W, []() { SceneManager::ChangeScene("train"); }));

		Entity* wall = new Entity("wall", 200, 64, 20, 200);
		wall->addComponent(new PhysicsComponent(scene->getMap()->createBody(200, 164, 10, 100, BodyTypeStatic)));
		wall->addComponent(new ImageComponent(new Image("res/images/door.png", 20, 200)));

		scene->addEntity(wall);
		scene->addEntity(door);
		scene->addEntity(entity);

		// ------------------------------------------------
		Scene* trainScene = new Scene(getWidthF(), getHeightF(), new TileMap("res/images/tiles.png", "res/maps/train.txt"));

		entity = new Entity("player", 400, 300, 20, 20);
		entity->addComponent(new PhysicsComponent(trainScene->getMap()->createBody(400, 300, 20, 30, BodyTypeDynamic)));
		entity->addComponent(new AnimationComponent(new Image("res/images/player.png", 40, 60, 4, 6),
			{
				AnimationDef("idle", new Animation(0, 4, 0.2f)),
				AnimationDef("walk", new Animation(6, 6, 0.125f)),
				AnimationDef("jump", new Animation(12, 3, 0.3f)),
				AnimationDef("fall", new Animation(18, 2, 0.4f))
			}));
		entity->addComponent(new PlayerComponent(400, 800));

		door = new Entity("door", 512, 64, 20, 20);
		door->addComponent(new ImageComponent(new Image("res/images/door.png", 46, 64)));
		door->addComponent(new InteractionComponent(40.0f, GLFW_KEY_W, []() { SceneManager::ChangeScene("station"); }));
		
		trainScene->addEntity(door);
		trainScene->addEntity(entity);

		SceneManager::AddScene("station", scene);
		SceneManager::AddScene("train", trainScene);

		// Font
		font = new BitmapFont("res/images/blocky_font.png", 20.0f, 28.0f, 2.0f);
	}

	~Frost()
	{
		SceneManager::Free();
	}

	void onInput() override
	{
		if (Input::KeyPressed(GLFW_KEY_ESCAPE))
			close();

		if (Input::KeyPressed(GLFW_KEY_F7))
			toggleDebugMode();

		SceneManager::OnInput();
	}

	void onUpdate() override
	{
		//setTitle(m_data.title + "  |  " + std::to_string(Timer::GetFPS()));

		SceneManager::OnUpdate();
	}

	void onRender() const override
	{
		SceneManager::OnRender();
	}

	void onRenderDebug() const override
	{
		SceneManager::OnRenderDebug();

		// Debug Info
		font->onRender(stringf("FPS: %d", Timer::GetFPS()), 2.0f, getHeight() - 30.0f, Renderer::GetScreenView(), "shader");
		font->onRender(stringf("Simulation Time: %4.2f ms", SceneManager::GetActiveScene()->getMap()->getSimulationTime()), 2.0f, 2.0f, Renderer::GetScreenView(), "shader");
	}
};

int main()
{
	Frost* game = new Frost();

	game->run();

	delete game;

	return 0;
}