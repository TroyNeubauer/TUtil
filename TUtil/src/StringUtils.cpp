#include "TUtil/StringUtils.h"

#include <cstring>

namespace TUtil {

	namespace StringUtils {

		size_t Length(const char* string)
		{
			if (string == nullptr)
				return 0;
			return std::strlen(string);
		}

		bool Equal(const char* a, const char* b)
		{
			if (a == nullptr || b == nullptr)
				return false;
			return std::strcmp(a, b) == 0;
		}

		bool Equal(const wchar_t* a, const wchar_t* b)
		{
			if (a == nullptr || b == nullptr)
				return false;
			return std::wcscmp(a, b) == 0;
		}

		bool Contains(const char* string, const char* part)
		{
			if (string == nullptr || part == nullptr)
				return false;
			return std::strstr(string, part) != NULL;
		}

		int64_t LastIndexOf(const char* string, const char* stringEnd, char target)
		{
			if (string == nullptr)
				return -1;


			return -1;
		}

		bool EndsWith(const char* string, const char* stringEnd, const char* target, const char* targetEnd)
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

		bool StartsWith(const char* string, const char* target)
		{
			if (string == nullptr || target == nullptr)
				return false;
			char s, t;
			while ((t = *target))
			{
				if (t == 0x00)//The string is shorter than the target
					return false;
				s = *string;
				if (t != s)//They differ
					return false;
				target++;
				string++;
			}
			return true;//We made it to the end of target and every character matched
		}

		int64_t IndexOf(const char* string, char target)
		{
			if (string == nullptr)
				return -1;
			char c;
			uint64_t i = 0;
			while (c = string[i])
			{
				if (c == target)
					return i;
				i++;
			}
			return -1;
		}
	}
}
