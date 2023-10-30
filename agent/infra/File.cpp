#include "File.h"
#include "utils/Logger.h"

#include <unistd.h>

File::File(std::string filePath)
{
    mFilePath = filePath;
}

File::FileStatus File::Open()
{
    auto openResult = open(mFilePath.c_str(), 0);
    if (openResult == -1)
    {

    }
}

File::FileStatus File::Close()
{

}

File::FileStatus File::Read(File::BufferRefType buffer, uint32_t &bytesRead) 
{

}

File::FileStatus File::Write(File::BufferRefType buffer, uint32_t &bytesWritten)
{

}