#include <cstring>
#include "CmaCache.h"
#include "ALog.h"

#define LOG_TAG "CmaCache"

CmaCache::CmaCache(uint32_t capacity, uint32_t nodeCapacity) :cache_capacity_(capacity), node_nodeCapacity_(nodeCapacity)
{
    baseAddr_ = new uint8_t [cache_capacity_ * node_nodeCapacity_];
}

CmaCache::~CmaCache()
{
    delete [] baseAddr_;
}

bool CmaCache::copy(uint32_t index, uint8_t* data, uint32_t len)
{
    uint8_t *nodeBuf = at(index);
    if (nullptr == nodeBuf) {
        ALOGE("get buf nullptr!");
        return false;
    }
    if (len > node_nodeCapacity_) {
        ALOGE("need len:%u, exceed nodeBufLen:%u!", len, node_nodeCapacity_);
        return false;
    }
    memcpy(nodeBuf, data, len);
    auto padding = (int32_t)(node_nodeCapacity_ - len);
    if (padding > 0) {
        bzero(nodeBuf + len, padding);
    }
    return true;
}

uint8_t* CmaCache::at(uint32_t id)
{
    if (id >= cache_capacity_) {
        ALOGE("invalid id:%d, exceed capacity:%d", id, cache_capacity_);
        return nullptr;
    }
    return baseAddr_ + id * node_nodeCapacity_;
}

uint32_t CmaCache::length(uint32_t id) const
{
    if (id >= cache_capacity_) {
        ALOGE("invalid id:%d, exceed capacity:%d", id, cache_capacity_);
        return 0;
    }
    return node_nodeCapacity_;
}
