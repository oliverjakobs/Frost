#include "Application.h"

#include "SceneManager.h"

#include "json/TemplateLoader.h"

#include "Animation/Animator.h"
#include <glm/gtc/matrix_transform.hpp>

using namespace ignis;

class Frost : public Application
{
private:
	std::shared_ptr<SceneManager> m_sceneManager;

	Animator animator;
	std::shared_ptr<Texture> texture;
	std::shared_ptr<OrthographicCamera> camera;

	float tex_x;
	float tex_y;
	float tex_width;
	float tex_height;
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

		// auto camera = std::make_shared<OrthographicCamera>(glm::vec3(m_width / 2.0f, m_height / 2.0f, 0.0f), glm::vec2(m_width, m_height));
		// m_sceneManager = std::make_shared<SceneManager>(camera, 32.0f, 4);
		// m_sceneManager->RegisterScenes("res/templates/scenes/register.json");
		// m_sceneManager->ChangeScene("scene");

		animator.AddCondition("cond_1", [](Entity* e, int s) { return Input::KeyPressed(KEY_1); });
		animator.AddCondition("cond_2", [](Entity* e, int s) { return Input::KeyPressed(KEY_2); });

		Animation idle = Animation(0, 4, 0.2f);
		idle.AddContition("cond_1", "walk");
		animator.AddAnimation("idle", idle);

		Animation walk = Animation(6, 6, 0.125f);
		walk.AddContition("cond_2", "idle");
		animator.AddAnimation("walk", walk);

		camera = std::make_shared<OrthographicCamera>(glm::vec3(m_width / 2.0f, m_height / 2.0f, 0.0f), glm::vec2(m_width, m_height));

		texture = std::make_shared<Texture>("res/textures/player.png", 4, 6);
		tex_width = 80.0f;
		tex_height = 120.0f;
		tex_x = 512.0f - tex_width / 2.0f;
		tex_y = 400.0f - tex_height / 2.0f;
	}

	~Frost()
	{
		Renderer2D::Destroy();
		FontRenderer::Destroy();
		Primitives2D::Destroy();
	}
	
	void OnEvent(const Event& e) override
	{
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
			}
		}

		// m_sceneManager->OnEvent(e);
	}

	void OnUpdate(float deltaTime) override
	{
		// discard frames that took to long
		// if (deltaTime > 0.4f) return;

		// m_sceneManager->OnUpdate(deltaTime);

		animator.Tick(deltaTime);
	}

	void OnRender() override
	{
		// m_sceneManager->OnRender();

		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(tex_x, tex_y, 0.0f));
		model = glm::scale(model, glm::vec3(tex_width, tex_height, 1.0f));

		float src_w = 1.0f / texture->GetColumns();
		float src_h = 1.0f / texture->GetRows();

		int frame = animator.GetFrame();

		float src_x = (frame % texture->GetColumns()) * src_w;
		float src_y = 1 - src_h - ((frame / texture->GetColumns()) * src_h);

		glm::mat4 src = glm::mat4(1.0f);
		src = glm::translate(src, glm::vec3(src_x, src_y, 0.0f));
		src = glm::scale(src, glm::vec3(src_w, src_h, 1.0f));

		Renderer2D::Start(camera->GetViewProjection());

		Renderer2D::RenderTexture(texture, model, src);

		Renderer2D::Flush();
	}

	void OnRenderDebug() override
	{
		// m_sceneManager->OnRenderDebug();

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
		
		// auto player = m_sceneManager->GetScene()->GetEntity("player");
		//  
		// ImGui::Text("Name: %s", player->GetName().c_str());
		// auto position = player->GetPosition();
		// ImGui::Text("Position: %4.2f, %4.2f", position.x, position.y);
		// ImGui::Text("Precise Y: %f", position.y);
		// 
		// ImGui::Separator();
		// 
		// ImGui::Text("Scene: %s", m_sceneManager->GetSceneName().c_str());

		ImGui::End();

		// m_sceneManager->OnImGui();
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