#include "TUtil/Core.h"

#ifdef T_PLATFORM_WINDOWS

#include "TUtil/System.h"
#include "TUtil/Timer.h"
#include "WindowsUtils.h"

#include <Windows.h>
#include <stdio.h>
#include <intrin.h>
#include <psapi.h>
#include <TCHAR.h>
#include <pdh.h>
#include <conio.h>

namespace TUtil {

	static SYSTEM_INFO info = {};
	static uint64_t timerResulution = 0;

	//total CPU usage
	static PDH_HQUERY cpuQuery;
	static PDH_HCOUNTER cpuTotal;

	//Process CPU time
	static ULARGE_INTEGER lastCPU, lastSysCPU, lastUserCPU;
	static int numProcessors;
	static HANDLE self;
	static Timer* startTimer = nullptr;


	void System::Init()
	{
		startTimer = new Timer();
		PdhOpenQuery(NULL, NULL, &cpuQuery);
		PdhAddEnglishCounter(cpuQuery, L"\\Processor(_Total)\\% Processor Time", NULL, &cpuTotal);
		PdhCollectQueryData(cpuQuery);

		SYSTEM_INFO sysInfo;
		FILETIME ftime, fsys, fuser;

		GetSystemInfo(&sysInfo);
		numProcessors = sysInfo.dwNumberOfProcessors;

		GetSystemTimeAsFileTime(&ftime);
		memcpy(&lastCPU, &ftime, sizeof(FILETIME));

		self = GetCurrentProcess();
		GetProcessTimes(self, &ftime, &ftime, &fsys, &fuser);
		memcpy(&lastSysCPU, &fsys, sizeof(FILETIME));
		memcpy(&lastUserCPU, &fuser, sizeof(FILETIME));
	}

	float System::GetTime()
	{
		if (startTimer == nullptr)
			return -1.0f;
		else
			return static_cast<float>(startTimer->Stop().Seconds());
	}


	bool System::KBHit() {
		return _kbhit();
	}

	void GetMemInfo(MEMORYSTATUSEX* memInfo)
	{
		memInfo->dwLength = sizeof(MEMORYSTATUSEX);
		GlobalMemoryStatusEx(memInfo);
	}

	uint64_t System::GetTotalMachineVirtualMemory() {
		MEMORYSTATUSEX memInfo;
		GetMemInfo(&memInfo);

		DWORDLONG totalVirtualMem = memInfo.ullTotalPageFile;
		return totalVirtualMem;
	}

	uint64_t System::GetSystemVirtualMemoryUsage() {
		MEMORYSTATUSEX memInfo;
		GetMemInfo(&memInfo);

		DWORDLONG virtualMemUsed = memInfo.ullTotalPageFile - memInfo.ullAvailPageFile;
		return virtualMemUsed;
	}

	uint64_t System::GetProcessVirtualMemoryUsage() {
		PROCESS_MEMORY_COUNTERS_EX pmc;
		GetProcessMemoryInfo(GetCurrentProcess(), (PROCESS_MEMORY_COUNTERS*)&pmc, sizeof(pmc));

		SIZE_T virtualMemUsedByMe = pmc.PrivateUsage;
		return virtualMemUsedByMe;
	}

	//stats for physical memory
	uint64_t System::GetTotalMachinePhysicalMemory() {
		MEMORYSTATUSEX memInfo;
		GetMemInfo(&memInfo);

		DWORDLONG totalPhysMem = memInfo.ullTotalPhys;
		return totalPhysMem;
	}

	uint64_t System::GetSystemPhysicalMemoryUsage() {
		MEMORYSTATUSEX memInfo;
		GetMemInfo(&memInfo);

		DWORDLONG physMemUsed = memInfo.ullTotalPhys - memInfo.ullAvailPhys;
		return physMemUsed;
	}

	uint64_t System::GetProcessPhysicalMemoryUsage() {
		PROCESS_MEMORY_COUNTERS_EX pmc;
		GetProcessMemoryInfo(GetCurrentProcess(), (PROCESS_MEMORY_COUNTERS*)&pmc, sizeof(pmc));

		SIZE_T physMemUsedByMe = pmc.WorkingSetSize;
		return physMemUsedByMe;
	}

	float System::GetSystemCPUUsagePercent() {
		PDH_FMT_COUNTERVALUE counterVal;

		PdhCollectQueryData(cpuQuery);
		PdhGetFormattedCounterValue(cpuTotal, PDH_FMT_DOUBLE, NULL, &counterVal);
		return (float)counterVal.doubleValue;
	}

	float System::GetProcessCPUUsagePercent() {
		FILETIME ftime, fsys, fuser;
		ULARGE_INTEGER now, sys, user;
		double percent;

		GetSystemTimeAsFileTime(&ftime);
		memcpy(&now, &ftime, sizeof(FILETIME));

		GetProcessTimes(self, &ftime, &ftime, &fsys, &fuser);
		memcpy(&sys, &fsys, sizeof(FILETIME));
		memcpy(&user, &fuser, sizeof(FILETIME));
		percent = (double) ((sys.QuadPart - lastSysCPU.QuadPart) + (user.QuadPart - lastUserCPU.QuadPart));
		percent /= (now.QuadPart - lastCPU.QuadPart);
		percent /= numProcessors;
		lastCPU = now;
		lastUserCPU = user;
		lastSysCPU = sys;

		return (float)(percent * 100.0);
	}

	uint32_t System::GetProcessorCount() {
		return numProcessors;
	}


	static inline void GetSystemInfo() {
		if (info.dwPageSize == 0) {
			GetSystemInfo(&info);
		}
	}

	uint64_t System::PageSize() {
		GetSystemInfo();
		return info.dwPageSize;
	}

	uint64_t System::AllocationGranularity() {
		GetSystemInfo();
		return info.dwAllocationGranularity;
	}

	uint64_t System::PerformanceCounterResulution()
	{
		if (timerResulution == 0) {
			LARGE_INTEGER result;
			if (QueryPerformanceFrequency(&result) == 0) {
				char buf[1024];
				WindowsUtils::GetLastErrorMessage(buf, sizeof(buf));
				fprintf(stderr, "QueryPerformanceFrequency Returned 0! Error: %s", buf);
			}
			timerResulution = result.QuadPart;
		}
		return timerResulution;
	}

}

#endif
