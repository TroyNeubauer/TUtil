

function mkdirs(file)
	local total = "."
	local path = string.match(file, "(.-)([^/]-([^%.]+))$")

	for part in string.gmatch(path, "([^/]+)") do
		if part ~= "." then
			total = total.."/"..part
			os.mkdir(total)
		end
	end


end

function copySrcFile(file)
	local destPath = "./TUtil/src/vendor/"..file
	mkdirs(destPath)

	print("Copying: "..destPath)
	os.copyfile("./TUtil/vendor/"..file, destPath)
end

function copySrcFiles(path)
	local srcPath = "./TUtil/vendor/"..path
	local destPath = "./TUtil/src/vendor/"..path
	mkdirs(destPath)

	--for fname in lfs.dir(srcPath) do
	--	print("files are: "..fname)
	--end

	--print("Copying from Folder: "..destPath)
	os.copyfile(srcPath, destPath)
end


function copyHeaderFile(file)
	local destPath = "./TUtil/include/TUtil/vendor/"..file
	mkdirs(destPath)

	print("Copying: "..destPath)
	os.copyfile("./TUtil/vendor/"..file, destPath)

end


workspace "TUtil"
	architecture "x64"
	startproject "Test"

	configurations
	{
		"Debug",
		"Release",
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- Include directories relative to root folder (solution directory)
IncludeDir = {}

VendorIncludeDir = "TUtil/include/TUtil/vendor"

project "TUtil"
	location "TUtil"
	kind "StaticLib"
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
		"%{VendorIncludeDir}/str",
		"%{VendorIncludeDir}/libarchive/libarchive",
		"TUtil/vendor/libarchive/libarchive",
	}

	print "Copying dependent files..."
	copyHeaderFile("str/Str.h")
	copySrcFile("str/Str.cpp")
	--copySrcFiles("libarchive/libarchive")

	links 
	{ 
	}

	defines
	{
		"LIBARCHIVE_STATIC",
	}

	filter "system:windows"
		links
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
		"%{prj.name}/src/**.c",
	}

	includedirs
	{
		"%{prj.name}/src",
		"%{prj.name}/vendor/",
		"TUtil/include/",
		"%{VendorIncludeDir}/str",
		"%{VendorIncludeDir}/libarchive/libarchive",
		"TUtil/vendor/libarchive/libarchive",
	}

	links 
	{
		"TUtil",
	}

	defines
	{
	}

	filter "system:windows"
		links
		{
			"Pdh.lib",
			"kernel32.lib",
		}

	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		runtime "Release"
		optimize "speed"
		inlining "auto"


