workspace "Frost"
	startproject "Frost"

	configurations
	{
		"Debug",
		"Release"
	}

outputDir = "%{cfg.buildcfg}"   

group "Packages"

include "Frost/packages/glfw"
include "Frost/packages/glad"
include "Frost/packages/imgui"

group ""

project "Frost"
	location "Frost"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "On"
	
	targetdir ("bin/" .. outputDir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputDir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/res/shader/*.vert",
		"%{prj.name}/res/shader/*.frag",
		"%{prj.name}/res/**.tm"
	}

	links
	{
		"GLFW",
		"Glad",
		"ImGui",
		"opengl32"
	}

	includedirs
	{
		"%{prj.name}/src",
		"%{prj.name}/packages/include",
		"%{prj.name}/packages/imgui",
		"%{prj.name}/packages/spdlog/include",
		"%{prj.name}/packages/glfw/include",
		"%{prj.name}/packages/glad/include"
	}

	filter "system:windows"
		systemversion "latest"

	filter "configurations:Debug"
		runtime "Debug"
		symbols "On"

	filter "configurations:Release"
		runtime "Release"
		optimize "On"