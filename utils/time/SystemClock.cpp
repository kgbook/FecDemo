#include <ctime>
#include "SystemClock.h"

uint64_t SystemClock::uptimeMillisecond() {
    struct timespec ts{};
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (uint64_t(ts.tv_sec) * 1000000000LL + ts.tv_nsec) / 1000000;
}

uint64_t SystemClock::realTimeMillisecond() {
    struct timespec ts{};
    clock_gettime(CLOCK_REALTIME, &ts);
    return (uint64_t(ts.tv_sec) * 1000000000LL + ts.tv_nsec) / 1000000;
}
