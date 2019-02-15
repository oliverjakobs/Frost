
#include "TilePhysics/Body.h"

#include "ECS/Entity.h"
#include "ECS/PlayerComponent.h"
#include "ECS/ImageComponent.h"
#include "ECS/InteractionComponent.h"

#include "Scene/SceneManager.h"
#include "Scene/EntityManager.h"

#include "Font/BitmapFont.h"

class Frost : public Scrapbook
{
private:
	unique_ptr<Font> font;
public:
	Frost() : Scrapbook("TileMap", 1024, 800)
	{
		Renderer::EnableBlend(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		Renderer::SetClearColor(0.2f, 0.2f, 0.2f, 1.0f);

		// load resources
		ResourceManager::AddShader("shader", new Shader("res/shader/shader.vert", "res/shader/shader.frag"));
		ResourceManager::AddImage("player", new Image("res/images/player.png", 40, 60, 4, 6));
		ResourceManager::AddImage("door", new Image("res/images/door.png", 46, 64));
		ResourceManager::AddImage("wall", new Image("res/images/door.png", 20, 200));

		setDebugMode(true);
		enableVsync(false);
		
		// ------------------------------------------------
		Entity* entity = EntityManager::CreateEntity("player", 0, 0, 20, 20);
		entity->addComponent(new PhysicsComponent({ 20, 30, BodyTypeDynamic }));
		entity->addComponent(new AnimationComponent(ResourceManager::GetImage("player"),
			{
				AnimationDef("idle", Animation(0, 4, 0.2f)),
				AnimationDef("walk", Animation(6, 6, 0.125f)),
				AnimationDef("jump", Animation(12, 3, 0.3f)),
				AnimationDef("fall", Animation(18, 2, 0.4f))
			}));
		entity->addComponent(new PlayerComponent(400, 800));

		Entity* door = EntityManager::CreateEntity("door", 512, 64, 20, 20);
		door->addComponent(new ImageComponent(ResourceManager::GetImage("door")));

		Entity* wall = EntityManager::CreateEntity("wall", 200, 64, 20, 200);
		wall->addComponent(new PhysicsComponent({ 10, 100, BodyTypeStatic }));
		wall->addComponent(new ImageComponent(ResourceManager::GetImage("wall")));
		
		// ------------------------------------------------
		Scene* scene = new Scene(getWidthF(), getHeightF(), new TileMap("res/images/tiles.png", "res/maps/station.txt"));

		scene->addEntity(EntityManager::GetEntity("wall"));
		scene->addEntity(EntityManager::GetEntity("door")->addComponent(new InteractionComponent(0.0f, GLFW_KEY_W, []() { SceneManager::ChangeScene("train"); })));
		scene->addEntity(EntityManager::GetEntity("player"), 400, 300);

		Scene* trainScene = new Scene(getWidthF(), getHeightF(), new TileMap("res/images/tiles.png", "res/maps/train.txt"));
		
		trainScene->addEntity(EntityManager::GetEntity("door")->addComponent(new InteractionComponent(0.0f, GLFW_KEY_W, []() { SceneManager::ChangeScene("station"); })));
		trainScene->addEntity(EntityManager::GetEntity("player"), 512, 64);

		SceneManager::AddScene("station", scene);
		SceneManager::AddScene("train", trainScene);

		// Font
		font = unique_ptr<BitmapFont>(new BitmapFont("res/images/blocky_font.png", 20.0f, 28.0f, 2.0f));
	}

	~Frost()
	{
		SceneManager::Free();
		EntityManager::Free();
	}

	void onInput() override
	{
		if (Input::KeyPressed(GLFW_KEY_ESCAPE))
			close();

		if (Input::KeyPressed(GLFW_KEY_F7))
			toggleDebugMode();

		if (Input::KeyPressed(GLFW_KEY_X))
			SceneManager::GetActiveScene()->deleteEntity("wall");

		SceneManager::OnInput();
	}

	void onUpdate() override
	{
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