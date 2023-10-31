#include "infra/File.h"

#include "test_utils/FsUtils.h"
#include <catch2/catch_test_macros.hpp>
#include <cstring>

struct TestStruct
{
    int x;
    char c;
};

TEST_CASE( "Sanity test", "[File, infra]" ) 
{
    // Step 1 - prepare enviroment - create empty file
    const std::string SANITY_TEST_FILE("/tmp/test_sanity.bin");

    agent_tests::FsUtils::DeleteFile(SANITY_TEST_FILE);
    REQUIRE(!agent_tests::FsUtils::IsFileExists(SANITY_TEST_FILE)); // Make sure that the file doesn't exists

    REQUIRE(agent_tests::FsUtils::CreateFile(SANITY_TEST_FILE));
    REQUIRE(agent_tests::FsUtils::IsFileExists(SANITY_TEST_FILE)); // Make sure that the file was created

    // Step 2 - Write binary data o file
    infra::File writeFile(SANITY_TEST_FILE);

    infra::File::FileStatus status = writeFile.Open();
    REQUIRE(infra::File::FileStatus::Success == status);

    TestStruct writeData;
    writeData.x = 2;
    writeData.c = 54;

    infra::File::BufferType writeBuffer(sizeof(TestStruct));
    std::memcpy(writeBuffer.data(), &writeData, sizeof(TestStruct));

    uint32_t bytesWritten = 0;
    status = writeFile.Write(writeBuffer, bytesWritten);
    REQUIRE(infra::File::FileStatus::Success == status);
    REQUIRE(bytesWritten == writeBuffer.size());
    writeFile.Close();

    // Step 3 - Read binary data from file and compare it to the writted data
    infra::File readFile(SANITY_TEST_FILE);
    status = readFile.Open();
    REQUIRE(infra::File::FileStatus::Success == status);

    TestStruct readData = {};
    infra::File::BufferType readBuffer(sizeof(TestStruct));

    uint32_t bytesRead = 0;
    status = readFile.Read(readBuffer, bytesRead);
    REQUIRE(infra::File::FileStatus::Success == status);
    REQUIRE(bytesRead == sizeof(TestStruct));

    std::memcpy(&readData, readBuffer.data(), sizeof(TestStruct));
    REQUIRE(readData.x == writeData.x);
    REQUIRE(readData.c == writeData.c);

    readFile.Close();

    // Step 4 - Delete temporary file
    agent_tests::FsUtils::DeleteFile(SANITY_TEST_FILE);
}