workspace "Frost"

	configurations
	{
		"Debug",
		"Release"
	}

outputDir = "%{cfg.buildcfg}"   

IncludeDir = {}
IncludeDir["INCLUDE"] = "Frost/external/include" 
IncludeDir["GLFW"] = "Frost/external/glfw/include" 
IncludeDir["GLAD"] = "Frost/external/glad/include"

include "Frost/external/glfw"
include "Frost/external/glad"

project "Frost"
	location "Frost"
	kind "ConsoleApp"
	language "C++"
	
	targetdir ("bin/" .. outputDir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputDir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/res/shader/*.vert",
		"%{prj.name}/res/shader/*.frag"
	}

	links
	{
		"GLFW",
		"GLAD",
		"opengl32"
	}

	includedirs
	{
		"%{prj.name}/src",
		"Frost/external/include",
		"Frost/external/glfw/include",
		"Frost/external/glad/include"
	}

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"
