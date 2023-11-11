#ifndef SYSTEM_CLOCK_H
#define SYSTEM_CLOCK_H

#include <cstdint>

class SystemClock {
public:
    static uint64_t uptimeMillisecond();
    static uint64_t realTimeMillisecond();
};

#endif //SYSTEM_CLOCK_H
