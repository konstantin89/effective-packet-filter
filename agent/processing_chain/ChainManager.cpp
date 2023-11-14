#include "ChainManager.h"

using processing_chain::ChainManager;

ChainManager::ChainManager()
{
    mChain = std::make_unique<ChainType>();
}
