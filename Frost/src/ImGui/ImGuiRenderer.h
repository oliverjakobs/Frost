#pragma once

#include "imgui.h"

struct GLFWwindow;

struct ImGuiRenderer
{
	static void Init(GLFWwindow* context, unsigned int configFlags);
	static void Quit();

	static void Start();
	static void Flush(GLFWwindow* context);
};