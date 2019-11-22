
#define CATCH_CONFIG_RUNNER
#include <catch2/catch.hpp>

int main(int argc, char* const argv[]) {
	int result = Catch::Session().run(argc, argv);

#ifdef _MSC_VER
	system("PAUSE");
#endif
}


#include "FileSystemTest.h"
#include "BufferTest.h"
#include "StringUtilsTest.h"


