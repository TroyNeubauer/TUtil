
#define CATCH_CONFIG_RUNNER
#include "catch2/catch.hpp"

int main(int argc, char* const argv[]) {
	int result = Catch::Session().run(argc, argv);

#if defined(_MSC_VER) && !defined(CI_BUILD)
	system("PAUSE");
#endif
	return result;
}


#include "FileSystemTest.h"
#include "BufferTest.h"
#include "StringUtilsTest.h"


