#include <utility>
#include "ElapsedTime.h"
#include "SystemClock.h"
#include "ALog.h"

#define LOG_TAG "ElapsedTime"

ElapsedTime::ElapsedTime(std::string tag) : tag_(std::move(tag)) {
    start_ = SystemClock::uptimeMillisecond();
}

ElapsedTime::~ElapsedTime() {
    LOGD(LOG_TAG, "%s spend time = %lld", tag_.c_str(), duration());
}

uint64_t ElapsedTime::duration() const {
    auto end = SystemClock::uptimeMillisecond();
    return end - start_;
}