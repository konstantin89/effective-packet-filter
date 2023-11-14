#pragma once

#include <memory>
#include "Link.h"
#include "packet/PacketInterface.h"

namespace processing_chain
{

class ChainBuilderInterface
{
public:
    using LinkPtrType = std::unique_ptr<Link<PacketInterface>>;

public:
    virtual LinkPtrType BuildIpLink() = 0;
};

}