#include "TUtil/Core.h"
#ifdef T_PLATFORM_EMSCRIPTEN

#include "TUtil/System.h"
#include "TUtil/Timer.h"

#include <ctime>

namespace TUtil {


	static Timer* startTimer = nullptr;

	void System::Init()
	{
		startTimer = new Timer();
	}


	float System::GetTime()
	{
		if (startTimer == nullptr)
			return -1.0f;
		else
			return static_cast<float>(startTimer->Stop().Seconds());
	}

	bool System::KBHit()
	{
		return false;
	}

	uint64_t System::GetTotalMachineVirtualMemory() {
		return 0;
	}

	uint64_t System::GetSystemVirtualMemoryUsage() {
		return 0;
	}

	uint64_t System::GetProcessVirtualMemoryUsage() {
		return 0;
	}

	//stats for physical memory
	uint64_t System::GetTotalMachinePhysicalMemory() {
		return 0;
	}

	uint64_t System::GetSystemPhysicalMemoryUsage() {
		return 0;
	}

	uint64_t System::GetProcessPhysicalMemoryUsage() {
		return 0;
	}

	float System::GetSystemCPUUsagePercent() {
		return 0.0f;
	}

	float System::GetProcessCPUUsagePercent() {
		return 0.0f;
	}

	uint32_t System::GetProcessorCount() {
		return 1;
	}

	uint64_t System::PageSize() {
		return 4096;
	}

	uint64_t System::AllocationGranularity() {
		return 0;
	}

	uint64_t System::PerformanceCounterResulution()
	{
		return CLOCKS_PER_SEC;
	}

}

#endif
