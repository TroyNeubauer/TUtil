#include "TUtil/Core.h"
#ifdef T_PLATFORM_WINDOWS

#include "TUtil/FileSystem.h"
#include "WindowsUtils.h"
#include "TUtil/StringUtils.h"

namespace TUtil {


	static thread_local wchar_t winUTF16Buf[1024];
	static thread_local char winUTF8Buf[1024];

	static wchar_t* ToUTF16(const char* text, wchar_t* dest, int bytes)
	{
		int result = MultiByteToWideChar(CP_UTF8, 0, text, -1, dest, bytes / sizeof(wchar_t));
		if (result == 0)
		{
			char buf[256];
			WindowsUtils::GetLastErrorMessage(buf, sizeof(buf));
			T_ERROR("Unable to convert string: \"%s\" to UTF16. The error message is: %s\n", text, buf);
		}
		return dest;
	}

	static wchar_t* ToUTF16(const char* text)
	{
		return ToUTF16(text, winUTF16Buf, sizeof(winUTF16Buf));
	}


	static const char* FromUTF16(const wchar_t* text, char* dest, int bytes)
	{
		int result = WideCharToMultiByte(CP_UTF8, 0, text, -1, dest, bytes, nullptr, nullptr);
		if (result == 0)
		{
			char buf[256];
			WindowsUtils::GetLastErrorMessage(buf, sizeof(buf));
			T_ERROR("Unable to convert string from UTF16 to UTF8. The error message is: %s\n", buf);
		}
		return dest;
	}

	static const char* FromUTF16(const wchar_t* text)
	{
		return FromUTF16(text, winUTF8Buf, sizeof(winUTF8Buf));
	}

	bool FileSystem::Exists(const char* path)
	{
		DWORD atts = GetFileAttributesW(ToUTF16(path));
		return atts != INVALID_FILE_ATTRIBUTES;
	}

	bool FileSystem::PathsEqual(const char* a, const char* b)
	{
		wchar_t fullA[512], fullB[512];
		GetFullPathNameW(ToUTF16(a), sizeof(fullA), fullA, nullptr);
		GetFullPathNameW(ToUTF16(b), sizeof(fullB), fullB, nullptr);

		return StringUtils::Equal(fullA, fullB);
	}

	std::uint64_t FileSystem::FileSize(const char* path)
	{
		HANDLE handle = CreateFileW(ToUTF16(path), GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
		if (handle == INVALID_HANDLE_VALUE)
		{
			return INVALID_FILE;
		}
		LARGE_INTEGER size;
		GetFileSizeEx(handle, &size);
		CloseHandle(handle);
		return size.QuadPart;
	}

	bool FileSystem::IsDirectory(const char* path)
	{
		DWORD atts = GetFileAttributesW(ToUTF16(path));
		return atts & FILE_ATTRIBUTE_DIRECTORY;
	}

	bool FileSystem::CreateFile(const char* path)
	{
		HANDLE handle = CreateFileW(ToUTF16(path), GENERIC_READ, 0, nullptr, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, nullptr);
		if (handle == INVALID_HANDLE_VALUE)
		{
			return false;
		}
		CloseHandle(handle);//A bit wasteful. Looking into assigning the handle to a path might be more efficent since Paths are usually opened anyway
		return true;
	}

	bool FileSystem::CreateDirectory(const char* path)
	{
		
		if (CreateDirectoryW(ToUTF16(path), nullptr))
		{
			return true;
		}
		return Exists(path);
	}

	bool FileSystem::TruncateFile(const char* path)
	{
		HANDLE handle = CreateFileW(ToUTF16(path), GENERIC_WRITE, 0, nullptr, TRUNCATE_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
		if (handle == INVALID_HANDLE_VALUE)
		{
			if (GetLastError() == ERROR_FILE_NOT_FOUND)
				return FileSystem::CreateFile(path);//Try to create it if it didnt exist
			else
				return false;
		}
		CloseHandle(handle);//A bit wasteful. Looking into assigning the handle to a path might be more efficent since Paths are usually opened anyway
		return true;
	}

	void FileSystem::AbsloutePath(const char* file, char* buf, uint32_t bufLength)
	{
		GetFullPathNameA(file, bufLength, buf, nullptr);
	}

	bool FileSystem::Delete(const char* path)
	{
		return DeleteFileW(ToUTF16(path));
	}


	//Use of p_ to denote parameters from local vars in this long function
	void* FileSystem::MapFile(const char* p_File, FileOpenOptions p_Options, std::uint64_t& p_FileLength, FileError* p_Error, std::uint64_t p_Bytes, std::uint64_t p_Offset)
	{
		if (p_Bytes == ENTIRE_FILE)
		{
			p_Bytes = 0;
			p_Offset = 0;
		}
		std::uint64_t pageSize = System::PageSize();

		DWORD flags = (p_Options & FileOpenOptions::RANDOM_ACCESS) ? FILE_FLAG_RANDOM_ACCESS : FILE_FLAG_SEQUENTIAL_SCAN;
		DWORD desiredAccess = 0;
		if (p_Options & READ) desiredAccess |= GENERIC_READ;
		if (p_Options & WRITE) desiredAccess |= GENERIC_WRITE;
		
		HANDLE handle = CreateFileW(ToUTF16(p_File), desiredAccess, FILE_SHARE_READ, NULL, OPEN_EXISTING, flags, NULL);

		if (handle == INVALID_HANDLE_VALUE)
		{
			if (p_Error) {
				DWORD lastError = GetLastError();
				switch (lastError) {
				case ERROR_FILE_NOT_FOUND:
				case ERROR_PATH_NOT_FOUND:
					*p_Error = FileError::FILE_NOT_FOUND;
					break;
				case ERROR_ACCESS_DENIED:
					*p_Error = FileError::ACCESS_DENIED;
					break;
				case ERROR_TOO_MANY_OPEN_FILES:
					*p_Error = FileError::TOO_MANY_FILES;
					break;
				default:
					*p_Error = FileError::UNKNOWN;
					break;
				}
			}
			return nullptr;
		}

		LARGE_INTEGER size;
		if (!GetFileSizeEx(handle, &size))
		{
			char errorMessage[1024];
			WindowsUtils::GetLastErrorMessage(errorMessage, sizeof(errorMessage));
			//HZ_CORE_ERROR("Failed to get file length for file \"{0}\", error from GetFileSizeEx is: {1}", path, errorMessage);
			if (p_Error)
				*p_Error = FileError::UNKNOWN;
			goto fail;
		}
		p_FileLength = size.QuadPart;
		if (p_FileLength == 0)
		{
			T_ASSERT(false, "Unable to memory map file of length 0!");
		}

		DWORD protect = 0;
		if (p_Options & WRITE) protect = PAGE_READWRITE;//There is no write only memory mapping, so if they went to write they must have read write access
		else if (p_Options & READ) protect = PAGE_READONLY;
		else//They didnt specify a valid access mode
		{
			if (p_Error)
				* p_Error = FileError::INVALID_PARAMETER;
			goto fail;
		}
		HANDLE viewHandle = CreateFileMappingW(handle, nullptr, protect, (p_Bytes >> 32) & 0xFFFFFFFFULL, p_Bytes & 0xFFFFFFFFULL, nullptr);

		if (viewHandle == nullptr)
		{
			char errorMessage[1024];
			WindowsUtils::GetLastErrorMessage(errorMessage, sizeof(errorMessage));
			T_ERROR("Failed to create file mapping for file: \"%s\", error from CreateFileMappingA is: %s", p_File, errorMessage);
			if (p_Error)
				*p_Error = FileError::UNKNOWN;
			goto fail;
		}
		
		DWORD access = 0;
		if (p_Options & READ) access |= FILE_MAP_READ;
		if (p_Options & WRITE) access |= FILE_MAP_WRITE;

		void* data = MapViewOfFile(viewHandle, access, (p_Offset & 0xFFFFFFFF00000000ULL) >> 32, (p_Offset & 0xFFFFFFFFULL), p_Bytes);
		if (data == nullptr)
		{
			char errorMessage[1024];
			WindowsUtils::GetLastErrorMessage(errorMessage, sizeof(errorMessage));
			T_ERROR("Failed to map view of file: \"%s\", error from MapViewOfFile2 is: %s", p_File, errorMessage);
			if (p_Error)
				* p_Error = FileError::UNKNOWN;//FIXME: report proper errors
			CloseHandle(viewHandle);
			goto fail;
		}
		CloseHandle(viewHandle);//Clean up the other resources
		CloseHandle(handle);

		*p_Error = FileError::NONE;
		return data;

	fail:
		CloseHandle(handle);
		return nullptr;

	}

	void FileSystem::UnmapFile(const void* file)
	{
		UnmapViewOfFile(file);
	}

	/*File::File(File&& other) : m_Path(other.GetPath())
	{
		other.m_Length = this->m_Length;
		other.m_FileHandle = this->m_FileHandle;
		other.m_Data = this->m_Data;
		other.m_FreeData = this->m_FreeData;

		this->m_FileHandle = INVALID_FILE_HANDLE;
		this->m_Data = nullptr;
	}

	File::~File()
	{
		if (!m_CompletedDeInit)
		{
			m_CompletedDeInit = true;
#ifdef HZ_DEBUG
			Log_fclose((FILE*)m_FileHandle, 0, __FILE__, __LINE__);
#elif HZ_RELEASE
			Log_fclose((FILE*)m_FileHandle, 0);
#endif
			if (m_FreeData)
			{	//In this case m_Data is the block we malloc'ed
				//We only need to free m_Data because the file handle and view were destroyed in the constructor
				free(m_Data);
			}
			else
			{
				CloseHandle(m_FileHandle);
				UnmapViewOfFile(m_Data);
				if (m_OtherPage)//We allocated another page to get space for the null byte
				{
					VirtualFree(m_OtherPage, 0, MEM_RELEASE);
				}
			}
		}

	}*/

}

#endif
