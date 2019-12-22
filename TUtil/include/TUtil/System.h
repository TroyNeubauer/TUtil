#pragma once

#include <stdint.h>
#include <stdlib.h>

#ifdef T_PLATFORM_WINDOWS
	#include <malloc.h>
#endif

namespace TUtil {

	class System
	{
	public:
		static void Init();

		//Returns the time since init was called in seconds or -1 if Init hasnt been called yet
		static float GetTime();

		//Uses stack allocation if avilable. Returns a buffer of the size: elements * sizeof(T)
		template<typename T>
		inline static T* FastNew(size_t elements);

		//Frees a pointer returned by FastAlloc
		inline static void FastDelete(void* ptr);

		//Stats for virtual memory
		static uint64_t GetTotalMachineVirtualMemory();//The total amount of virtual memory avilable to the system in bytes
		static uint64_t GetSystemVirtualMemoryUsage();//The amount of virtual memory being currently used on this machine in bytes
		static uint64_t GetProcessVirtualMemoryUsage();//The amount of virtual memory used by this process in bytes

		//stats for physical memory
		static uint64_t GetTotalMachinePhysicalMemory();//The amount of physical ram avilable to this machine in bytes
		static uint64_t GetSystemPhysicalMemoryUsage();//The amount of physical ram used by this machine in bytes
		static uint64_t GetProcessPhysicalMemoryUsage();//The amount of physical ram used by this process in bytes

		static float GetSystemCPUUsagePercent();//from [0,100] indicates the total CPU usage across all cores
		static float GetProcessCPUUsagePercent();//from [0,100] indicates the total CPU usage across all cores by this process

		static uint32_t GetProcessorCount();
		
		//Returns true if a key press within the command line connected to this process is detected
		static bool KBHit();
	
		static uint64_t PageSize();
		static uint64_t AllocationGranularity();
		static uint64_t PerformanceCounterResulution();
	};

	//Definitions to template functions
	template<typename T>
	inline T* System::FastNew(size_t elements)
	{
		const size_t bytes = sizeof(T) * elements;
#ifdef T_PLATFORM_WINDOWS
		return (T*) _malloca(bytes);
#else
		return (T*) malloc(bytes);
#endif
	}

	inline void System::FastDelete(void* ptr)
	{
#ifdef T_PLATFORM_WINDOWS
		_freea(ptr);
#else
		free(ptr);
#endif
	}

}
