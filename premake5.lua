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
include "Frost/packages/lua"

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
		--Config
		"%{prj.name}/config.json",
		--Resources
		"%{prj.name}/res/resources.json",
		"%{prj.name}/res/shader/*.vert",
		"%{prj.name}/res/shader/*.frag",
		"%{prj.name}/res/images/**.png",
		"%{prj.name}/res/maps/**.json",
		"%{prj.name}/res/scripts/**.json",
		"%{prj.name}/res/scripts/**.lua"
	}

	links
	{
		"GLFW",
		"Glad",
		"ImGui",
		"Lua",
		"opengl32"
	}

	includedirs
	{
		"%{prj.name}/src",
		"%{prj.name}/packages/glm",
		"%{prj.name}/packages/stb",
		"%{prj.name}/packages/imgui",
		"%{prj.name}/packages/json/single_include",
		"%{prj.name}/packages/spdlog/include",
		"%{prj.name}/packages/glfw/include",
		"%{prj.name}/packages/glad/include",
		"%{prj.name}/packages/lua/src",
		"%{prj.name}/packages/sol2/single/include"
	}

	filter "system:windows"
		systemversion "latest"

	filter "configurations:Debug"
		runtime "Debug"
		symbols "On"

	filter "configurations:Release"
		runtime "Release"
		optimize "On"