#include "Application.h"

#include "Tile/World.h"

#include "Entity/Scene.h"

using namespace ignis;
using namespace tile;

class Frost : public Application
{
private:
	ignis::RenderState m_renderState;
	ignis::OrthographicCamera m_camera;

	std::shared_ptr<Scene> m_scene;

	Font font = Font("res/fonts/OpenSans.ttf", 32.0f);
	Shader fontShader = Shader("res/shaders/font.vert", "res/shaders/font.frag");
public:
	Frost() 
		: Application("Frost", 1024, 800)
	{
		// ---------------| Config |------------------------------------------
		m_renderState.SetBlend(true, GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glClearColor(0.2f, 0.2f, 0.2f, 1.0f);

		m_camera = ignis::OrthographicCamera(0.0f, (float)m_width, 0.0f, (float)m_height);

		Renderer2D::Init(std::make_shared<Shader>("res/shaders/renderer2D.vert", "res/shaders/renderer2D.frag"));
		FontRenderer::Init(std::make_shared<Shader>("res/shaders/font.vert", "res/shaders/font.frag"));
		Primitives2D::Init(std::make_shared<Shader>("res/shaders/lines.vert", "res/shaders/lines.frag"));

		EnableDebugMode(true);
		EnableImGui(true);
		EnableVsync(false);

		m_scene = std::make_shared<Scene>();

		auto entity = std::make_shared<Entity>("player", glm::vec2(400.0f, 200.0f), glm::vec2(20.0f, 20.0f));

		entity->AddComponent<PhysicsComponent>(m_scene->GetWorld()->CreateBody(400.0f, 200.0f, 20.0f, 30.0f, BodyType::BODY_DYNAMIC), glm::vec2(0.0f, 30.0f));
		entity->AddComponent<TextureComponent>(std::make_shared<Texture>("res/textures/player.png", 4, 6), 40.0f, 60.0f);

		std::map<std::string, Animation> animations
		{
			{ "idle", Animation(0, 4, 0.2) },
			{ "walk", Animation(6, 6, 0.125) },
			{ "jump", Animation(12, 3, 0.3) },
			{ "fall", Animation(18, 2, 0.4) }
		};

		entity->AddComponent<AnimationComponent>(animations);
		entity->AddComponent<PlayerComponent>(400.0f, 800.0f);

		m_scene->AddEntity(entity);
	}

	~Frost()
	{
		Renderer2D::Destroy();
		FontRenderer::Destroy();
		Primitives2D::Destroy();
	}
	
	void OnEvent(Event& e) override
	{
		if (e.GetType() == EventType::WindowResize)
		{
			WindowResizeEvent& resize = (WindowResizeEvent&)e;
			m_camera.SetProjection(0.0f, (float)resize.GetWidth(), 0.0f, (float)resize.GetHeight());
		}

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
			case KEY_DELETE:
				m_scene->Clear();
				break;
			}
		}
	}

	void OnUpdate(float deltaTime) override
	{
		m_scene->OnUpdate(deltaTime);
	}

	void OnRender() override
	{
		m_scene->GetWorld()->Render(glm::vec3(), m_camera.GetViewProjection());

		Renderer2D::Start(m_camera.GetViewProjection());

		m_scene->OnRender();

		Renderer2D::Flush();
	}

	void OnRenderDebug() override
	{
		// debug info
		FontRenderer::RenderText(font, obelisk::format("FPS: %d", m_timer.FPS), 0.0f, 32.0f, ignisScreenMat(), WHITE);

		Primitives2D::Start(m_camera.GetViewProjection());

		m_scene->OnRenderDebug();

		Primitives2D::Flush();
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