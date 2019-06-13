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

struct GLFWwindow;

class Application
{
private:
	GLFWwindow* m_window;

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

	void enableDebugMode(bool b);
	void toggleDebugMode();

	void enableVsync(bool b);

	void EventCallback(Event& e);

	// --------------------------| loop control |----------------------------
	void run();
	void close();

	virtual void onEvent(Event& e) = 0;
	virtual void onUpdate() = 0;
	virtual void onRender() = 0;
	virtual void onRenderDebug() const {}

	// ----------------------------------------------------------------------
	void setTitle(const std::string& title);

	int getWidth() const;
	int getHeight() const;
	float getWidthF() const;
	float getHeightF() const;
	GLFWwindow* getContext() const;
};