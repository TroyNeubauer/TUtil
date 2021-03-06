#pragma once

#include "Core.h"
#include <stdint.h>
#include <inttypes.h>

#include "Core.h"

#ifdef T_PLATFORM_WINDOWS
	#include <Windows.h>
	typedef LARGE_INTEGER TimeType;
#elif defined(T_PLATFORM_UNIX) || defined(T_PLATFORM_EMSCRIPTEN)
	#include <ctime>
	typedef timespec TimeType;
#else
	#error Cannot find time implementation
#endif

namespace TUtil {
	class Timer {
	public:
		//Constructing a timer starts it
		Timer();

		Timer& Start();
		Timer& Stop();
		std::uint64_t Nanos();
		
		double Seconds()
		{
			return Nanos() / 1000000000.0;
		}
		double Millis()
		{
			return Nanos() / 1000000.0;
		}
		uint64_t Micros()
		{
			return Nanos() / 1000;
		}

		static inline void FormatNanos(char* buf, uint64_t bufSize, uint64_t nanos) { FormatNanos(buf, bufSize, "", nanos); }

		static inline void FormatNanos(char* buf, uint64_t bufSize, const char* message, uint64_t nanos)
		{
			if (nanos < 1000)
				snprintf(buf, bufSize, "%s %" PRIu64 " nanoseconds", message,			 nanos);
			else if (nanos < (1000ULL * 1000ULL))
				snprintf(buf, bufSize, "%s %.2f microseconds", message,	(double) nanos / 1000.0);
			else if (nanos < (1000ULL * 1000ULL * 1000ULL))
				snprintf(buf, bufSize, "%s %.2f milliseconds", message,	(double) nanos / 1000000.0);
			else
				snprintf(buf, bufSize, "%s %.3f seconds", message,		(double) nanos / 1000000000.0);
		}
		
		inline Timer& Print(const char* message)
		{
			char buf[512];
			FormatNanos(buf, sizeof(buf), message, Nanos());
			printf("%s\n", buf);
			return *this;
		}
		
	private:
		TimeType start, end;
	};

	//A timer that prints the time since construnction when the destructor is called.
	class ScopedTimer
	{
	public:
		inline ScopedTimer(const char* message = "[Scoped Timer] Time: ") 
			: m_Message(message) {}
		
		inline ~ScopedTimer()
		{
			timer.Stop().Print(m_Message);
		}

	private:
		const char* m_Message;
		Timer timer;
	};

}
