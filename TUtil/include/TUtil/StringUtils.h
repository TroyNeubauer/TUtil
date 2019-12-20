#pragma once

#include "TUtil/vendor/str/Str.h"

#include <cstring>
#include <cwchar>

namespace TUtil {

	//A nicer looking api for C-strings
	//Unless otherwise specified, all methods in this class will accecpt null pointers properly
	namespace StringUtils {

		//Two nullptrs are not equal to each other
		//The empty string ("") is always equal to itself
		bool Equal(const char* a, const char* b);
		bool Equal(const wchar_t* a, const wchar_t* b);
		//The empty string always contains itself
		bool Contains(const char* string, const char* target);


		inline size_t Length(const char* string, const char* stringEnd) { return stringEnd - string; }
		size_t Length(const char* string);

		//Returns the capacity needed to store a string. Since nullptr is not a valid string
		//This method return 0 for Capacity(nullptr)
		inline size_t Capacity(const char* string) { return Length(string) + 1; }
		
		inline void Copy(char* dest, size_t capacity, const char* src) { strncpy(dest, src, capacity); }

		template<typename T>
		inline T* begin(T* string) { return string; }
		
		template<typename T>
		inline T* end(T* string) { return string + Length(string); }


		//Both EndsWith(x, "") and StartsWith(x, "") return false for any value of x because
		//a string cannot contain another string that is empty
		bool StartsWith(const char* string, const char* target);
		bool EndsWith(const char* string, const char* stringEnd, const char* target, const char* targetEnd);
		inline bool EndsWith(const char* string, const char* target) { return EndsWith(string, string + Length(string), target, target + Length(target)); }

		int64_t IndexOf(const char* string, char target);

		int64_t LastIndexOf(const char* string, const char* stringEnd, char target);
		inline int64_t LastIndexOf(const char* string, char target) { return LastIndexOf(string, string + Length(string), target); }

		namespace Constexpr {

			template<typename T>
			constexpr bool Equal(const T* a, const T* b);
			constexpr bool Contains(const char* string, const char* target);

			constexpr size_t Length(const char* string, const char* stringEnd) { return stringEnd - string; }
			constexpr size_t Length(const char* string);
			constexpr inline size_t Capacity(const char* string) { return string ? (Length(string) + 1) : 0; }

			constexpr bool EndsWith(const char* string, const char* stringEnd, const char* target, const char* targetEnd);
			constexpr bool EndsWith(const char* string, const char* target) { return EndsWith(string, string + Length(string), target, target + Length(target)); }

			constexpr bool StartsWith(const char* string, const char* target);

			constexpr int64_t IndexOf(const char* string, char target);

			constexpr int64_t LastIndexOf(const char* string, const char* stringEnd, char target);
			constexpr inline int64_t LastIndexOf(const char* string, char target) { return LastIndexOf(string, string + Length(string), target); }

		}

		template<class none = void>
		constexpr bool ContainsAny(const char* string, const char* first) { return Contains(string, first); }

		template<class ... Types>
		bool ContainsAny(const char* string, const char* first, Types... args) { return Contains(string, first) || ContainsAny(string, args...); }

		//Advances the pointer until end returns true for the current character.
		template<typename T, typename F>//Template to allow for both const char* and char*
		void Until(T*& string, F end);

		template<typename T>
		void FirstOf(T*& string, char target) { Until(string, [target](char current) { return current == target; }); }

		template<typename T>
		void FirstOf(T*& string, char a, char b) { Until(string, [a, b](char current) { return current == a || current == b; }); }

		template<typename T>
		void FirstNotOf(T*& string, char target) { Until(string, [target](char current) { return current != target; }); }

		template<typename T>
		void FirstNotOf(T*& string, char a, char b) { Until(string, [a, b](char current) { return current != a || current != b; }); }


		//Advances string to the null termination character or to the n+1th character of the matched target
		//string will be modified to be an empty string if the target is not found
		//Find("Fast string finding is fun", "is") -> " fun"
		template<typename T>
		void Find(T*& string, const char* target);

		//Advances string to the null termination character or to the first character of the matched target
		//string will be modified to be an empty string if the target is not found
		//Find("Fast string finding is fun", "is") -> "is fun"
		template<typename T>
		T* FindBegin(T*& string, const char* target);

		template<typename T>
		void NextLine(T*& string);

	}
}


#include "StringUtils.inl"
