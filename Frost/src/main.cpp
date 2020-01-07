#include "Application.h"

#include "Entity/Scene.h"

#include "json/TemplateLoader.h"

using namespace ignis;

glm::vec2 GetMousePos(Scene* scene)
{
	glm::vec2 viewSize = scene->GetCamera()->GetSize();
	glm::vec2 viewPos = scene->GetCamera()->GetPosition();

	glm::vec2 mousePos = Input::MousePosition();
	mousePos.y = viewSize.y - mousePos.y;

	mousePos += viewPos - (viewSize / 2.0f);

	return mousePos;
}

class Frost : public Application
{
private:
	std::shared_ptr<Scene> m_scene;

	bool editmode;
	bool showgrid;

	float gridsize;
	float padding;

	std::shared_ptr<Entity> m_active;

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
		Primitives2D::Init(std::make_shared<Shader>("res/shaders/lines.vert", "res/shaders/lines.frag"));

		FontRenderer::Init(std::make_shared<Shader>("res/shaders/font.vert", "res/shaders/font.frag"));
		FontRenderer::AddFont("font", std::make_shared<Font>("res/fonts/OpenSans.ttf", 32.0f));

		EnableDebugMode(true);
		EnableImGui(true);
		EnableVsync(false);

		auto camera = std::make_shared<OrthographicCamera>(glm::vec3(m_width / 2.0f, m_height / 2.0f, 0.0f), glm::vec2(m_width, m_height));
		m_scene = TemplateLoader::LoadScene("res/templates/scenes/scene.json", camera);

		// player
		m_scene->AddEntity(TemplateLoader::LoadEntity("res/templates/entities/player.json"), glm::vec2(120.0f, 400.0f));

		editmode = true;
		showgrid = false;

		gridsize = 32.0f;
		padding = gridsize * 4;

		m_active = nullptr;
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
			((OrthographicCamera*)m_scene->GetCamera())->SetProjection(glm::vec2((float)resize.GetWidth(), (float)resize.GetHeight()));
		}

		if (e.GetType() == EventType::KeyPressed)
		{
			switch (((KeyPressedEvent&)e).GetKeyCode())
			{
			case KEY_ESCAPE:
				Close();
				break;
			case KEY_F5:
				Pause();
				break;
			case KEY_F6:
				ToggleVsync();
				break;
			case KEY_F7:
				ToggleDebugMode();
				break;
			case KEY_F8:
				ToggleImGui();
				break;
			case KEY_F1:
				editmode = !editmode;
				break;
			case KEY_DELETE:
				m_scene->RemoveEntity("tree");
				break;
			}
		}
	}

	void OnUpdate(float deltaTime) override
	{
		// discard frames that took to long
		if (deltaTime > 0.4f) return;

		if (!editmode)
		{
			m_scene->OnUpdate(deltaTime);
		}
		else
		{
			float cameraspeed = 400.0f;
			glm::vec3 position = m_scene->GetCamera()->GetPosition();

			if (Input::KeyPressed(KEY_A))
				position.x -= cameraspeed * deltaTime;
			if (Input::KeyPressed(KEY_D))
				position.x += cameraspeed * deltaTime;

			if (Input::KeyPressed(KEY_S))
				position.y -= cameraspeed * deltaTime;
			if (Input::KeyPressed(KEY_W))
				position.y += cameraspeed * deltaTime;

			m_scene->GetCamera()->SetPosition(position);

			m_active = m_scene->GetEntityAt(GetMousePos(m_scene.get()));
		}
	}

	void OnRender() override
	{
		m_scene->OnRender();

		if (editmode)
		{
			// render grid
			Primitives2D::Start(m_scene->GetCamera()->GetViewProjection());
			
			if (showgrid)
			{
				for (float x = -padding; x <= m_scene->GetWidth() + padding; x += gridsize)
				{
					Primitives2D::DrawLine(x, -padding, x, m_scene->GetHeight() + padding);
				}

				for (float y = -padding; y <= m_scene->GetHeight() + padding; y += gridsize)
				{
					Primitives2D::DrawLine(-padding, y, m_scene->GetWidth() + padding, y);
				}
			}

			if (m_active)
			{
				auto tex = m_active->GetComponent<TextureComponent>();

				if (tex != nullptr)
				{
					glm::vec2 position = m_active->GetPosition();

					glm::vec2 min = position - glm::vec2(tex->GetWidth() / 2.0f, 0.0f);
					glm::vec2 max = min + tex->GetDimension();

					Primitives2D::DrawRect(min, max - min);
					Primitives2D::DrawCircle(m_active->GetPosition(), 2.0f);
				}
			}

			Primitives2D::Flush();
		}
	}

	void OnRenderDebug() override
	{
		if (!editmode)
			m_scene->OnRenderDebug();

		// debug info
		FontRenderer::RenderText("font", obelisk::format("FPS: %d", m_timer.FPS), 0.0f, 32.0f, ignisScreenMat(), WHITE);
	}

	void OnImGui() override
	{
		ImGui::Begin("Settings");

		ImGui::Text("F5: Pause/Unpause");
		ImGui::Text("F6: Toggle Vsync");
		ImGui::Text("F7: Toggle debug mode");
		ImGui::Text("F8: Toggle ImGui");

		ImGui::Separator();

		ImGui::Text("F1: Toggle edit mode");
		ImGui::Text("DEL: Remove Trees");

		ImGui::End();

		// ----
		ImGui::Begin("DEBUG");
		
		auto player = m_scene->GetEntity("player");
		 
		ImGui::Text("Name: %s", player->GetName().c_str());
		auto position = player->GetPosition();
		ImGui::Text("Position: %4.2f, %4.2f", position.x, position.y);

		ImGui::End();

		if (editmode)
		{
			ImGui::Begin("Editor");

			ImGui::Text("Selected Entity: %s", m_active == nullptr ? "null" : m_active->GetName().c_str());

			ImGui::Checkbox("Show grid", &showgrid);

			ImGui::End();
		}
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