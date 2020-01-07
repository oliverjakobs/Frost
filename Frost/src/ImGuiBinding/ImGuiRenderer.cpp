#include "ImGuiRenderer.h"

#include "ImGuiGLFW.h"
#include "ImGuiBinding.h"

void ImGuiRenderer::Init(GLFWwindow* context, unsigned int configFlags)
{
	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= configFlags;

	//ImGuiConfigFlags_NavEnableKeyboard:		Enable Keyboard Controls
	//ImGuiConfigFlags_NavEnableGamepad:		Enable Gamepad Controls
	//ImGuiConfigFlags_DockingEnable:			Enable Docking
	//ImGuiConfigFlags_ViewportsEnable:			Enable Multi-Viewport / Platform Windows
	//ImGuiConfigFlags_ViewportsNoTaskBarIcons;
	//ImGuiConfigFlags_ViewportsNoMerge;

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsClassic();

	// When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
	ImGuiStyle& style = ImGui::GetStyle();
	style.WindowRounding = 0.0f;
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		style.Colors[ImGuiCol_WindowBg].w = 1.0f;
	}

	// Setup Platform/Renderer bindings
	ImGuiGLFW_Init(context, true);
	ImGuiBinding_Init("#version 410");
}

void ImGuiRenderer::Quit()
{
	ImGuiBinding_Shutdown();
	ImGuiGLFW_Shutdown();
	ImGui::DestroyContext();
}

void ImGuiRenderer::Begin()
{
	ImGuiBinding_NewFrame();
	ImGuiGLFW_NewFrame();
	ImGui::NewFrame();
}

void ImGuiRenderer::End()
{
	ImGuiIO& io = ImGui::GetIO();

	int width, height;
	glfwGetWindowSize(glfwGetCurrentContext(), &width, &height);
	io.DisplaySize = ImVec2((float)width, (float)height);

	// Rendering
	ImGui::Render();
	ImGuiBinding_RenderDrawData(ImGui::GetDrawData());

	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		GLFWwindow* backup_current_context = glfwGetCurrentContext();
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
		glfwMakeContextCurrent(backup_current_context);
	}
}