#pragma once
#include "TUtil/Core.h"

#ifdef T_PLATFORM_WINDOWS

#include <Windows.h>
#include <Shlobj.h>

#undef CreateFile
#undef CreateDirectory
#undef DeleteFile

namespace TUtil {
	class WindowsUtils {
	public:
		static void GetLastErrorMessage(char* buf, unsigned int capacity);
	};
}

#endif
