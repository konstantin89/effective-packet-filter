#include <iostream>
#include <thread>
#include <chrono>
#include "spdlog/spdlog.h"
#include <arpa/inet.h> //ntohs

#include "infra/File.h"
#include "common/Version.h"
#include "common/PacketMessage.h"


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
        infra::FileInterface::BufferType buffer(sizeof(struct PacketMessage));
        uint32_t bytesRead = 0;
        f.Read(buffer, bytesRead);

        std::cout << "Bytes read: " << bytesRead  << std::endl;

        if (bytesRead > 0)
        {
            struct PacketMessage *packetMessage = (struct PacketMessage *)buffer.data();
            std::cout << "IP version " << (int)(packetMessage->ipHeader.ipv4Header.version)  << std::endl;

            struct udphdr *udp_header = (struct udphdr *)&packetMessage->transportHeader.udpHeader;
            std::cout << "Dest Port " << ntohs(udp_header->dest)  << std::endl;

        }

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