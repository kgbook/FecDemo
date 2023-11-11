//
// Created by cory on 2023/11/12.
//

#ifndef FEC_NETWORKTYPEDEF_H
#define FEC_NETWORKTYPEDEF_H

#include <cstdint>

#define MAX_UDP_PAYLOAD  1472  // MTU - IP header - UDP header
#define UDP_PACKET_HEADER_SIZE  8  // 4 bytes for packet number, 4 bytes for total packets

struct TcpPacketHeader {
    uint32_t payloadSize;
    uint32_t flags;
};

struct UdpPacketHeader {
    int seq;
    size_t total_packets;
};

enum class MediaMimeType {
    kVideo,
    kAudio,
};

#endif //FEC_NETWORKTYPEDEF_H
