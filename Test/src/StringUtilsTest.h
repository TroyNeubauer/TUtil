#pragma once

#include "TUtil/StringUtils.h"
using namespace TUtil;

TEST_CASE("StringUtils::Equal", "[string]") {
	REQUIRE(StringUtils::Equal("", ""));
	REQUIRE_FALSE(StringUtils::Equal(nullptr, ""));
	REQUIRE_FALSE(StringUtils::Equal("", nullptr));
	REQUIRE_FALSE(StringUtils::Equal(nullptr, (const char*) nullptr));

	std::vector<char> vec1, vec2;
	vec1.reserve(1000000);
	vec2.reserve(1000000);
	std::fill(vec1.begin(), vec1.end(), 'a');
	std::fill(vec2.begin(), vec2.end(), 'a');

	REQUIRE(StringUtils::Equal("test", "test"));
	REQUIRE(StringUtils::Equal("I am equal", "I am equal"));
	REQUIRE(StringUtils::Equal(vec1.data(), vec2.data()));
	REQUIRE(StringUtils::Equal(vec2.data(), vec1.data()));

	REQUIRE_FALSE(StringUtils::Equal("test", nullptr));
	REQUIRE_FALSE(StringUtils::Equal(nullptr, "test"));
	REQUIRE_FALSE(StringUtils::Equal("tes\0t", "test"));

}

TEST_CASE("StringUtils::Equal (wchar_t)", "[string]") {
	REQUIRE(StringUtils::Equal(L"", L""));
	REQUIRE_FALSE(StringUtils::Equal(nullptr, L""));
	REQUIRE_FALSE(StringUtils::Equal(L"", nullptr));
	REQUIRE_FALSE(StringUtils::Equal(nullptr, (const wchar_t*) nullptr));

	std::vector<wchar_t> vec1, vec2;
	vec1.reserve(1000000);
	vec2.reserve(1000000);
	std::fill(vec1.begin(), vec1.end(), 'a');
	std::fill(vec2.begin(), vec2.end(), 'a');

	REQUIRE(StringUtils::Equal(L"test", L"test"));
	REQUIRE(StringUtils::Equal(L"I am equal", L"I am equal"));
	REQUIRE(StringUtils::Equal(vec1.data(), vec2.data()));
	REQUIRE(StringUtils::Equal(vec2.data(), vec1.data()));

	REQUIRE_FALSE(StringUtils::Equal(L"test", nullptr));
	REQUIRE_FALSE(StringUtils::Equal(nullptr, L"test"));
	REQUIRE_FALSE(StringUtils::Equal(L"tes\0t", L"test"));

}

TEST_CASE("StringUtils::Contains", "[string]") {
	REQUIRE_FALSE(StringUtils::Contains(nullptr, nullptr));
	REQUIRE_FALSE(StringUtils::Contains(nullptr, ""));
	REQUIRE_FALSE(StringUtils::Contains("", nullptr));

	REQUIRE(StringUtils::Contains("", ""));
	REQUIRE(StringUtils::Contains("a", ""));
	REQUIRE(StringUtils::Contains("test test", ""));

	REQUIRE_FALSE(StringUtils::Contains("", "a"));
	REQUIRE_FALSE(StringUtils::Contains("", "test test"));
	
	REQUIRE(StringUtils::Contains("test test", "test"));
	REQUIRE(StringUtils::Contains("test test!", "!"));

	REQUIRE(StringUtils::Contains("hat mat lat cat", "cat"));
	REQUIRE_FALSE(StringUtils::Contains("hat mat lat caat", "cat"));


}

template<typename F>
void EndsWithCase(F func)
{
	REQUIRE_FALSE(func(nullptr, nullptr));
	REQUIRE_FALSE(func(nullptr, ""));
	REQUIRE_FALSE(func("", nullptr));
	REQUIRE_FALSE(func("", ""));
	REQUIRE_FALSE(func("real text please fail", ""));

	REQUIRE_FALSE(func("this will fail", "!"));
	REQUIRE_FALSE(func("this will fail many times", "!"));

	REQUIRE(func("maybe will this one work?", "work?"));
	REQUIRE(func("maybe will this one work?", " work?"));
	REQUIRE(func("maybe will this one work?", "?"));
	REQUIRE(func("a", "a"));
}

TEST_CASE("StringUtils::EndsWith", "[string]") {
	EndsWithCase([](const char* a, const char* b) -> bool { return StringUtils::EndsWith(a, b); });
}

TEST_CASE("StringUtils::EndsWithNConstexpr", "[string]") {
	EndsWithCase([](const char* a, const char* b) -> bool { return StringUtils::EndsWithNConstexpr(a, b); });
}
