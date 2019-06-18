workspace "Frost"
	startproject "Frost"

	configurations
	{
		"Debug",
		"Release"
	}

outputDir = "%{cfg.buildcfg}"   

group "Dependencies"

include "Frost/external/glfw"
include "Frost/external/glad"
include "Frost/external/imgui"

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
		"%{prj.name}/external/include",
		"%{prj.name}/external/imgui",
		"%{prj.name}/external/spdlog/include",
		"%{prj.name}/external/glfw/include",
		"%{prj.name}/external/glad/include"
	}

	filter "system:windows"
		systemversion "latest"

	filter "configurations:Debug"
		runtime "Debug"
		symbols "On"

	filter "configurations:Release"
		runtime "Release"
		optimize "On"