#pragma once

#include <stdint.h>
#include "TUtil/Core.h"

namespace TUtil {
	
	namespace Integer {
		uint32_t GetMinBitPosition(uint64_t value);
		uint64_t CountBits(uint64_t value);
		uint64_t RoundUp(uint64_t value, uint64_t multiple);

		template<typename T>
		static T CeilDivide(T a, T b) {
			if (a == 0)
				return 0;
			return (a - 1) / b + 1;
		}

	}

}

