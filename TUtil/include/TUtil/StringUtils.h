#pragma once

#include <TUtil/vendor/str/Str.h>

#include <cstring>
#include <cwchar>

namespace TUtil {

	//A nicer looking api for C-strings
	//Unless otherwise specified, all methods in this class will accecpt null pointers properly
	class StringUtils
	{
	public:
		//Two nullptrs are not equal to each other
		//The empty string ("") is always equal to itself
		static bool Equal(const char* a, const char* b);
		static bool Equal(const wchar_t* a, const wchar_t* b);
		//The empty string always contains itself
		static bool Contains(const char* string, const char* target);
		
		//Calling EndsWith(x, ""); will always return false for any value of x
		static constexpr bool EndsWith(const char* string, const char* target) { return EndsWith(string, string + Length(string), target, target + Length(target)); }
		static constexpr bool EndsWith(const char* string, const char* stringEnd, const char* target, const char* targetEnd);

		static inline bool EndsWithNConstexpr(const char* string, const char* target) {return EndsWithNConstexpr(string, string + Length(string), target, target + Length(target)); }
		static bool EndsWithNConstexpr(const char* string, const char* stringEnd, const char* target, const char* targetEnd);

		static constexpr bool StartsWith(const char* string, const char* target);

		static constexpr size_t Length(const char* string);
		static constexpr size_t Length(const char* string, const char* stringEnd) { return stringEnd - string; }
		static inline constexpr size_t Capacity(const char* string) { return Length(string) + 1; }

		//inline static void Copy(char* dest, const char* source) { std::strcpy(dest, source); }

		template<class none = void>
		constexpr static bool ContainsAny(const char* string, const char* first) { return Contains(string, first); }

		template<class ... Types>
		static bool ContainsAny(const char* string, const char* first, Types... args) { return Contains(string, first) || ContainsAny(string, args...); }

		//Advances the pointer until end returns true for the current character.
		template<typename T, typename F>//Template to allow for both const char* and char*
		static void Until(T*& string, F end);

		template<typename T>
		inline static void FirstOf(T*& string, char target) { Until(string, [target](char current) { return current == target; }); }
		template<typename T>
		inline static void FirstOf(T*& string, char a, char b) { Until(string, [a, b](char current) { return current == a || current == b; }); }

		template<typename T>
		inline static void FirstNotOf(T*& string, char target) { Until(string, [target](char current) { return current != target; }); }
		template<typename T>
		inline static void FirstNotOf(T*& string, char a, char b) { Until(string, [a, b](char current) { return current != a || current != b; }); }


		//Advances string to the null termination character or to the n+1th character of the matched target
		//string will be modified to be an empty string if the target is not found
		//Find("Fast string finding is fun", "is") -> " fun"
		template<typename T>
		static void Find(T*& string, const char* target);

		static constexpr size_t IndexOf(const char* string, char target);

		static constexpr inline size_t LastIndexOf(const char* string, char target) { return LastIndexOf(string, string + Length(string), target); }
		static constexpr size_t LastIndexOf(const char* string, const char* stringEnd, char target);

		//Advances string to the null termination character or to the first character of the matched target
		//string will be modified to be an empty string if the target is not found
		//Find("Fast string finding is fun", "is") -> "is fun"
		template<typename T>
		static T* FindBegin(T*& string, const char* target);

		template<typename T>
		static void NextLine(T*& string);

	};
}


#include "StringUtils.inl"