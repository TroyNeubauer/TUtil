#pragma once

namespace TUtil {
	namespace StringUtils {

		namespace Constexpr {
		
		
			template<typename T>
			constexpr bool Equal(const T* a, const T* b)
			{
				if (a == nullptr || b == nullptr)
					return false;
					
				char aa = *a, bb = *b;
				if (aa != bb)
					return false;
				else if (aa == 0x00 && bb == 0x00)
					return true;
				return Equal(a + 1, b + 1);			
			}
			
			constexpr bool Contains(const char* string, const char* target)
			{
				return false;
			}
		
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
		
			constexpr bool EndsWith(const char* string, const char* stringEnd, const char* target, const char* targetEnd)
			{
				if (string == nullptr || target == nullptr)
					return false;
				if (Length(target, targetEnd) > Length(string, stringEnd))
					return false;
				return EndsWithHelper(stringEnd - 1, target, targetEnd - 1);
			}
			
			constexpr bool StartsWithHelper(const char* string, const char* target)
			{
				return (*target == 0x00) || ((*string == *target) && StartsWithHelper(string + 1, target + 1));
			}
		
			constexpr bool StartsWith(const char* string, const char* target)
			{
				if (string == nullptr || target == nullptr || *string == 0x00 || *target == 0x00) return false;
				return StartsWithHelper(string, target);
			}
		
			constexpr size_t Length(const char* string)
			{
				if (string == nullptr || *string == 0x00)
					return 0;
				else
					return 1 + Length(string + 1);
			}
		
		
			constexpr int64_t IndexOfHelper(const char* string, char target, size_t i)
			{
				if (string[i] == 0x00)
					return -1;
				if (string[i] == target)
					return i;
				return IndexOfHelper(string, target, i + 1);
			}
		
			constexpr int64_t IndexOf(const char* string, char target)
			{
				if (string == nullptr) return -1;
				return IndexOfHelper(string, target, 0);
			}
		
		
		
			constexpr int64_t LastIndexOfHelper(const char* string, char target, size_t i)
			{
				if (string[i] == target)
					return i;
				else if (i == 0)//We've reached the beginning of the string
					return -1;
				return LastIndexOfHelper(string, target, i - 1);
			}
		
			constexpr int64_t LastIndexOf(const char* string, const char* stringEnd, char target)
			{
				if (string == nullptr)
					return -1;
				return LastIndexOfHelper(string, target, stringEnd - string - 1);
			}
		}
	
		template<typename T, typename F>
		void Until(T*& string, F end)
		{
			while (*string && !end(*string))
				string++;
		}
	
		template<typename T>
		void Find(T*& string, const char* target)
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
		T* FindBegin(T*& string, const char* target)
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
		void NextLine(T*& string)
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
}
