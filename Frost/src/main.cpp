#include "Core/Application.h"

#include "Scene/SceneManager.h"
#include "Scene/EntityMananger.h"

class Frost : public Application
{
private:

public:
	Frost() : Application("Frost", 1024, 800)
	{
		// ---------------| Config|------------------------------------------
		Renderer::EnableBlend(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		Renderer::SetClearColor(0.2f, 0.2f, 0.2f, 1.0f);

		EnableDebugMode(true);
		EnableVsync(false);

		// ---------------| Load resources|----------------------------------
		ResourceManager::Load("res/resources.json");

		Scene* scene = new Scene("station", new TileMap(ResourceManager::GetImage("tileset"), "res/maps/station1.txt"));

		scene->AddEntity("player", "res/scripts/player.json");
		scene->GetRegistry().assign<ScriptComponent>(scene->GetEntity("player"), "res/scripts/player.lua", "on_update");

		SceneManager::AddScene(scene);
	}

	~Frost()
	{

	}
	
	void OnEvent(Event& e) override
	{
		if (e.GetEventType() == EventType::KeyPressed)
		{
			KeyPressedEvent& keyPressed = (KeyPressedEvent&)e;

			switch (keyPressed.GetKeyCode())
			{
			case KEY_ESCAPE:
				Close();
				break;
			case KEY_F7:
				ToggleDebugMode();
				break;
			}
		}
	}

	void OnUpdate() override
	{
		SceneManager::OnUpdate();
	}

	void OnRender() override
	{
		SceneManager::OnRender();
	}

	void OnRenderDebug() override
	{
		SceneManager::OnRenderDebug();

		// Debug Info
		ImGui::Begin("Info");
		ImGui::Text("FPS: %d", Timer::GetFPS());

		static bool select = false;
		ImGui::Checkbox("Vsync", &select);
		EnableVsync(select);
		ImGui::End();

		//Entity
		ImGui::Begin("Player");

		unsigned int entity = SceneManager::GetActiveScene()->GetEntity("player");
		auto transform = SceneManager::GetActiveScene()->GetRegistry().get<TransformComponent>(entity);
		auto animation = SceneManager::GetActiveScene()->GetRegistry().get<AnimationComponent>(entity);

		ImGui::Text("Position: %4.2f, %4.2f", transform.position.x, transform.position.y);
		ImGui::Text("Current Animation: %s", animation.currentAnimation.c_str());

		ImGui::End();
	}
};


int main()
{
	Logger::SetFormat("[%T] [%^%l%$]: %v");
	Logger::SetLevel(LogLevel::Trace);

#if 1
	Frost* game = new Frost();

	game->Run();

	delete game;
#else

	sol::state lua;

	lua.open_libraries(sol::lib::base);

	lua.new_usertype<glm::vec2>("Vec2",
		sol::constructors<glm::vec2(), glm::vec2(float, float)>(),
		"x", &glm::vec2::x,
		"y", &glm::vec2::y
		);

	// Input
	lua.new_usertype<LuaInput>("Input",
		sol::constructors<LuaInput()>(),
		"KeyPressed", &LuaInput::KeyPressed,
		"KeyReleased", &LuaInput::KeyReleased
		);

	// Components
	lua.new_usertype<LuaTransformComponent>("Transform",
		sol::constructors<LuaTransformComponent(TransformComponent*)>(),
		"position", sol::property(&LuaTransformComponent::GetPosition, &LuaTransformComponent::SetPosition),
		"dimension", sol::property(&LuaTransformComponent::GetDimension, &LuaTransformComponent::SetDimension)
		);

	lua.script_file("res/scripts/player.lua");

	sol::function update = lua["on_update"];	

	update(0);

	system("Pause");
#endif

	return 0;
}