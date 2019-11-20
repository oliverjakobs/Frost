#include "Application.h"

#include "Tile/World.h"

#include "Entity/Components.h"

using namespace ignis;
using namespace tile;

class Frost : public Application
{
private:
	ignis::RenderState m_renderState;

	std::shared_ptr<World> m_world;
	std::shared_ptr<Body> m_body;
	std::shared_ptr<Entity> m_entity;

	Font font = Font("res/fonts/OpenSans.ttf", 32.0f);
	Shader fontShader = Shader("res/shaders/font.vert", "res/shaders/font.frag");
public:
	Frost() 
		// : Application("config.json")
		: Application("Frost", 1024, 800)
	{
		// ---------------| Config |------------------------------------------
		m_renderState.SetBlend(true, GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glClearColor(0.2f, 0.2f, 0.2f, 1.0f);

		Renderer2D::Init(std::make_shared<Shader>("res/shaders/renderer2D.vert", "res/shaders/renderer2D.frag"));
		Primitives2D::Init(std::make_shared<Shader>("res/shaders/lines.vert", "res/shaders/lines.frag"));

		EnableDebugMode(true);
		EnableImGui(true);
		EnableVsync(false);

		m_world = std::make_shared<World>("");

		m_body = m_world->CreateBody(600.0f, 200.0f, 20.0f, 30.0f, BodyType::BODY_DYNAMIC);

		m_entity = std::make_shared<Entity>("Entity", glm::vec2(400.0f, 200.0f), glm::vec2(20.0f, 20.0f));

		m_entity->AddComponent(new PhysicsComponent(m_world->CreateBody(400.0f, 200.0f, 20.0f, 30.0f, BodyType::BODY_DYNAMIC), glm::vec2(0.0f, 30.0f)));
		m_entity->AddComponent(new TextureComponent(std::make_shared<Texture>("res/textures/player.png", 4, 6), 40.0f, 60.0f));

		std::map<std::string, Animation> animations
		{
			{ "idle", Animation(0, 4, 0.2) },
			{ "walk", Animation(6, 6, 0.125) },
			{ "jump", Animation(12, 3, 0.3) },
			{ "fall", Animation(18, 2, 0.4) }
		};

		m_entity->AddComponent(new AnimationComponent(animations));
		m_entity->AddComponent(new PlayerComponent(400.0f, 800.0f));
	}

	~Frost()
	{

	}
	
	void OnEvent(Event& e) override
	{
		if (e.GetType() == EventType::KeyPressed)
		{
			KeyPressedEvent& keyPressed = (KeyPressedEvent&)e;

			switch (keyPressed.GetKeyCode())
			{
			case KEY_ESCAPE:
				Close();
				break;
			case KEY_F5:
				Pause();
				break;
			case KEY_F7:
				ToggleDebugMode();
				break;
			case KEY_F8:
				ToggleImGui();
				break;
			}
		}
	}

	void OnUpdate(float deltaTime) override
	{
		m_entity->OnUpdate(deltaTime);
		m_body->Tick(deltaTime);
	}

	void OnRender() override
	{
		m_world->Render(glm::vec3(), m_camera.GetViewProjection());

		Renderer2D::Start(m_camera.GetViewProjection());

		m_entity->OnRender();

		Renderer2D::Flush();

		Primitives2D::Start(m_camera.GetViewProjection());

		Primitives2D::DrawCircle(m_entity->GetPosition(), 2.0f);

		Primitives2D::DrawRect(m_body->GetPosition() - m_body->GetHalfDimension(), m_body->GetDimension());

		Primitives2D::Flush();

	}

	void OnRenderDebug() override
	{
		// debug info
		fontShader.Use();
		fontShader.SetUniformMat4("u_Projection", ignisScreenMat());
		fontShader.SetUniform4f("u_Color", WHITE);

		FontRenderer::RenderText(font, obelisk::format("FPS: %d", m_timer.FPS), 0.0f, 32.0f);
	}

	void OnImGui() override
	{

	}
}; 

#define RUN_GAME 1

int main()
{
#if RUN_GAME
	Frost* game = new Frost();

	game->Run();

	delete game;
#else

	system("Pause");
#endif

	return 0;
}