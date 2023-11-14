#ifndef CMA_CACHE_H
#define CMA_CACHE_H

#include <cstdint>

class CmaCache {
public:
    CmaCache(uint32_t capacity, uint32_t nodeCapacity);
    ~CmaCache();

    uint8_t *at(uint32_t id);
    uint32_t length(uint32_t id) const;
    bool copy(uint32_t index, uint8_t *data, uint32_t len);

private:
    uint32_t cache_capacity_;
    uint32_t node_nodeCapacity_;
    uint8_t *baseAddr_;
};

#endif //CMA_CACHE_H
