#pragma once

#include "Input/Input.h"

#include "Maths/Maths.h"

#include "ImGuiBinding/ImGuiRenderer.h"

#include "Obelisk/Obelisk.h"
#include "Ignis/Ignis.h"

#include "Event/EventHandler.h"
#include "Scene/SceneManager.h"

class Application
{
private:
	GLFWwindow* m_window;

	bool m_running;
	bool m_paused;
	bool m_debug;
	bool m_showImGui;

	// gets called when an event is processed, handles some (e.g. WindowCloseEvents) 
	// and passes the rest to OnEvent, which is implemented in the game class
	void EventCallback(Event& e);
	bool OnWindowClose(WindowCloseEvent& e);

	bool LoadApplication(const std::string& title, int width, int height, int glMajor, int glMinor);
protected:
	std::string m_title;

	int m_width;
	int m_height;

	obelisk::Timer m_timer;

	SceneManager m_sceneManager;
public:
	Application(const std::string& config);
	Application(const std::string& title, int width, int height);
	virtual ~Application();

	void EnableDebugMode(bool b);
	void ToggleDebugMode();

	void EnableImGui(bool b);
	void ToggleImGui();

	void Pause();

	void EnableVsync(bool b);

	// --------------------------| Game Loop |-------------------------------
	void Run();
	void Close();

	virtual void OnEvent(Event& e) = 0;
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