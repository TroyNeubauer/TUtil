#include "TUtil/Core.h"
#ifdef T_PLATFORM_EMSCRIPTEN

#include "TUtil/Math/Integer.h"

namespace TUtil {


	uint32_t Integer::GetMinBitPosition(uint64_t value) {
		for (int i = 0; i < 8 * sizeof(uint64_t); i++)
			if ((value >> i) & 0x01)
				return i;
		return -1;
	}

	uint64_t Integer::CountBits(uint64_t value) {
		int count = 0;
		for (int i = 0; i < 8 * sizeof(uint64_t); i++)
			if ((value >> i) & 0x01)
				count++;
		return count;

	}

	uint64_t Integer::RoundUp(uint64_t value, uint64_t multiple) {
		if (multiple == 0)
			return value;

		uint64_t remainder = value % multiple;
		if (remainder == 0)
			return value;

		return value + multiple - remainder;
	}
}

#endif