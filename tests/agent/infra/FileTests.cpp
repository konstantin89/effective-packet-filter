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
    const std::string SANITY_TEST_FILE("/tmp/test_sanity.bin");

    agent_tests::FsUtils::DeleteFile(SANITY_TEST_FILE);
    REQUIRE(!agent_tests::FsUtils::IsFileExists(SANITY_TEST_FILE)); // Make sure that the file doesn't exists

    REQUIRE(agent_tests::FsUtils::CreateFile(SANITY_TEST_FILE));
    REQUIRE(agent_tests::FsUtils::IsFileExists(SANITY_TEST_FILE)); // Make sure that the file was created

    infra::File testFile(SANITY_TEST_FILE);

    infra::File::FileStatus status = testFile.Open();
    REQUIRE(infra::File::FileStatus::Success == status);

    TestStruct data;
    data.x = 2;
    data.c = 54;

    infra::File::BufferType dataBuffer(sizeof(TestStruct));
    std::memcpy(dataBuffer.data(), &data, sizeof(TestStruct));

    uint32_t bytesWritten = 0;
    status = testFile.Write(dataBuffer, bytesWritten);
    REQUIRE(infra::File::FileStatus::Success == status);
    REQUIRE(bytesWritten == dataBuffer.size());

    testFile.Close();

    agent_tests::FsUtils::DeleteFile(SANITY_TEST_FILE);

    //REQUIRE(agent_tests::FsUtils::DeleteFile(SANITY_TEST_FILE));

    // REQUIRE( v.size() == 5 );
    // REQUIRE( v.capacity() >= 5 );

    // SECTION( "resizing bigger changes size and capacity" ) {
    //     v.resize( 10 );

    //     REQUIRE( v.size() == 10 );
    //     REQUIRE( v.capacity() >= 10 );
    // }
}