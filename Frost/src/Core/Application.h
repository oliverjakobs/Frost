#pragma once

#include "Graphics.h"

#include "Utility/Utils.h"
#include "Utility/Timer.h"
#include "Utility/Debugger.h"

#include "Input/Input.h"

#include "Maths/Maths.h"

struct GLFWwindow;

class Application
{
private:
	GLFWwindow* m_window;

	bool m_debug;
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

	// --------------------------| loop control |----------------------------
	void run();
	void close();

	// --------------------------| loop funtions |---------------------------
	virtual void onInput() = 0;
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