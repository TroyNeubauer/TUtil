#include "TUtil/Core.h"

#ifdef T_PLATFORM_WINDOWS

#include "TUtil/Timer.h"
#include "TUtil/System.h"
#include "WindowsUtils.h"

namespace TUtil {
	Timer::Timer()
	{
		Start();
	}

	Timer& Timer::Start()
	{
		QueryPerformanceCounter(&start);
		return *this;
	}

	Timer& Timer::Stop()
	{
		QueryPerformanceCounter(&end);
		return *this;
	}

	std::uint64_t Timer::Nanos()
	{
		return (end.QuadPart - start.QuadPart) * 1000000000ULL / System::PerformanceCounterResulution();
	}

}

#endif
