#pragma once

#include "Graphics.h"

#include "Debugger.h"

#include "Utility/Utils.h"
#include "Utility/Timer.h"

#include "Input/Input.h"

#include "Maths/Maths.h"

#include "Event/ApplicationEvent.h"
#include "Event/MouseEvent.h"
#include "Event/KeyEvent.h"

#include "ImGui/ImGuiRenderer.h"

struct GLFWwindow;

class Application
{
private:
	GLFWwindow* m_window;

	ImGuiRenderer m_imguiRenderer;

	bool m_running;
	bool m_paused;
	bool m_debug;

	bool OnWindowClose(WindowCloseEvent& e);
	void EventCallback(Event& e);

	bool LoadApplication(const std::string& title, int width, int height, int glMajor, int glMinor);
protected:
	std::string m_title;

	int m_width;
	int m_height;
public:
	Application(const std::string& config);
	Application(const std::string& title, int width, int height);
	virtual ~Application();

	void EnableDebugMode(bool b);
	void ToggleDebugMode();

	void Pause();

	void EnableVsync(bool b);

	// --------------------------| Game Loop |-------------------------------
	void Run();
	void Close();

	virtual void OnEvent(Event& e) = 0;
	virtual void OnUpdate() = 0;
	virtual void OnRender() = 0;
	virtual void OnRenderDebug() {}

	// ----------------------------------------------------------------------
	void SetTitle(const std::string& title);

	int GetWidth() const;
	int GetHeight() const;
	float GetWidthF() const;
	float GetHeightF() const;
	GLFWwindow* GetContext() const;
};