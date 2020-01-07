workspace "Frost"
	architecture "x64"
	startproject "Frost"

	configurations
	{
		"Debug",
		"Release"
	}

outputDir = "%{cfg.buildcfg}"   

group "Packages"

include "Frost/packages/glfw"
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
		"%{prj.name}/src/**.c",
		"%{prj.name}/src/**.cpp",
		--Config
		"%{prj.name}/config.json",
		--Resources
		"%{prj.name}/res/resources.json",
		"%{prj.name}/res/shaders/**.vert",
		"%{prj.name}/res/shaders/**.frag",
		"%{prj.name}/res/images/**.png",
		"%{prj.name}/res/maps/**.json",
		"%{prj.name}/res/scripts/**.json"
	}

	links
	{
		"GLFW",
		"ImGui",
		"opengl32"
	}

	includedirs
	{
		"%{prj.name}/src",
		"%{prj.name}/packages/glm",
		"%{prj.name}/packages/imgui",
		"%{prj.name}/packages/glfw/include"
	}

	filter "system:windows"
		systemversion "latest"

	filter "configurations:Debug"
		runtime "Debug"
		symbols "On"

	filter "configurations:Release"
		runtime "Release"
		optimize "On"