

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


outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

VendorIncludeDir = "TUtil/include/TUtil/vendor"

project "TUtil"
	location "TUtil"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	intrinsics "on"
	systemversion "latest"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")


	print "Copying dependent files..."
	copyHeaderFile("str/Str.h")
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

	filter "system:windows"
		excludes "%{prj.name}/src/Platform/Unix"

	filter "system:linux"
		excludes "%{prj.name}/src/Platform/Windows"
	


	links 
	{ 
	}

	defines
	{
		"LIBARCHIVE_STATIC",
	}

	filter "system:windows"
		defines "PLATFORM_CONFIG_H=\"TUtil/vendor/libarchive/win_config.h\""

	filter "system:linux"
		defines "PLATFORM_CONFIG_H=\"TUtil/vendor/libarchive/linux_config.h\""

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
