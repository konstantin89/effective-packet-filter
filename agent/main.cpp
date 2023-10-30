#include <iostream>

#include "common/Version.h"
#include "spdlog/spdlog.h"

int main()
{
    spdlog::info("Starting Effective Packet Filter version {}.{}.{} !", 
        EPF_VERSION_MAJOR, 
        EPF_VERSION_MINOR,
        EPF_VERSION_PATCH);

    spdlog::set_level(spdlog::level::debug);

    return 0;
}