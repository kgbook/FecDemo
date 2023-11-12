//
// Created by cory on 2023/11/1.
//

#ifndef FEC_FRAME_BUF_H
#define FEC_FRAME_BUF_H

#include <cstdint>

class UdpFrameBuf {
public:
    explicit UdpFrameBuf(size_t capacity);
    virtual ~UdpFrameBuf();

    void copy(uint32_t seq, uint32_t total, const void *data, size_t len);
    inline bool consume() const {return ((collected_ > 0) && (collected_ >= total_)); }
    void print(const char *tag);

    inline char *base() { return addr_; }
    inline size_t size() const { return offset_; }
    inline size_t capacity() const { return capacity_; }
    inline void clear() { offset_ = 0; seq_ = 0; total_ =0; collected_ = 0; }
    inline char *buf() { return addr_ + offset_; }
    inline size_t buflen() const { return capacity_ - offset_;}
    inline void setOffset(size_t offset) { collected_++; offset_ += offset;}
    inline uint32_t &packetNum() { return total_;}
    inline uint32_t &seq() { return  seq_;}
    inline uint32_t &collected() {return collected_;}

private:
    char *addr_;
    size_t capacity_;
    size_t offset_;
    uint32_t collected_;
    uint32_t total_;
    uint32_t seq_;
};

#endif //FEC_FRAME_BUF_H
