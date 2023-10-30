#include "File.h"

#include <unistd.h>
#include <fcntl.h>

#include "spdlog/spdlog.h"

static constexpr int LINUX_SYSCALL_ERROR  = -1;

File::File(std::string filePath)
{
    mFilePath = filePath;
}

File::~File()
{

}

File::FileStatus File::Open()
{
    auto openResult = open(mFilePath.c_str(), 0);
    if (LINUX_SYSCALL_ERROR == openResult)
    {
        spdlog::error("Failed to open file {}", mFilePath);
        return File::FileStatus::Failed;
    }

    mFd = openResult;

    return File::FileStatus::Success;
}

File::FileStatus File::Close()
{
    auto closeResult = close(mFd);
    if (LINUX_SYSCALL_ERROR == closeResult)
    {
        return File::FileStatus::Failed;
    }

    return File::FileStatus::Success;
}

File::FileStatus File::Read(File::BufferRefType buffer, uint32_t &bytesRead) 
{

    return File::FileStatus::Success;

}

File::FileStatus File::Write(File::BufferRefType buffer, uint32_t &bytesWritten)
{

    return File::FileStatus::Success;
}