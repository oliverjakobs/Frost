#pragma once

#include "Graphics.h"

#include "Utility/Utils.h"
#include "Utility/Timer.h"
#include "Utility/Debugger.h"

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
	bool m_debug;

	bool OnWindowClose(WindowCloseEvent& e);
protected:
	std::string m_title;

	int m_width;
	int m_height;
public:
	Application(const std::string& title, int width, int height);
	virtual ~Application();

	void EnableDebugMode(bool b);
	void ToggleDebugMode();

	void EnableVsync(bool b);

	void EventCallback(Event& e);

	// --------------------------| loop control |----------------------------
	void Run();
	void Close();

	virtual void OnEvent(Event& e) = 0;
	virtual void OnUpdate() = 0;
	virtual void OnRender() = 0;
	virtual void OnRenderDebug() const {}

	// ----------------------------------------------------------------------
	void SetTitle(const std::string& title);

	int GetWidth() const;
	int GetHeight() const;
	float GetWidthF() const;
	float GetHeightF() const;
	GLFWwindow* GetContext() const;
};