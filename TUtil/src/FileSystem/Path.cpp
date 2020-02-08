#include "TUtil/FileSystem.h"

namespace TUtil {
	const Path Path::INVALID = Path("");

	inline bool Path::operator==(const Path& other) const
	{
		return FileSystem::PathsEqual(this->m_Path.c_str(), other.m_Path.c_str());
	}

	Path Path::GetParent() const
	{
		std::uint64_t index = StringUtils::LastIndexOf(m_Path.c_str(), '/');
		Buf result;
		result.set(m_Path.c_str(), m_Path.c_str() + index);
		return result;
	}

	Path Path::GetChild(const char* childName) const
	{
		Buf result;
		result.set(m_Path);
		result.append(FileSystem::GetSlashCharacter());
		result.append(childName);
		return result;
	}

	File* Path::Open(FileOpenOptions options)
	{
		return File::Open(*this, options);
	}

	File* Path::Open(FileOpenOptions options, FileError& error)
	{
		return File::Open(*this, options, &error);
	}

	void Path::UpdatePath()
	{
		bool isLastArchive = false;
		m_InArchive = false;
		FileSystem::PathNameIterator(m_Path.c_str(), [this, &isLastArchive](const char* fileName, const char* total, const char* rest) -> bool {
			if (isLastArchive)
			{
				m_InArchive = true;
				return true;//Break
			}
			isLastArchive = FileSystem::HasArchiveExtension(fileName);
			return false;
		});
		//Use const cast here since normalizing a path isnt mutating the state of this object from the user's perspective
		Buf normalized;
		FileSystem::NormalizePath(m_Path.c_str(), normalized.c_str(), m_Path.capacity());
		m_Path.set(normalized);
	}

	Path::Path(const char* path) : m_Path(path) { UpdatePath(); }
	Path::Path(const std::string& path) : m_Path(path.c_str()) { UpdatePath(); }
	Path::Path(Str& path) : m_Path(path.c_str()) { UpdatePath(); }

	Path::operator const Str& () { return m_Path; }
	Path::operator char* () { return m_Path.c_str(); }
	Path::operator const char* () const { return m_Path.c_str(); }
	const char* Path::ToString() const { return m_Path.c_str(); }


	bool Path::Exists() const { return FileSystem::Exists(m_Path.c_str()); }
	bool Path::IsDirectory() const { return FileSystem::IsDirectory(m_Path.c_str()); }
	bool Path::IsFile() const { return !IsDirectory(); }

	bool Path::Delete() const { return FileSystem::Delete(m_Path.c_str()); }
	bool Path::Truncate() const { return FileSystem::TruncateFile(m_Path.c_str()); }

	bool Path::CreateFile() { FileSystem::CreateFile(m_Path.c_str()); return Exists(); }
	bool Path::CreateDirectory() { FileSystem::CreateDirectory(m_Path.c_str()); return Exists(); }

	const char* Path::AbsloutePath(char* buf, uint32_t bufLength) { FileSystem::AbsloutePath(m_Path.c_str(), buf, bufLength); return buf; }


	bool Path::IsInvalid() const { return *this == Path::INVALID; }

}
