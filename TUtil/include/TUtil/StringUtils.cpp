#include "StringUtils.h"

bool TUtil::StringUtils::EndsWith(const char* string, const char* stringEnd, const char* part, const char* partEnd)
{
	size_t len = stringEnd - string;
	size_t partLength = partEnd - part;
	if (partLength > len)
		return false;//The pattern is longer than the string
	size_t partOffset = len - partLength;
	for (int i = 0; i < partLength; i++)
	{
		if (string[partOffset + i] != part[i])
			return false;
	}
	return true;
}

constexpr size_t TUtil::StringUtils::Length(const char* string)
{
	return *string ? 1 + Length(string + 1) : 0;
}
