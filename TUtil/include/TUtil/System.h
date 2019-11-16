#pragma once

#include <stdint.h>
#include <malloc.h>

namespace TUtil {

	class System
	{
	public:
		static void Init();

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

		static uint64_t LogFloor(uint64_t value);
		//Computes the log base 2 for the value and rounds up. Ex 2->1, 4->2, 5->3, 8->3
		//We need to make powers of two produce the largest value for a given input.
		//because LogFloor(5)->2, LogFloor(4) is still 2 and with a +1 we get the right anwser
		//LogFloor(4)->2, Log2Celi(3)->1+1->2
		static inline uint64_t Log2Celi(uint64_t value) { return LogFloor(value - 1) + 1; }
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
