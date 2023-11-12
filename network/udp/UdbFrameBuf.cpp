//
// Created by cory on 2023/11/1.
//

#include <cstring>
#include "ALog.h"
#include "NetworkTypedef.h"
#include "UdpFrameBuf.h"

#define LOG_TAG "UdpFrameBuf"

UdpFrameBuf::UdpFrameBuf(size_t capacity) : offset_(0), capacity_(capacity), collected_(0), total_(0), seq_(NETWORK_INIT_PACKET_SEQ_NO) {
    addr_ = new char [capacity_];
}

UdpFrameBuf::~UdpFrameBuf() {
    delete [] addr_;
    offset_ = 0;
    capacity_ = 0;
}

void UdpFrameBuf::copy(uint32_t seq, uint32_t total, const void *data, size_t len) {
    if (seq != seq_) {
        clear();
        seq_ = seq;
        total_ = total;
    }
    memcpy(buf(), data, len);
    setOffset(len);
}

void UdpFrameBuf::print(const char *tag) {
    LOGD("[%s]seq:%u, consume:%d, collectd:%d, total packet:%d, total byte:%d", tag, seq(), consume(), collected_, total_, size());
}
