#include "TUtil/Core.h"
#ifdef T_PLATFORM_UNIX

#include "TUtil/Timer.h"
#include "TUtil/FileSystem/FileSystem.h"
#include "TUtil/FileSystem/FileEnums.h"
#include "TUtil/FileSystem/Path.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>

#include <fcntl.h>
#include <unistd.h>

#include <unordered_map>

#ifdef T_PLATFORM_OSX
	#include <mach/error.h>
	#include <sys/errno.h>
#endif


namespace TUtil {

	bool FileSystem::Exists(const char* path)
	{
		struct stat64 data;
		return stat64(path, &data) == 0;
	}

	std::uint64_t FileSystem::FileSize(const char* path)
	{
		struct stat64 data;
		int result = stat64(path, &data);
		if (result == -1)
		{//Error
			return INVALID_FILE;
		}
		return data.st_size;
	}

	bool FileSystem::PathsEqual(const char* a, const char* b)
	{
		return StringUtils::Equal(a, b);//Provide Better implementation later
	}

	bool FileSystem::IsDirectory(const char* path)
	{
		struct stat64 data;
		int result = stat64(path, &data);
		if (result == -1)
		{//Error
			return false;
		}
		return data.st_mode & S_IFDIR;
	}

	bool FileSystem::CreateFile(const char* path)
	{
		int fd = open(path, O_CREAT, 0777);
		if (fd != -1)// Success
		{
			close(fd);
			return true;
		}
		else//Error
		{
			return false;
		}
	}


	bool FileSystem::CreateDirectory(const char* path)
	{
		int result = mkdir(path, S_IRWXU | S_IRWXG | S_IRWXO);
		if (result == 0)
		{//Error
			return true;
		}
		else
		{
			int error = errno;
			if (error == EEXIST)
				return true;
			else
				return false;
		}
	}

	bool FileSystem::TruncateFile(const char* path)
	{
		int fd = open(path, O_TRUNC | O_WRONLY);
		if (fd != -1)// Success
		{
			close(fd);
			return true;
		}
		else//Error
		{
			fd = open(path, O_CREAT | O_WRONLY, 0777);
			if (fd != -1)// Success
			{
				close(fd);
				return true;
			}
			else//Error
			{
				fprintf(stderr, "Failed to open file %s, Error: %i", path, errno);
				return false;
			}
		}
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
		return remove(path) == 0;
	}


	static std::unordered_map<const void*, size_t> s_FileMappings;

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
		int flags = 0;
		if ((p_Options & FileOpenOptions::READ) && (p_Options & FileOpenOptions::WRITE))
			flags |= O_RDWR;
		else if (p_Options & FileOpenOptions::READ)
			flags |= O_RDONLY;
		else if (p_Options & FileOpenOptions::WRITE)
			flags |= O_WRONLY;

		errno = 0;
		int fd = open(p_File, flags);
		if (fd == 0)
		{
			*p_Error = FileError::FILE_NOT_FOUND;
			return nullptr;
		}


		flags = MAP_SHARED;
		if (p_Bytes > (1024L * 1024L * 1024L))
		{
			flags |= MAP_NORESERVE;
		}


		void* result = mmap(nullptr, p_Bytes, PROT_READ, flags, fd, p_Offset);
		close(fd);
		if (result == MAP_FAILED)
		{
			//TODO: FILE_NOT_FOUND, ACCESS_DENIED, TOO_MANY_FILES, NOT_ENOUGH_MEMORY, IS_DIRECTORY, IS_FILE, INVALID_PARAMETER, UNKNOWN

			*p_Error = FileError::FILE_NOT_FOUND;
			return nullptr;
		}
		s_FileMappings[result] = p_Bytes;

		*p_Error = FileError::NONE;
		return result;
	}

	void FileSystem::UnmapFile(const void* file)
	{
		auto it = s_FileMappings.find(file);
		if (it != s_FileMappings.end())
		{
			munmap(const_cast<void*>(file), it->second);
			s_FileMappings.erase(it);
		}
	}

}
#endif
