#include "FsUtils.h"

#include <iostream>
#include <fstream> 
#include <cstdio>
#include <filesystem>

using namespace agent_tests;

bool FsUtils::CreateFile(const std::string fileName)
{
  std::ofstream f(fileName);
  bool isFileOpenend = f.good();
  f.close();

  return isFileOpenend;
}

bool FsUtils::DeleteFile(const std::string fileName)
{
    return (0 == std::filesystem::remove(fileName.c_str()));
}

bool FsUtils::IsFileExists(const std::string fileName)
{
    return std::filesystem::exists(fileName.c_str());
}