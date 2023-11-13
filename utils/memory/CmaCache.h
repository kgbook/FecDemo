#ifndef CMA_CACHE_H
#define CMA_CACHE_H

#include <cstdint>

class CmaCache {
public:
    explicit CmaCache(size_t capacity, size_t nodeCapacity);
    ~CmaCache();

    uint8_t *at(int32_t id);
    uint32_t length(int32_t id);
    bool copy(uint32_t index, uint8_t *data, uint32_t len);

private:
    size_t cache_capacity_;
    size_t node_nodeCapacity_;
    uint8_t *baseAddr_;
};

#endif //CMA_CACHE_H
