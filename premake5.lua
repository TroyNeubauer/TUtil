workspace "TUtil"
	architecture "x64"
	startproject "TUtilTest"

	configurations
	{
		"DebugLib",
		"DebugDLL",
		"ReleaseLib",
		"ReleaseDLL",
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- Include directories relative to root folder (solution directory)
IncludeDir = {}
IncludeDir["str"] = "TUtil/vendor/ocornut_str"
IncludeDir["libarchive"] = "TUtil/vendor/libarchive/libarchive"


project "TUtil"
	location "TUtil"

	filter "*Lib"
		kind "StaticLib"
	filter "*DLL"
		kind "SharedLib"

	language "C++"
	cppdialect "C++17"
	staticruntime "on" 
	intrinsics "on"
	systemversion "latest"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/include/**.h",
	}

	includedirs
	{
		"%{prj.name}/include/",
	}

	links 
	{ 
	}

	defines
	{
	}


	filter "configurations:Debug"
		defines "T_DEBUG"
		runtime "Debug"
		symbols "on"
		floatingpoint "Strict"


	filter "configurations:Release"
		defines "T_RELEASE"
		runtime "Release"
		optimize "speed"
		inlining "auto"
		floatingpoint "Fast"

project "Test"
	location "Test"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "on" 
	intrinsics "on"
	systemversion "latest"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
		""

	}

	includedirs
	{
		"%{prj.name}/src",
		"%{prj.name}/vendor/",
		"TUtil/include/",
	}

	links 
	{
		"TUtil",
	}

	defines
	{
	}

	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		runtime "Release"
		optimize "speed"
		inlining "auto"

