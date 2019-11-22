#include "TUtil/StringUtils.h"

namespace TUtil {

	bool StringUtils::Equal(const char* a, const char* b)
	{
		if (a == nullptr || b == nullptr)
			return false;
		return std::strcmp(a, b) == 0;
	}

	bool StringUtils::Equal(const wchar_t* a, const wchar_t* b)
	{
		if (a == nullptr || b == nullptr)
			return false;
		return std::wcscmp(a, b) == 0;
	}

	bool StringUtils::Contains(const char* string, const char* part)
	{
		if (string == nullptr || part == nullptr)
			return false;
		return std::strstr(string, part) != NULL;
	}

	bool StringUtils::EndsWithNConstexpr(const char* string, const char* stringEnd, const char* target, const char* targetEnd)
	{
		if (string == nullptr || target == nullptr || *target == 0)
			return false;
		size_t targetLen = Length(target, targetEnd);
		size_t len = Length(string, stringEnd);
		size_t targetOffset = len - targetLen;
		if (targetLen > len)
			return false;
		for (size_t i = 0; i < targetLen; i++)
		{
			if (string[i + targetOffset] != target[i])
				return false;
		}
		return true;
	}
}