project "Lua"
    kind "StaticLib"
    language "C"
    
	targetdir ("bin/" .. outputDir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputDir .. "/%{prj.name}")

	files
	{
        "src/*.c",
        "src/*.h"
    }
    
	filter "system:windows"
        systemversion "latest"
        staticruntime "On"

    filter { "system:windows", "configurations:Release" }
        buildoptions "/MT"