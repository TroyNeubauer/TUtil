#include "TUtil/Core.h"
#ifdef T_PLATFORM_UNIX

#include "TUtil/FileSystem.h"
#include "TUtil/Timer.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>

#include <fcntl.h>
#include <unistd.h>


namespace TUtil {

	bool FileSystem::Exists(const char* path)
	{
		return access(path, F_OK);
	}

	uint64_t FileSystem::FileSize(const char* path)
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
		int fd = open(path, O_CREAT);
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

	bool FileSystem::CreateFileWithParents(const char* path)
	{
		return true;
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
			if (errno == EEXIST)
				return true;
			else
				return false;
		}
	}

	bool FileSystem::CreateDirectories(const char* path)
	{

		return true;
	}

	bool FileSystem::TruncateFile(const char* path)
	{
		int fd = open(path, O_CREAT | O_TRUNC | O_WRONLY);
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

	void FileSystem::AbsloutePath(const char* file, char* buf, uint32_t bufLength)
	{
	}

	bool FileSystem::Delete(const char* path)
	{
		if (Exists(path))
		{
			if (IsDirectory(path))
			{
				//TODO: Delete files in dir if there are any
				return rmdir(path) == 0;
			}
			else
			{
				return unlink(path) == 0;
			}
		}
		return false;
	}


	//Use of p_ to denote parameters from local vars in this long function
	void* FileSystem::MapFile(const char* p_File, FileOpenOptions p_Options, uint64_t& p_FileLength, FileError* p_Error, uint64_t p_Bytes, uint64_t p_Offset)
	{
		uint64_t realLength = FileSystem::FileSize(p_File);
		if (realLength == INVALID_FILE)
		{
			*p_Error = FileError::FILE_NOT_FOUND;
			return nullptr;
		}

		p_FileLength = realLength;
		if (p_Bytes == ENTIRE_FILE)
		{
			p_Bytes = realLength;
		}
		int flags = 0;
		if ((p_Options & FileOpenOptions::READ) && (p_Options & FileOpenOptions::WRITE))
			flags |= O_RDWR;
		else if (p_Options & FileOpenOptions::READ)
			flags |= O_RDONLY;
		else if (p_Options & FileOpenOptions::WRITE)
			flags |= O_WRONLY;

		int fd = open(p_File, flags);
		if (fd == 0)
		{
			*p_Error = FileError::FILE_NOT_FOUND;
		}
		void* result = mmap(nullptr, p_Bytes, PROT_READ, MAP_PRIVATE, fd, p_Offset);
		if (result == nullptr)
		{
			*p_Error = FileError::FILE_NOT_FOUND;
		}
		return result;
	}

	void FileSystem::UnmapFile(void* file)
	{

	}

}
#endif
