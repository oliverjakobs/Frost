workspace "Frost"
	architecture "x64"
	startproject "Frost"

	configurations
	{
		"Debug",
		"OptimizedDebug",
		"Release"
	}

output_dir = "%{cfg.buildcfg}"

group "Packages"

include "packages/Ignis"
include "packages/Minimal"

group ""

project "Frost"
	kind "ConsoleApp"
	language "C"
	cdialect "C99"
	staticruntime "On"
	
	targetdir ("build/bin/" .. output_dir .. "/%{prj.name}")
	objdir ("build/bin-int/" .. output_dir .. "/%{prj.name}")

	files
	{
		--Config
		"config.ini",
		--Source
		"src/**.h",
		"src/**.c",
		--Resources
		"res/**.ini",
		"res/fonts/**.ttf",
		"res/shaders/**.vert",
		"res/shaders/**.frag",
		"res/textures/**.png"
	}

	links
	{
		"Minimal",
		"Ignis",
		"opengl32"
	}

	includedirs
	{
		"src",
		"packages/Minimal/src",
		"packages/Ignis/src"
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