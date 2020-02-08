#include "TUtil/Core.h"
#ifdef T_PLATFORM_UNIX

#include "TUtil/Timer.h"
#include "TUtil/System.h"

namespace TUtil {
	Timer::Timer()
	{
		Start();
	}

	Timer& Timer::Start()
	{
		clock_gettime(CLOCK_REALTIME, &start);
		return *this;
	}

	Timer& Timer::Stop()
	{
		clock_gettime(CLOCK_REALTIME, &end);
		return *this;
	}

	std::uint64_t Timer::Nanos()
	{
		std::uint64_t seconds = end.tv_sec - start.tv_sec;
		return 1000000000ULL * seconds + end.tv_nsec - start.tv_nsec;
	}

}

#endif
