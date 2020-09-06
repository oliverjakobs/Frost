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

group ""

project "Frost"
	location "Frost"
	kind "ConsoleApp"
	language "C"
	cdialect "C99"
	staticruntime "On"
	
	targetdir ("bin/" .. outputDir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputDir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.c",
		"%{prj.name}/src/**.hpp",
		"%{prj.name}/src/**.cpp",
		--Config
		"%{prj.name}/config.json",
		--Resources
		"%{prj.name}/res/fonts/**.ttf",
		"%{prj.name}/res/shaders/**.vert",
		"%{prj.name}/res/shaders/**.frag",
		"%{prj.name}/res/templates/**.json",
		"%{prj.name}/res/textures/**.png"
	}

	links
	{
		"GLFW",
		"opengl32"
	}

	includedirs
	{
		"%{prj.name}/src",
		"%{prj.name}/packages/glfw/include"
	}

	filter "system:windows"
		systemversion "latest"
		defines { "_CRT_SECURE_NO_WARNINGS" }

	filter "configurations:Debug"
		runtime "Debug"
		symbols "On"

	filter "configurations:Release"
		runtime "Release"
		optimize "On"