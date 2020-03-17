#include "SceneManager.hpp"

#include "json/TemplateLoader.hpp"

#include "Application.hpp"

SceneManager::SceneManager(std::shared_ptr<Camera> camera, float gridsize, uint16_t padding)
	: m_camera(camera)
{
	m_editmode = true;
	m_showgrid = false;

	m_gridsize = gridsize;
	m_padding = gridsize * padding;

	m_active = nullptr;
}

SceneManager::~SceneManager()
{
	if (m_scene)
		m_scene->OnExtit();
}

void SceneManager::RegisterScenes(const std::string& path)
{
	TemplateLoader::LoadSceneRegister(this, path);
}

void SceneManager::RegisterScene(const std::string& name, const std::string& path)
{
	m_register.insert({ name, path });
}

void SceneManager::ChangeScene(const std::string& name)
{
	if (!obelisk::StringCompare(m_sceneName, name))
	{
		auto newScene = LoadScene(name);

		if (newScene)
		{
			// Exit old Scene
			if (m_scene)
				m_scene->OnExtit();

			m_scene = newScene;
			m_sceneName = name;

			// Enter new scene
			m_scene->OnEntry();
		}
		else
		{
			OBELISK_WARN("Failed to load scene: %s", name.c_str());
		}
	}
}

std::shared_ptr<Scene> SceneManager::LoadScene(const std::string& name)
{
	auto path = m_register.find(name);

	if (path == m_register.end())
	{
		OBELISK_WARN("Couldn't find scene: ", name.c_str());
		return nullptr;
	}

	return TemplateLoader::LoadScene(path->second, m_camera);
}

void SceneManager::OnEvent(const Event& e)
{
	if (e.GetType() == EventType::WindowResize)
	{
		WindowResizeEvent& resize = (WindowResizeEvent&)e;
		((OrthographicCamera*)m_camera.get())->SetProjection((float)resize.GetWidth(), (float)resize.GetHeight());
	}

	if (e.GetType() == EventType::KeyPressed)
	{
		switch (((KeyPressedEvent&)e).GetKeyCode())
		{
		case KEY_F1:
			m_editmode = !m_editmode;
			break;
		case KEY_DELETE:
			m_scene->RemoveEntity("tree");
			break;
		}
	}

	m_scene->OnEvent(e);
}

void SceneManager::OnUpdate(float deltaTime)
{
	if (!m_editmode)
	{
		m_scene->OnUpdate(deltaTime);
	}
	else
	{
		float cameraspeed = 400.0f;
		glm::vec3 position = m_camera->GetPosition();

		if (Input::KeyPressed(KEY_A))
			position.x -= cameraspeed * deltaTime;
		if (Input::KeyPressed(KEY_D))
			position.x += cameraspeed * deltaTime;
		if (Input::KeyPressed(KEY_S))
			position.y -= cameraspeed * deltaTime;
		if (Input::KeyPressed(KEY_W))
			position.y += cameraspeed * deltaTime;

		m_camera->SetPosition(position);

		m_active = m_scene->GetEntityAt(GetMousePos());
	}
}

void SceneManager::OnRender()
{
	m_scene->OnRender();

	if (m_editmode)
	{
		// render grid
		Primitives2DStart(m_camera->GetViewProjectionPtr());

		if (m_showgrid)
		{
			ignis_color_rgba color = IGNIS_WHITE;
			ignisBlendColorRGBA(&color, 0.5f);

			for (float x = -m_padding; x <= m_scene->GetWidth() + m_padding; x += m_gridsize)
			{
				Primitives2DRenderLine(x, -m_padding, x, m_scene->GetHeight() + m_padding, color);
			}

			for (float y = -m_padding; y <= m_scene->GetHeight() + m_padding; y += m_gridsize)
			{
				Primitives2DRenderLine(-m_padding, y, m_scene->GetWidth() + m_padding, y, color);
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

				Primitives2DRenderRect(min.x, min.y, max.x - min.x, max.y - min.y, IGNIS_WHITE);
				Primitives2DRenderCircle(position.x, position.y, 2.0f, IGNIS_WHITE);
			}
		}

		Primitives2DFlush();
	}
}

void SceneManager::OnRenderDebug()
{
	if (!m_editmode)
		m_scene->OnRenderDebug();
}

void SceneManager::OnImGui()
{
	if (m_editmode)
	{
		ImGui::Begin("Editor");

		ImGui::Text("Selected Entity: %s", m_active == nullptr ? "null" : m_active->GetName().c_str());

		ImGui::Checkbox("Show grid", &m_showgrid);

		ImGui::End();
	}
}

glm::vec2 SceneManager::GetMousePos() const
{
	glm::vec2 viewSize = m_camera->GetSize();
	glm::vec2 viewPos = m_camera->GetPosition();

	glm::vec2 mousePos = Input::MousePosition();
	mousePos.y = m_camera->GetSize().y - mousePos.y;

	mousePos += viewPos - (m_camera->GetSize() / 2.0f);

	return mousePos;
}