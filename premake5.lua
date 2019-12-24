newoption {
	trigger     = "compiler",
	value       = "compiler",
	description = "Choose a compiler",
	default     = "",
	allowed =
	{
		{ "clang",    "Clang LLVM Compiler" },
		{ "gcc",  "GNU Compiler" },
		{ "msc",  "MSVC (Windows only)" },
	}
}


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

local function ends_with(str, ending)
   return ending == "" or str:sub(-#ending) == ending
end

function copySrcFiles(path, path2)
	local srcPath = "./TUtil/vendor/"..path
	local destPath = "./TUtil/src/vendor/"..path2
	print("Copying source files to Folder: "..destPath)

	for file in io.popen("dir \""..srcPath.."\" /b"):lines() do
		if ends_with(file, ".c") or ends_with(file, ".cpp") then
			local srcFile = srcPath.."/"..file
			local destFile = destPath.."/"..file
			mkdirs(destFile)
			--print("copying: "..srcFile.." to: "..destFile)
			os.copyfile(srcFile, destFile)
		end
	end
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

	if _OPTIONS["compiler"] ~= "" then
		print("Using compiler ".._OPTIONS["compiler"])
		toolset(_OPTIONS["compiler"])
	end
	

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
	
	print "Copying dependent files..."

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
	copySrcFiles("libarchive/libarchive", "libarchive")

	links 
	{ 
	}

	defines
	{
		"LIBARCHIVE_STATIC",
	}

	filter "system:windows"
		defines "_CRT_SECURE_NO_WARNINGS"

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
		defines "_CRT_SECURE_NO_WARNINGS"
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


