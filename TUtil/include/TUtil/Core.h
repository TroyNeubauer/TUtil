#pragma once

//Common includes
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

//========== COMPILER detect ==========
#if defined(_MSC_VER)
	#define T_COMPILER_MSVC

#elif defined(__GNUC__)
	#define T_COMPILER_GCC
#elif defined(__clang__)
	#define T_COMPILER_CLANG
#elif defined(__EMSCRIPTEN__ )
	#define T_COMPILER_EMSCRIPTEN
#endif


//========== PLATFORM detect ==========
#if defined(__ANDROID__)
	#define T_PLATFORM_ANDROID
	#define T_PLATFORM_UNIX
	#define T_PLATFORM_LINUX

#elif defined(_WIN32) || defined(WIN32) || defined(__CYGWIN__) || defined(__MINGW32__) || defined(__BORLANDC__)
	#define T_PLATFORM_WINDOWS

#elif defined(__APPLE__) || defined(__MACH__)
	#include <TargetConditionals.h>
	/* TARGET_OS_MAC exists on all the platforms
	 * so we must check all of them (in this order)
	 * to ensure that we're running on MAC
	 * and not some other Apple platform */
	#define T_PLATFORM_APPLE
	#define T_PLATFORM_UNIX
	#if TARGET_IPHONE_SIMULATOR == 1
		#error "IOS simulator is not supported!"
	#elif TARGET_OS_IPHONE == 1
		#define HZ_PLATFORM_IOS
		#error "IOS is not supported!"
	#elif TARGET_OS_MAC == 1
		#define HZ_PLATFORM_MACOS
		#error "MacOS is not supported!"
	#else
		#error "Unknown Apple platform!"
	#endif


#elif defined(__linux__)//Defined after android to target desktop linux
	#define T_PLATFORM_UNIX
	#define T_PLATFORM_LINUX
#else
	#error Unknown platform
#endif


#ifdef DEBUG
	// debug break
	#if defined(T_PLATFORM_WINDOWS)
		#define T_DEBUGBREAK() __debugbreak()
	#elif defined(T_PLATFORM_LINUX)
		#include <signal.h>
		#define T_DEBUGBREAK() raise(SIGTRAP)

	#else
		#error No debug break!
	#endif

		#define T_ERROR(format, ...) fprintf(stderr, format, __VA_ARGS__); fprintf(stderr, "\n"); T_DEBUGBREAK();

		#define T_ASSERT(value, message, ...) if (!value) { fprintf(stderr, "Assertion failed!: "); T_ERROR(message, __VA_ARGS__); }

#else
	#define T_ERROR(format, ...)
	#define T_ASSERT(value, message, ...)

#endif
