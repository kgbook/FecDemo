#include <unistd.h>
#include <map>
#include <cstring>
#include "UDPReceiver.h"
#include "NetworkTypedef.h"
#include "ALog.h"

#define LOG_TAG "UDPReceiver"

constexpr int BUFFER_SIZE = 1024 * 1024;

UDPReceiver::UDPReceiver(uint16_t port) : BaseReceiver("UDPReceiver"), port_(port) {
    socketLen_ = sizeof(mySocketAddr_);
    socket_ = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    memset(&mySocketAddr_, 0, sizeof(mySocketAddr_));
    mySocketAddr_.sin_family = AF_INET;
    mySocketAddr_.sin_port = htons(port_);
    mySocketAddr_.sin_addr.s_addr = htonl(INADDR_ANY);
    //bind socket to port
    if (bind(socket_, (struct sockaddr *) &mySocketAddr_, socketLen_) < 0) {
        LOGE(LOG_TAG, "bind failed!");
    }

    recvBuffer_ = static_cast<uint8_t *>(malloc(BUFFER_SIZE));
}

UDPReceiver::~UDPReceiver() {
    close(socket_);
}

bool UDPReceiver::threadLoop() {
    socklen_t addr_len = sizeof(peerSocketAddr_);
    int total_packets = 0;
    int total_received = 0;
    packets_.clear();

    while (true) {
        int received = recvfrom(socket_, packet_, MAX_UDP_PAYLOAD, 0, (struct sockaddr*)&peerSocketAddr_, &addr_len);
        if (received <= UDP_PACKET_HEADER_SIZE) {
            break;  // error or connection closed
        }
        auto* header = (UdpPacketHeader*)packet_;
        total_packets = header->total_packets;
        int data_len = received - UDP_PACKET_HEADER_SIZE;
        packets_[header->seq] = std::vector<char>(packet_ + UDP_PACKET_HEADER_SIZE, packet_ + UDP_PACKET_HEADER_SIZE + data_len);
        if (header->seq == total_packets - 1) {
            break;  // received all packets
        }
    }

    int count = 0;
    int last_packet_num = -1;
    for (const auto& p : packets_) {
        if (count >= total_packets) {
            break;  // Stop after total_packets elements
        }

        if (total_received + p.second.size() > BUFFER_SIZE) {
            break;  // Not enough space in buffer, stop receiving
        }
        LOGD(LOG_TAG, "packet num = %d", p.first);
        if (p.first != last_packet_num + 1) {
            LOGD(LOG_TAG, "lost packet, num = %d", last_packet_num + 1);
        }
        last_packet_num = p.first;

        memcpy(recvBuffer_ + total_received, p.second.data(), p.second.size());
        total_received += p.second.size();
        count++;
    }
    LOGD(LOG_TAG, "total_packets = %d, total_received_packets = %d", total_packets, count);
    output(recvBuffer_, total_received, nullptr);
    return true;
}
