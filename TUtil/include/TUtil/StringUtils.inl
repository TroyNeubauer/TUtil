#pragma once

namespace TUtil {
	
	constexpr bool EndsWithHelper(const char* stringEnd, const char* target, const char* targetEnd)
	{
		if (targetEnd < target)//We have gone through all of target without a hit
			return false;
		if (*targetEnd == *stringEnd)
		{
			if (targetEnd == target)
				return true;//The entire target matches!
			else
				return EndsWithHelper(stringEnd - 1, target, targetEnd - 1);
		}
		else
			return false;
	}

	constexpr bool StringUtils::EndsWith(const char* string, const char* stringEnd, const char* target, const char* targetEnd)
	{
		if (string == nullptr || target == nullptr)
			return false;
		if (Length(target, targetEnd) > Length(string, stringEnd))
			return false;
		return EndsWithHelper(stringEnd - 1, target, targetEnd - 1);
	}

	constexpr bool StringUtils::StartsWith(const char* string, const char* target)
	{
		if (string == nullptr || target == nullptr) return false;
		return (*string == *target) && StartsWith(string + 1, target + 1);
	}

	constexpr size_t StringUtils::Length(const char* string)
	{
		if (string == nullptr || *string == 0x00)
			return 0;
		else
			return 1 + Length(string + 1);
	}

	template<typename T, typename F>
	void StringUtils::Until(T*& string, F end)
	{
		while (*string && !end(*string))
			string++;
	}



	constexpr size_t IndexOfHelper(const char* string, char target, size_t i)
	{
		if (string[i] == 0x00)
			return -1;
		if (string[i] == target)
			return i;
		return IndexOfHelper(string, target, i + 1);
	}

	constexpr size_t StringUtils::IndexOf(const char* string, char target)
	{
		if (string == nullptr) return -1;
		return IndexOfHelper(string, target, 0);
	}



	constexpr size_t LastIndexOfHelper(const char* string, char target, size_t i)
	{
		if (string[i] == target)
			return i;
		else if (i == 0)//We've reached the beginning of the string
			return -1;
		return LastIndexOfHelper(string, target, i - 1);
	}

	constexpr size_t StringUtils::LastIndexOf(const char* string, const char* stringEnd, char target)
	{
		if (string == nullptr)
			return -1;
		return LastIndexOfHelper(string, target, stringEnd - string - 1);
	}



	template<typename T>
	void StringUtils::Find(T*& string, const char* target)
	{
		const char* targetStart = target;
		while (*string && *target)
		{
			if (*string == *target) target++;//Advance. If we reach the end of the string target[0] will be null and we will return
			else target = targetStart;
			string++;
		}
	}

	template<typename T>
	T* StringUtils::FindBegin(T*& string, const char* target)
	{
		if (*string == 0x00 || *target == 0x00) return string;
		const char* targetStart = target;
		T* start;//Guaranteed to be assigned the first time a character is matched
		while (*string)
		{
			if (*string == *target) {//A character matches
				if (target == targetStart)//We are starting the beginning of a new match so save the place in the string
					start = string;
				target++;
				if (*target == 0x00)//We matched the entire target
				{
					T* end = string;
					string = start;//Reset string to point to the start of the target
					return end + 1;
				}
			}
			else
			{
				target = targetStart;//Bring back target since we lost the old one
			}
			string++;
		}
		return string;
	}

	template<typename T>
	void StringUtils::NextLine(T*& string)
	{
		char endingChar = 0x00;
		Until(string, [&endingChar](char current)
		{
			if (current == '\r' || current == '\n')
			{
				endingChar = current;
				return true;
			}
			else
				return false;
		});//Continue until we find a newline
		string++;//Skip the first endline character

		//If \r\n is used we need to consume the \n
		if (endingChar == '\r' && *string == '\n')
			string++;
	}
}
