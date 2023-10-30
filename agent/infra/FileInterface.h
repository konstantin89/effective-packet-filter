#ifndef __FILE_INTERFACE_H
#define __FILE_INTERFACE_H

#include <string>
#include <cstdint>

#include <vector>

class FileInterface
{
public:

    enum class FileStatus
    {
        Success,
        Failed
    };

    using BufferType = std::vector<uint8_t>;
    using BufferRefType = BufferType&;

    virtual FileStatus Open() = 0;
    virtual FileStatus Close() = 0;
    virtual FileStatus Read(BufferRefType buffer, uint32_t &bytesRead) = 0;
    virtual FileStatus Write(BufferRefType buffer, uint32_t &bytesWritten) = 0;

    FileInterface() {};

    // Virtual destructor
    virtual ~FileInterface() {};

    // Delete copy constructor
    FileInterface(const FileInterface&) = delete;

    // Delete assign copy operator
    FileInterface& operator=(const FileInterface&) = delete;
};

#endif