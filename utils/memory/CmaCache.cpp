#include <cstring>
#include "CmaCache.h"
#include "ALog.h"

#define LOG_TAG "CmaCache"

CmaCache::CmaCache(size_t capacity, size_t nodeCapacity) :cache_capacity_(capacity), node_nodeCapacity_(nodeCapacity)
{
    baseAddr_ = new uint8_t [cache_capacity_ * node_nodeCapacity_];
}

CmaCache::~CmaCache()
{
    delete [] baseAddr_;
}

bool CmaCache::copy(uint32_t index, uint8_t* data, uint32_t len)
{
    uint8_t *nodeBuf;
    uint32_t nodeCapacity;
    if (len > nodeCapacity) {
        ALOGE("need len:%u, exceed nodeBufLen:%u!", len, nodeCapacity);
        return false;
    }
    memcpy(nodeBuf, data, len);
    int32_t padding = nodeCapacity - len;
    if (padding > 0) {
        bzero(nodeBuf + len, padding);
    }
    return true;
}

uint8_t* CmaCache::at(int32_t id)
{
    if (id >= cache_capacity_) {
        ALOGE("invalid id:%d, exceed capacity:%d", id, cache_capacity_);
        return nullptr;
    }
    return baseAddr_ + id * node_nodeCapacity_;
}

uint32_t CmaCache::length(int32_t id)
{
    if (id >= cache_capacity_) {
        ALOGE("invalid id:%d, exceed capacity:%d", id, cache_capacity_);
        return 0;
    }
    return node_nodeCapacity_;
}
