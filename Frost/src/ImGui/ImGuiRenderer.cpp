#include "ImGuiRenderer.h"

#include "examples/imgui_impl_glfw.h"
#include "examples/imgui_impl_opengl3.h"

#include "GLFW/glfw3.h"

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
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		style.WindowRounding = 0.0f;
		style.Colors[ImGuiCol_WindowBg].w = 1.0f;
	}
	
	// Setup Platform/Renderer bindings
	ImGui_ImplGlfw_InitForOpenGL(context, true);
	ImGui_ImplOpenGL3_Init("#version 410");
}

void ImGuiRenderer::Quit()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}

void ImGuiRenderer::Start()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
}

void ImGuiRenderer::Flush(GLFWwindow* context)
{
	ImGuiIO& io = ImGui::GetIO();
	int displayW, displayH;
	glfwGetFramebufferSize(context, &displayW, &displayH);
	io.DisplaySize = ImVec2(displayW, displayH);

	// Rendering
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		GLFWwindow* backup_current_context = glfwGetCurrentContext();
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
		glfwMakeContextCurrent(backup_current_context);
	}
}
