#include "Application.h"

#include "Entity/Scene.h"

#include <string>

#include "json/TemplateLoader.h"

#include "Physics/Collider.h"

using namespace ignis;

class Frost : public Application
{
private:
	std::shared_ptr<OrthographicCamera> m_camera;
	std::shared_ptr<Scene> m_scene;

	std::shared_ptr<Body> body1;
	std::shared_ptr<Body> body2;

	glm::vec2 gravity;
	bool startSim;

	Font font = Font("res/fonts/OpenSans.ttf", 32.0f);
public:
	Frost() 
		: Application("Frost", 1024, 800)
	{
		// ---------------| Config |------------------------------------------
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		//glEnable(GL_DEPTH_TEST);
		glClearColor(0.2f, 0.2f, 0.2f, 1.0f);

		Renderer2D::Init(std::make_shared<Shader>("res/shaders/renderer2D.vert", "res/shaders/renderer2D.frag"));
		FontRenderer::Init(std::make_shared<Shader>("res/shaders/font.vert", "res/shaders/font.frag"));
		Primitives2D::Init(std::make_shared<Shader>("res/shaders/lines.vert", "res/shaders/lines.frag"));

		EnableDebugMode(true);
		EnableImGui(true);
		EnableVsync(false);

		m_camera = std::make_shared<OrthographicCamera>(glm::vec3(m_width / 2.0f, m_height / 2.0f, 0.0f), glm::vec2(m_width, m_height));
		//m_scene = std::make_shared<Scene>(m_camera);

		//// trees
		//srand((unsigned int)time(nullptr));

		//float x = 20.0f;
		//while (x < m_scene->GetWidth())
		//{
		//	int texture = rand() % 5 + 1;

		//	auto tree = std::make_shared<Entity>("tree");
		//	tree->AddComponent<PositionComponent>();
		//	tree->AddComponent<TextureComponent>(std::make_shared<Texture>(obelisk::format("res/textures/tree_%d.png", texture)), 96.0f, 256.0f);

		//	m_scene->AddEntity(tree, glm::vec2(x, 192.0f));

		//	x += rand() % 80 + 140;
		//}

		//// player
		//m_scene->AddEntity(TemplateLoader::LoadEntity("res/templates/entities/player.json"), glm::vec2(400.0f, 200.0f));

		gravity = glm::vec2(0.0f, -980.0f);
		startSim = false;

		body1 = std::make_shared<Body>(400.0f, 400.0f, 20.0f, 30.0f);
		body2 = std::make_shared<Body>(500.0f, 80.0f, 400.0f, 50.0f);
	}

	~Frost()
	{
		Renderer2D::Destroy();
		FontRenderer::Destroy();
		Primitives2D::Destroy();
	}
	
	void OnEvent(const Event& e) override
	{
		if (e.GetType() == EventType::WindowResize)
		{
			WindowResizeEvent& resize = (WindowResizeEvent&)e;
			m_camera->SetProjection(glm::vec2((float)resize.GetWidth(), (float)resize.GetHeight()));
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
			case KEY_W:
				body1->SetPosition(glm::vec2(400.0f, 400.0f));
				break;
			case KEY_SPACE:
				startSim = true;
				break;
			case KEY_DELETE:
				m_scene->RemoveEntity("tree");
				break;
			}
		}

	}

	void OnUpdate(float deltaTime) override
	{
		if (startSim)
		{
			//m_scene->OnUpdate(deltaTime);
			float speed = 400.0f;

			glm::vec2 velocity = glm::vec2(0.0f, body1->GetVelocity().y);
			//glm::vec2 velocity = glm::vec2(0.0f, 0.0f);

			if (Input::KeyPressed(KEY_A))
				velocity.x -= speed;
			if (Input::KeyPressed(KEY_D))
				velocity.x += speed;

			body1->SetVelocity(velocity);

			glm::vec2 oldPosition = body1->GetPosition();
			body1->Tick(deltaTime, gravity);

			body1->ResolveCollision(*body2, oldPosition);
			//collision = Collider::CheckCollision(*body1, *body2);
		}
	}

	void OnRender() override
	{
		//m_scene->OnRender();

		Primitives2D::Start(m_camera->GetViewProjection());

		Primitives2D::DrawRect(body1->GetPosition() - body1->GetHalfDimension(), body1->GetDimension());
		Primitives2D::DrawRect(body2->GetPosition() - body2->GetHalfDimension(), body2->GetDimension());

		Primitives2D::Flush();
	}

	void OnRenderDebug() override
	{
		//m_scene->OnRenderDebug();

		// debug info
		FontRenderer::RenderText(font, obelisk::format("FPS: %d", m_timer.FPS), 0.0f, 32.0f, ignisScreenMat(), WHITE);
	}

	void OnImGui() override
	{
		ImGui::Begin("DEBUG");
		
		// auto player = m_scene->GetEntity("player");
		// 
		// ImGui::Text("Name: %s", player->GetName().c_str());
		auto position = body1->GetPosition();
		auto velocity = body1->GetVelocity();
		ImGui::Text("Position: %4.2f, %4.2f", position.x, position.y);
		ImGui::Text("Velocity: %4.2f, %4.2f", velocity.x, velocity.y);

		//ImGui::Text("Collision: %s", collision ? "True" : "False");
		
		ImGui::End();
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