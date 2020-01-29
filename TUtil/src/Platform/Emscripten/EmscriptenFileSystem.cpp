#include "TUtil/Core.h"
#ifdef T_PLATFORM_EMSCRIPTEN

#include "TUtil/Timer.h"
#include "TUtil/FileSystem/FileSystem.h"
#include "TUtil/FileSystem/FileEnums.h"
#include "TUtil/FileSystem/Path.h"

#include <stdio.h>
#include <stdint.h>

#include <unordered_map>



namespace TUtil {

	bool FileSystem::Exists(const char* path)
	{
		FILE* file = fopen(path, "r");

		if (file)
		{
			fclose(file);
			return true;
		}
		else
		{
			return false;
		}
	}

	std::uint64_t FileSystem::FileSize(const char* path)
	{
		FILE* file = fopen(path, "r");

		if (file)
		{
			fseek(file, 0, SEEK_END);
			std::uint64_t pos = ftell(file);

			fclose(file);
			return pos;
		}
		else
		{
			return INVALID_FILE;
		}
	}

	bool FileSystem::PathsEqual(const char* a, const char* b)
	{
		return StringUtils::Equal(a, b);//Provide Better implementation later
	}

	bool FileSystem::IsDirectory(const char* path)
	{
		return false;
	}

	bool FileSystem::CreateFile(const char* path)
	{
		return false;
	}


	bool FileSystem::CreateDirectory(const char* path)
	{
		return false;
	}

	bool FileSystem::TruncateFile(const char* path)
	{
		return false;
	}

	void FileSystem::AbsloutePath(const char* file, char* buf, uint32_t bufLength)
	{
		if (bufLength)
		{
			buf[0] = '\0';
		}
	}

	bool FileSystem::Delete(const char* path)
	{
		return false;
	}

	//Use of p_ to denote parameters from local vars in this long function
	void* FileSystem::MapFile(const char* p_File, FileOpenOptions p_Options, std::uint64_t& p_FileLength, FileError* p_Error, std::uint64_t p_Bytes, std::uint64_t p_Offset)
	{
		p_FileLength = FileSystem::FileSize(p_File);
		if (p_FileLength == INVALID_FILE)
		{
			*p_Error = FileError::FILE_NOT_FOUND;
			return nullptr;
		}

		if (p_Bytes == ENTIRE_FILE)
		{
			p_Bytes = p_FileLength;
		}

		if (p_Options != FileOpenOptions::READ)//We can only read on JS
		{
			*p_Error = FileError::ACCESS_DENIED;
		}

		uint8_t* data = new uint8_t[p_Bytes];
		FILE* file = fopen(p_File, "r");
		fread(data, p_Bytes, 1, file);
		fclose(file);

		*p_Error = FileError::NONE;
		return data;
	}

	void FileSystem::UnmapFile(const void* file)
	{
		const uint8_t* data = reinterpret_cast<const uint8_t*>(file);
		delete[] data;
	}

}
#endif
