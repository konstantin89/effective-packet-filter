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
    if (mFd != 0)
    {
        Close();
    }
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
        spdlog::error("Failed to close file {}", mFilePath);
        return File::FileStatus::Failed;
    }

    mFd = 0;

    return File::FileStatus::Success;
}

File::FileStatus File::Read(File::BufferRefType buffer, uint32_t &bytesRead) 
{
    auto readResult = read(mFd, buffer.data(), buffer.size());
    if (LINUX_SYSCALL_ERROR == readResult)
    {
        spdlog::error("Failed to read file {}", mFilePath);
        return File::FileStatus::Failed;
    }

    bytesRead = readResult;

    return File::FileStatus::Success;
}

File::FileStatus File::Write(File::BufferRefType buffer, uint32_t &bytesWritten)
{
    auto writeResult = write(mFd, buffer.data(), buffer.size());
    if (LINUX_SYSCALL_ERROR == writeResult)
    {
        spdlog::error("Failed to write file {}", mFilePath);
        return File::FileStatus::Failed;
    }

    bytesWritten = writeResult;

    return File::FileStatus::Success;
}