#pragma once

#include "TUtil/StringUtils.h"
using namespace TUtil;

template<typename F>
void EqualCase(F func)
{
	REQUIRE(func("", ""));
	REQUIRE_FALSE(func(nullptr, ""));
	REQUIRE_FALSE(func("", nullptr));
	REQUIRE_FALSE(func(nullptr, nullptr));

	std::vector<char> vec1, vec2;
	vec1.reserve(1000000);
	vec2.reserve(1000000);
	std::fill(vec1.begin(), vec1.end(), 'a');
	std::fill(vec2.begin(), vec2.end(), 'a');

	REQUIRE(func("test", "test"));
	REQUIRE(func("I am equal", "I am equal"));
	REQUIRE(func(vec1.data(), vec2.data()));
	REQUIRE(func(vec2.data(), vec1.data()));

	REQUIRE_FALSE(func("test", nullptr));
	REQUIRE_FALSE(func(nullptr, "test"));
	REQUIRE_FALSE(func("tes\0t", "test"));
}

TEST_CASE("StringUtils::Constexpr::Equal", "[string]") {
	EqualCase([](const char* a, const char* b) -> bool { return StringUtils::Constexpr::Equal(a, b); });
}

TEST_CASE("StringUtils::Equal", "[string]") {
	EqualCase([](const char* a, const char* b) -> bool { return StringUtils::Equal(a, b); });
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

TEST_CASE("StringUtils::Constexpr::EndsWith", "[string]") {
	EndsWithCase([](const char* a, const char* b) -> bool { return StringUtils::Constexpr::EndsWith(a, b); });
}

TEST_CASE("StringUtils::EndsWith", "[string]") {
	EndsWithCase([](const char* a, const char* b) -> bool { return StringUtils::EndsWith(a, b); });
}


