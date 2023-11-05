#include <iostream>
#include <thread>
#include <chrono>

#include "common/Version.h"
#include "spdlog/spdlog.h"

#include "infra/File.h"

void testCharDev()
{
    infra::File f("/dev/epf_packets_dev");

    auto status = f.Open();
    if (status != infra::FileInterface::FileStatus::Success)
    {
        spdlog::error("Failed to open /dev/epf_packets_dev \n");
        return;
    }

    while(1)
    {
        infra::FileInterface::BufferType buffer(10);
        uint32_t bytesRead = 0;
        f.Read(buffer, bytesRead);

        std::cout << "Bytes read: " << bytesRead  << std::endl;

        std::this_thread::sleep_for(std::chrono::seconds(5));
    }


}

int main()
{
    spdlog::set_level(spdlog::level::debug);

    spdlog::debug("Starting Effective Packet Filter version {}.{}.{} !", 
        EPF_VERSION_MAJOR, 
        EPF_VERSION_MINOR,
        EPF_VERSION_PATCH);

    testCharDev();

    return 0;
}