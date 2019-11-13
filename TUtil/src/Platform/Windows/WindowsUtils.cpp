#include "TUtil/Core.h"

#ifdef T_PLATFORM_WINDOWS

#include "WindowsUtils.h"

namespace TUtil {
	void WindowsUtils::GetLastErrorMessage(char* buf, unsigned int capacity) {
		DWORD error = GetLastError();
		DWORD result = FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL, error, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			buf, capacity, NULL);
		if (!result) {
			fprintf(stderr, "Failed to get information log from error code: %d, the error from FormatMessageA was %d", error, GetLastError());
			if (capacity)
				buf[0] = 0x00;//Make sure to terminate the string
			else
				T_ASSERT(false, "Capacity is 0!");
		}
	}
}

#endif