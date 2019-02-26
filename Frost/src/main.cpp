#include "TilePhysics/Tilemap.h"

#include "Font/FontRenderer.h"

#include "Systems.h"

class Frost : public Scrapbook
{
private:
	unique_ptr<Font> font;

	TileMap* map;

	ECS ecs;

	PlayerControlSystem* playerSystem;
	TilePhysicsSystem* physicsSystem;
	ImageRenderSystem* renderSystem;

	ECSSystemList logicSystems;
	ECSSystemList renderSystems;
public:
	Frost() : Scrapbook("TileMap", 1024, 800)
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
		
		//// ---------------| Create entities|---------------------------------
		//Entity* entity = EntityManager::CreateEntity("player", 0, 0, 20, 20);
		//entity->addComponent(new PhysicsComponent({ 20, 30, BodyTypeDynamic }));
		//entity->addComponent(new AnimationComponent(ResourceManager::GetImage("player"),
		//	{
		//		AnimationDef("idle", Animation(0, 4, 0.2f)),
		//		AnimationDef("walk", Animation(6, 6, 0.125f)),
		//		AnimationDef("jump", Animation(12, 3, 0.3f)),
		//		AnimationDef("fall", Animation(18, 2, 0.4f))
		//	}));
		//entity->addComponent(new PlayerComponent(400, 800));

		//Entity* door = EntityManager::CreateEntity("door", 512, 64, 20, 20);
		//door->addComponent(new ImageComponent(ResourceManager::GetImage("door")));

		//Entity* wall = EntityManager::CreateEntity("wall", 200, 64, 20, 200);
		//wall->addComponent(new PhysicsComponent({ 10, 100, BodyTypeStatic }));
		//wall->addComponent(new ImageComponent(ResourceManager::GetImage("wall")));
		//
		//// ---------------| Create Scenes|-----------------------------------
		//SceneManager::AddScene("station1", new SceneStation1());

		//Scene* scene = new Scene(new TileMap(ResourceManager::GetImage("tileset"), "res/maps/station2.txt"));
		//
		//scene->addEntity(EntityManager::GetEntity("door")->addComponent(new InteractionComponent(0.0f, GLFW_KEY_W, []() { SceneManager::ChangeScene("station1"); })));
		//scene->addEntity(EntityManager::GetEntity("player"), 512, 64);

		//SceneManager::AddScene("station2", scene);

		map = new TileMap(ResourceManager::GetImage("tileset"), "res/maps/station1.txt");

		playerSystem = new PlayerControlSystem();
		physicsSystem = new TilePhysicsSystem(map);
		renderSystem = new ImageRenderSystem();

		PositionComponent posComp;
		posComp.position = glm::vec2(400, 300);

		MovementComponent moveComp;
		moveComp.velocity = glm::vec2();
		moveComp.movementSpeed = 400.0f;
		moveComp.jumpPower = 800.0f;

		PhysicsComponent physComp;
		physComp.body = map->createBody(400, 300, 20, 30, BodyTypeDynamic);
		physComp.bodyPos = glm::vec2(0.0f, 30.0f);

		ImageComponent imageComp;
		imageComp.image = ResourceManager::GetImage("player");

		ecs.createEntity(posComp, moveComp, physComp, imageComp);

		logicSystems.addSystem(*playerSystem);
		logicSystems.addSystem(*physicsSystem);
		renderSystems.addSystem(*renderSystem);
	}

	~Frost()
	{
		delete map;

		delete playerSystem;
		delete physicsSystem;
		delete renderSystem;
	}

	void onInput() override
	{
		if (Input::KeyPressed(GLFW_KEY_ESCAPE))
			close();

		if (Input::KeyPressed(GLFW_KEY_F7))
			toggleDebugMode();

		//if (Input::KeyPressed(GLFW_KEY_X))
		//	SceneManager::GetActiveScene()->deleteEntity("wall");

		//SceneManager::OnInput();
	}

	void onUpdate() override
	{
		//SceneManager::OnUpdate();

		map->onUpdate();

		ecs.updateSystems(logicSystems, Timer::GetDeltaTime());
	}

	void onRender() override
	{
		//SceneManager::OnRender();
		map->onRender();

		ecs.updateSystems(renderSystems, 0.0f);
	}

	void onRenderDebug() const override
	{
		//SceneManager::OnRenderDebug();
		map->onRenderDebug();

		// Debug Info
		FontRenderer::RenderText("blocky", stringf("FPS: %d", Timer::GetFPS()), 2.0f, getHeight() - 30.0f, Renderer::GetScreenView(), "shader");
		//FontRenderer::RenderText("blocky", stringf("Simulation Time: %4.2f ms", SceneManager::GetActiveScene()->getMap()->getSimulationTime()), 2.0f, 2.0f, Renderer::GetScreenView(), "shader");
	}
};

int main()
{
	Frost* game = new Frost();

	game->run();

	delete game;

	return 0;
}