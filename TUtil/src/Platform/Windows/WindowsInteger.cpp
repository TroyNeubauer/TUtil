#include "TUtil/Core.h"

#ifdef T_PLATFORM_WINDOWS

#include "TUtil/Math/Integer.h"
#include "WindowsUtils.h"

#include <Windows.h>
#include <immintrin.h>

namespace TUtil {


	uint32_t Integer::GetMinBitPosition(std::uint64_t value) {
		DWORD result;
		BitScanForward64(&result, value);//TODO provide other implementation for non Windows platforms
		return result;
	}

	std::uint64_t Integer::CountBits(std::uint64_t value) {
		return _mm_popcnt_u64(value);//TODO other implementation for non X86
	}

	std::uint64_t Integer::RoundUp(std::uint64_t value, std::uint64_t multiple) {
		if (multiple == 0)
			return value;

		std::uint64_t remainder = value % multiple;
		if (remainder == 0)
			return value;

		return value + multiple - remainder;
	}
}

#endif