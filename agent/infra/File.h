#pragma once

#include "FileInterface.h"
#include <string>

namespace infra
{

class File : public FileInterface
{
public:

    File(std::string filePath);

    ~File();

    FileStatus Open() override;
    FileStatus Close() override;
    FileStatus Read(BufferRefType buffer, uint32_t &bytesRead) override;
    FileStatus Write(BufferRefType buffer, uint32_t &bytesWritten) override;

private:
    std::string mFilePath;
    int mFd;
};

}
