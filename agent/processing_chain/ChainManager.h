#pragma once

#include <memory>

#include "Chain.h"
#include "ChainBuilderInterface.h"
#include "packet/PacketInterface.h"

namespace processing_chain
{
class ChainManager
{
public:
    using PacketType = std::shared_ptr<packet::PacketInterface>;
    using ChainPtrType = ChainBuilderInterface::ChainPtrType

    ChainManager();

private:

    ChainPtrType mChain;
};

}