#include "TUtil/Core.h"
#if 0


#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <errno.h>
#include <string.h>

#include "TUtil/FileSystem.h"

namespace TUtil {

	File::File(const Path& path, FileOpenOptions options) : m_Path(path)
	{
		uint64_t pageSize = System::PageSize();
		m_FileHandle = open(path, O_RDONLY);
		if(m_FileHandle == -1)
		{
			*error = FileError::FILE_NOT_FOUND;
			return;
		}
		struct stat stats;
		if(fstat(m_FileHandle, &stats) == -1)
		{
			*error = FileError::OTHER;
			return;
		}
		m_Length = stats.st_size;

		m_Data = mmap(nullptr, m_Length, PROT_READ, MAP_PRIVATE, m_FileHandle, 0);
		if(m_Data == MAP_FAILED)
		{
			switch(errno)
			{
				case EBADF:
					*error = FileError::FILE_NOT_FOUND;
					break;

				case EACCES:
					*error = FileError::ACCESS_DENIED;
					break;
					
				case EAGAIN:
				case ENOMEM:
				case EOVERFLOW:
					*error = FileError::NOT_ENOUGH_MEMORY;
					break;					
				
				case ENFILE:
					*error = FileError::TOO_MANY_FILES;
					break;
				
				default:
					*error = FileError::OTHER;
					break;
			}
			
			return;
		}
		void* neededAddress = (char*) m_Data + m_Length;
		if (m_Length % pageSize == 0)//There is no null byte since the string takes up an entire page
		{
			m_OtherPage = mmap(neededAddress, pageSize, PROT_READ, MAP_PRIVATE | MAP_ANONYMOUS | MAP_FIXED, -1, 0);
			if (m_OtherPage == MAP_FAILED)
			{	
				munmap(m_OtherPage, pageSize);
				//The page couldn't be allocated where it needed to be
				//So allocate a buffer big enough, release all the file handles, and then give the user the buffer
				char* newData = (char*) malloc(m_Length + 1);
				memcpy(newData, m_Data, m_Length);
				newData[m_Length] = 0x00;//Ensure there is a null byte at the end
				
				m_FreeData = true;
				munmap(m_Data, m_Length);
				m_Data = newData;//Give the user access to the file
			}
		}
		close(m_FileHandle);
		m_FileHandle = 0;
	}

	File::File(File&& other)
	{
		other.m_Length = this->m_Length;
		other.m_Data = this->m_Data;
		other.m_FreeData = this->m_FreeData;
		this->m_Data = nullptr;
		this->m_CompletedDeInit = true;
	}

	File::~File()
	{
		if (!m_CompletedDeInit)
		{
			m_CompletedDeInit = true;
			if (m_FreeData)
			{	//In this case m_Data is the block we malloc'ed
				//We only need to free m_Data because the file handle and view were destroyed in the constructor
				free(m_Data);
			}
			else
			{
				munmap(m_Data, m_Length);
				if (m_OtherPage)//We allocated another page to get space for the null byte
				{
					munmap(m_OtherPage, System::PageSize());
				}
			}
		}

	}
}


#endif
