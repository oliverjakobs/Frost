#pragma once

#include "Input/Input.hpp"

#include "ImGuiBinding/ImGuiRenderer.hpp"

#include "Obelisk/Obelisk.hpp"

#include "Event/EventHandler.hpp"

// Renderer
#include "IgnisRenderer/FontRenderer.h"
#include "IgnisRenderer/Primitives2D.h"
#include "IgnisRenderer/BatchRenderer2D.h"
#include "IgnisRenderer/Renderer2D.h"


void SetViewport(int x, int y, int w, int h);

const glm::mat4& GetScreenMat();
const float* GetScreenMatPtr();

class Application
{
private:
	GLFWwindow* m_window;

	bool m_running;
	bool m_paused;

	bool m_debug;
	bool m_showImGui;
	bool m_vsync;

	// gets called when an event is processed, handles some (e.g. WindowCloseEvents) 
	// and passes the rest to OnEvent, which is implemented in the game class
	void EventCallback(Event& e);
	bool OnWindowClose(WindowCloseEvent& e);

	void SetGLFWCallback();
	bool LoadApplication(const std::string& title, int width, int height, int glMajor, int glMinor);
protected:
	std::string m_title;

	int m_width;
	int m_height;

	obelisk::Timer m_timer;
public:
	Application(const std::string& title, int width, int height);
	virtual ~Application();

	// --------------------------| Settings |--------------------------------
	void EnableDebugMode(bool b);
	void EnableImGui(bool b);
	void EnableVsync(bool b);

	void ToggleDebugMode();
	void ToggleImGui();
	void ToggleVsync();

	void Pause();

	// --------------------------| Game Loop |-------------------------------
	void Run();
	void Close();

	virtual void OnEvent(const Event& e) = 0;
	virtual void OnUpdate(float deltaTime) = 0;
	virtual void OnRender() = 0;
	virtual void OnRenderDebug() {}
	virtual void OnImGui() {}

	// ----------------------------------------------------------------------
	void SetTitle(const std::string& title);

	int GetWidth() const;
	int GetHeight() const;
	float GetWidthF() const;
	float GetHeightF() const;
	GLFWwindow* GetContext() const;
};