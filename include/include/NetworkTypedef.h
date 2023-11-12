//
// Created by cory on 2023/11/12.
//

#ifndef FEC_NETWORKTYPEDEF_H
#define FEC_NETWORKTYPEDEF_H

#include <cstdint>
#include <cstddef>

#define NETWORK_MTU 1500
#define NETWORK_IPV4_HEADER_SIZE 20
#define NETWORK_UDP_HEADER_SIZE   8
#define NETWORK_MAX_UDP_PAYLOAD_SIZE  (NETWORK_MTU - NETWORK_IPV4_HEADER_SIZE - NETWORK_UDP_HEADER_SIZE)
#define NETWORK_UDP_PACKET_TAIL_SIZE  sizeof(UdpPacketTail)

#ifndef NETWORK_INIT_PACKET_SEQ_NO
#define NETWORK_INIT_PACKET_SEQ_NO 0
#endif

#ifndef NETWORK_INIT_FRAME_SEQ_NO
#define NETWORK_INIT_FRAME_SEQ_NO 0
#endif

struct UdpPacketTail {
    uint32_t packet_seq;
    uint32_t frame_seq;
    size_t total_packets;
};

enum class MediaMimeType {
    kVideo,
    kAudio,
};

#endif //FEC_NETWORKTYPEDEF_H
