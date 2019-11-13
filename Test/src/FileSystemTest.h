#include <stdio.h>
#include <iostream>
#include <fstream>

#include "TUtil/FileSystem.h"

TEST_CASE("FileSystem::CreateDirectory", "[filesystem]") {
	REQUIRE(TUtil::FileSystem::CreateDirectory("./test/"));
}

TEST_CASE("FileSystem::CreateDirectories", "[filesystem]") {

	REQUIRE(TUtil::FileSystem::CreateDirectories("test/test2/test3/"));
}

TEST_CASE("FileSystem::Delete", "[filesystem]") {
	char fileName[] = "./test/test.dat";
	FILE* f = fopen(fileName, "wb");
	REQUIRE(f != nullptr);
	REQUIRE(fwrite(fileName, 1, sizeof(fileName), f) == sizeof(fileName));
	REQUIRE(fclose(f) == 0);
	TUtil::FileSystem::Delete(fileName);
	REQUIRE(fopen(fileName, "rb") == nullptr);
	
}

TEST_CASE("FileSystem::FileLength", "[filesystem]") {
	char fileName[] = "./test/length.dat";
	FILE* f = fopen(fileName, "wb");
	REQUIRE(f != nullptr);
	REQUIRE(fwrite(fileName, 1, sizeof(fileName), f) == sizeof(fileName));
	REQUIRE(fclose(f) == 0);
	REQUIRE(TUtil::FileSystem::FileSize(fileName) == sizeof(fileName));
	TUtil::FileSystem::Delete(fileName);
}

TEST_CASE("FileSystem::TruncateFile", "[filesystem]") {
	char fileName[] = "./test/length.dat";
	FILE* f = fopen(fileName, "wb");
	REQUIRE(f != nullptr);
	REQUIRE(fwrite(fileName, 1, sizeof(fileName), f) == sizeof(fileName));
	REQUIRE(fclose(f) == 0);
	REQUIRE(TUtil::FileSystem::TruncateFile(fileName));
	REQUIRE(TUtil::FileSystem::FileSize(fileName) == 0);
	TUtil::FileSystem::Delete(fileName);
	
	//test TruncateFile creating non existing files
	REQUIRE(TUtil::FileSystem::TruncateFile("./test/file_that_will_never_exist.dat"));
	REQUIRE(TUtil::FileSystem::Delete("./test/file_that_will_never_exist.dat"));
}

TEST_CASE("FileSystem::CreateFile", "[filesystem]") {
	char fileName[] = "./test/test.dat";
	TUtil::FileSystem::Delete(fileName);
	REQUIRE(TUtil::FileSystem::CreateFile(fileName));
	FILE* f = fopen(fileName, "wb");
	REQUIRE(f != nullptr);
	fclose(f);
}


TEST_CASE("FileSystem::MapFile", "[filesystem]") {
	SECTION("Memory map test file") {
		char fileName[] = "./test/test.dat";
		FILE* f = fopen(fileName, "wb");
		REQUIRE(f != nullptr);
		REQUIRE(fwrite(fileName, 1, sizeof(fileName), f) == sizeof(fileName));
		REQUIRE(fclose(f) == 0);
		TUtil::FileError error;

		uint64_t length;
		void* data = TUtil::FileSystem::MapFile(fileName, TUtil::FileOpenOptions::READ, length, &error, 0, TUtil::FileSystem::ENTIRE_FILE);
		REQUIRE(data != nullptr);
		REQUIRE(TUtil::StringUtils::Equal(fileName, (const char*)data));
		TUtil::FileSystem::UnmapFile(data);
	}
}
