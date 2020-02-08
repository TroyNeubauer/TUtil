#include "TUtil/Core.h"
#ifdef T_PLATFORM_EMSCRIPTEN

#include "TUtil/Math/Integer.h"

namespace TUtil {


	uint32_t Integer::GetMinBitPosition(std::uint64_t value) {
		for (int i = 0; i < 8 * sizeof(std::uint64_t); i++)
			if ((value >> i) & 0x01)
				return i;
		return -1;
	}

	std::uint64_t Integer::CountBits(std::uint64_t value) {
		int count = 0;
		for (int i = 0; i < 8 * sizeof(std::uint64_t); i++)
			if ((value >> i) & 0x01)
				count++;
		return count;

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