#pragma once

#include <string>

namespace agent_tests
{

class FsUtils
{
public:
    static bool CreateFile(const std::string fileName);
    static bool DeleteFile(const std::string fileName);
    static bool IsFileExists(const std::string fileName);
};

}