#pragma once

#include "imgui.h"

struct GLFWwindow;

class ImGuiRenderer
{
private:
	float m_Time = 0.0f;
public:
	void Init(GLFWwindow* context, unsigned int configFlags);
	void Quit();

	void Start();
	void Flush(GLFWwindow* context);
};