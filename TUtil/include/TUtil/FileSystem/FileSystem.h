#pragma once

#include "TUtil/Core.h"
#include "TUtil/StringUtils.h"
#include "FileEnums.h"

#include <functional>
#include <utility>

#include <stdint.h>


#ifdef T_PLATFORM_WINDOWS
	#undef CreateFile
	#undef CreateDirectory
	#undef DeleteFile
#endif

namespace TUtil {

	enum class FileError;

	class FileSystem
	{
	public:
		static const std::uint64_t ENTIRE_FILE = (std::uint64_t) -1, INVALID_FILE = (std::uint64_t) -1;
		//Maps a section of a file into virtual memory
		//This function returns the pointer to the memory and the length of the file
		static void* MapFile(const char* file, FileOpenOptions options, std::uint64_t& fileLength, FileError* error, std::uint64_t bytes = ENTIRE_FILE, std::uint64_t offset = 0);
		static void UnmapFile(const void* file);

		//The following functions return true if their operation succeeded and false otherwise.
		static bool Exists(const char* path);
		static bool IsDirectory(const char* path);

		static std::uint64_t FileSize(const char* path);

		//Creates a new file with the name and location specified by path
		//The return value indicates weather or not a new file was created. 
		//Therefore asking CreateFile to create a file that already exists will return false indicating that the operation failed since no file was actually created. 
		//if the requesite parent directories dont exist, CreateFile will return false and no further action will be taken
		static bool CreateFile(const char* path);

		//Creates a new file with the name and location specified by path
		//The difference between this function and CreateFile is that CreateFileWithParents will create any non existant parent directories before trying to create the file
		static bool CreateFileWithParents(const char* path);

		//CreateDirectory and CreateDirectories are the directory versions of CreateFile and CreateFileWithParents
		//They function identically except these functions create directories instead
		static bool CreateDirectory(const char* path);
		static bool CreateDirectories(const char* path);
		
		//Truncates a file to have a length of 0. 
		//If the file doesnt exist, this method creates a new empty file. 
		//Returns true if the specified file is now a length of 0
		static bool TruncateFile(const char* path);

		//Copies the absloute path of file into buf
		static void AbsloutePath(const char* file, char* buf, uint32_t bufLength);

		//Deletes a file or directory
		//If path points to a directory that contains files, each file will be removed before the directory is deleted
		//If Delete returns false (indicating that the operation failed), some child files may have been removed even if removing their parent directory failed
		static bool Delete(const char* path);

		static void NormalizePath(const char* src, char* dest, uint32_t destSize);

		//Calls onPath() for each component in path with the name of the file or directory. This path must be normalized. ../ results in undefined behavior
		//The boolean return value in the lambda is used to determine weather to continue or not. True means break and iterate no more, false means continue
		//This is marked always inline because onPath will almost always be a lambda, therefore inlining this function will usually
		//lead the compiler to inline the lambda too
		template<typename F>
		inline static void PathNameIterator(const char* path, F onPath);
		template<typename F>
		inline static void PathNameIterator(char* path, F onPath);

		static bool PathsEqual(const char* a, const char* b);

		//Tests if the extension of fileName equals extension. For example IsExtension("test.png", "png"); would return true
		static bool IsExtension(const char* fileName, const char* extension);

		static inline bool IsSlash(char c){ return (c == '/' || c == '\\'); }

		static inline char NormalizeSlash(char c) { if (IsSlash(c)) { return GetSlashCharacter(); } else { return c; } }

		static inline char GetSlashCharacter()
		{
#ifdef T_PLATFORM_WINDOWS
			return '\\';
#else
			return '/';
#endif
		}

		static inline bool HasArchiveExtension(const char* fileName) { return StringUtils::ContainsAny(fileName, ".zip", ".pax", ".cpio", ".tar", ".mtree", ".ar"); }
	};

}


#include "FileSystem.inl"
