newoption {
	trigger     = "compiler",
	description = "Choose a compiler",
	allowed =
	{
		{ "clang",	"Clang LLVM Compiler" },
		{ "gcc",	"GNU Compiler" },
		{ "msc",	"MSVC (Windows only)" },
		{ "", 		"Default" }
	}
}

newoption {
	trigger     = "coverage",
	description = "Compile with code coverage enabled"
}

newoption {
	trigger     = "CI",
	description = "True to indicate that this is being built on a continous integration server"
}

local function add_new_gcc_toolset(name, prefix, suffix)
	local gcc                         = premake.tools.gcc
	local new_toolset                 = {}  
	new_toolset.getcflags             = gcc.getcflags
	new_toolset.getcxxflags           = gcc.getcxxflags
	new_toolset.getforceincludes      = gcc.getforceincludes
	new_toolset.getldflags            = gcc.getldflags
	new_toolset.getcppflags           = gcc.getcppflags
	new_toolset.getdefines            = gcc.getdefines
	new_toolset.getincludedirs        = gcc.getincludedirs
	new_toolset.getLibraryDirectories = gcc.getLibraryDirectories
	new_toolset.getlinks              = gcc.getlinks
	new_toolset.getundefines          = gcc.getundefines
	new_toolset.getmakesettings       = gcc.getmakesettings
	new_toolset.getrunpathdirs        = gcc.getrunpathdirs

	new_toolset.toolset_prefix        = prefix
	new_toolset.toolset_suffix		  = suffix

	function new_toolset.gettoolname (cfg, tool)  
		if tool == "cc" then
			name = new_toolset.toolset_prefix.."gcc"..new_toolset.toolset_suffix
		elseif tool == "cxx" then
			name = new_toolset.toolset_prefix.."g++"..new_toolset.toolset_suffix
		elseif tool == "ar" then
			name = new_toolset.toolset_prefix.."ar"
		end
		return name
	end  

	premake.tools[name] = new_toolset

	return name
end

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

function myCopyFile(srcFile, destFile)
	local srcStats = os.stat(srcFile)

	local destStats = os.stat(destFile)
	if srcStats == nil then
		print("ERROR: Failed to find src file: "..srcFile)
		return
	end
	if destStats ~= nil then
		if destStats.mtime > srcStats.mtime then
			return
		end
	end


	os.copyfile(srcFile, destFile)

end

function copySrcFile(file)
	local destPath = "./TUtil/src/vendor/"..file
	mkdirs(destPath)

	myCopyFile("./TUtil/vendor/"..file, destPath)
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
			myCopyFile(srcFile, destFile)
		end
	end
end


function copyHeaderFile(file)
	local destFile = "./TUtil/include/TUtil/vendor/"..file
	mkdirs(destFile)

	print("Copying: "..destFile)
	local srcFile = "./TUtil/vendor/"..file

	myCopyFile(srcFile, destFile)

end


workspace "TUtil"
	architecture "x64"
	startproject "Test"

	configurations
	{
		"Debug",
		"Release",
	}

	if os.target() == "emscripten" then
		require("emscripten")
	end

	if _OPTIONS["compiler"] and _OPTIONS["compiler"] ~= "" then
		print("Using compiler ".._OPTIONS["compiler"])
		toolset(_OPTIONS["compiler"])
	end

	if _OPTIONS["coverage"] and _OPTIONS["compiler"] == "gcc" then
		add_new_gcc_toolset("gcc-9", "/usr/bin/", "-9")--Force gcc9
		toolset "gcc-9"
		print "Forcing gcc-9"
	end

	if _OPTIONS["CI"] then
		defines
		{
			"CI_BUILD",
		}
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

	if _OPTIONS["coverage"] then
		buildoptions { "--coverage", "-fprofile-abs-path" }
	end


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

	if _OPTIONS["coverage"] then
		buildoptions { "-fprofile-abs-path" }

		if os.target() == "linux" then
			links
			{
				"gcov",
			}
		end
	end

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


