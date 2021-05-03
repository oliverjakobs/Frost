workspace "Frost"
	architecture "x64"
	startproject "Frost"

	configurations
	{
		"Debug",
		"OptimizedDebug",
		"Release"
	}

outputDir = "%{cfg.buildcfg}"

group "Packages"

include "Frost/packages/Ignis"
include "Frost/packages/Minimal"

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
		--Config
		"%{prj.name}/config.ini",
		--Source
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.c",
		--Resources
		"%{prj.name}/res/**.json",
		"%{prj.name}/res/fonts/**.ttf",
		"%{prj.name}/res/shaders/**.vert",
		"%{prj.name}/res/shaders/**.frag",
		"%{prj.name}/res/textures/**.png"
	}

	links
	{
		"Minimal",
		"Ignis",
		"opengl32"
	}

	includedirs
	{
		"%{prj.name}/src",
		"%{prj.name}/packages/Minimal/src",
		"%{prj.name}/packages/Ignis/src"
	}

	filter "system:windows"
		systemversion "latest"
		defines { "_CRT_SECURE_NO_WARNINGS" }

	filter "configurations:Debug"
		runtime "Debug"
		symbols "On"
		
	filter "configurations:OptimizedDebug"
		runtime "Debug"
		symbols "On"
		optimize "On"

	filter "configurations:Release"
		runtime "Release"
		optimize "On"