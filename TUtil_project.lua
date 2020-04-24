

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
			print("copying: "..srcFile.." to: "..destFile)
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

function copyHeaderFiles(path, path2)
	local srcPath = "./TUtil/vendor/"..path
	local destPath = "./TUtil/include/TUtil/vendor/"..path2
	print("Copying header files to Folder: "..destPath)

	for file in io.popen("dir \""..srcPath.."\" /b"):lines() do
		if ends_with(file, ".h") or ends_with(file, ".hpp") then
			local srcFile = srcPath.."/"..file
			local destFile = destPath.."/"..file
			mkdirs(destFile)
			print("copying: "..srcFile.." to: "..destFile)
			myCopyFile(srcFile, destFile)
		end
	end
end

function TUtilDependencies()
	includedirs
	{
		"vendor/TUtil/TUtil/include/",
	}

	links
	{
		"TUtil",
		"bz2",
		"z",
		"lz4",
		"nettle",
	}
end



outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}/"
emscriptenOutputdir = "%{cfg.buildcfg}-emcc/"

VendorIncludeDir = "TUtil/include/TUtil/vendor"

project "TUtil"
	location "TUtil"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	intrinsics "on"
	systemversion "latest"

	print "Copying dependent files..."
	copyHeaderFile("str/Str.h")
	copyHeaderFiles("libarchive/libarchive", "libarchive")

	copySrcFile("str/Str.cpp")
	copySrcFiles("libarchive/libarchive", "libarchive")

	files
	{
		"%{prj.name}/src/**.c",
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/include/**.h",
	}

	includedirs
	{
		"%{prj.name}/include/",
		"TUtil/vendor/libarchive/libarchive",
		"%{VendorIncludeDir}/str"
	}

	links 
	{ 
	}

	defines
	{
		"LIBARCHIVE_STATIC",
	}

	filter "system:windows or macosx or linux"
		if binLocationOverride then
			targetdir (binLocationOverride.."bin/" .. outputdir)
			objdir (binLocationOverride.."bin-int/" .. outputdir)
		else
			targetdir ("bin/" .. outputdir)
			objdir ("bin-int/" .. outputdir)
		end

	filter "system:emscripten"
		--On emscripten dump the binaries in the same dir to avoid passing very long path names to the compiler
		targetdir ("bin/" .. outputdir)
		objdir ("bin-int/" .. outputdir)

	filter "system:windows"
		excludes "%{prj.name}/src/Platform/Unix"
		excludes "%{prj.name}/src/Platform/Emscripten"

	filter "system:linux or macosx"
		excludes "%{prj.name}/src/Platform/Windows"
		excludes "%{prj.name}/src/Platform/Emscripten"

	filter "system:emscripten"
		excludes "%{prj.name}/src/Platform/Windows"
		excludes "%{prj.name}/src/Platform/Unix"
	



	filter "system:windows"
		defines "PLATFORM_CONFIG_H=\"TUtil/vendor/libarchive/win_config.h\""

	filter "system:linux or macosx"
		defines "PLATFORM_CONFIG_H=\"TUtil/vendor/libarchive/linux_config.h\""

	filter "system:emscripten"
		defines "PLATFORM_CONFIG_H=\"TUtil/vendor/libarchive/emscripten_config.h\""

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
