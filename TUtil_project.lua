

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


outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

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
