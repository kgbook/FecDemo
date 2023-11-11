#include <unistd.h>
#include <thread>
#include <cstring>
#include "UDPSender.h"
#include "NetworkTypedef.h"
#include "ALog.h"

#define LOG_TAG "UDPSender"

UDPSender::UDPSender(std::string clientIP, uint16_t port) : BaseModule("UDPSender"),
                                             clientIP_(clientIP), port_(port) {
    LOGD(LOG_TAG, "peer ip : %s", clientIP_.c_str());

    //my setting
    socketLen_ = sizeof (mySocketAddr_);
    socket_ = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    memset(&mySocketAddr_, 0, sizeof(mySocketAddr_));
    mySocketAddr_.sin_family = AF_INET;
    mySocketAddr_.sin_port = htons(port_);
    mySocketAddr_.sin_addr.s_addr = htonl(INADDR_ANY);
    //bind socket to port
    if (bind(socket_, (struct sockaddr *) &mySocketAddr_, socketLen_) < 0) {
        LOGE(LOG_TAG, "bind socket error:%s", strerror(errno));
        return;
    }

    //peer setting
    peerSocketAddr_.sin_family = AF_INET;
    peerSocketAddr_.sin_port = htons(10086);
    peerSocketAddr_.sin_addr.s_addr = inet_addr(clientIP.c_str());
}

UDPSender::~UDPSender() {
    close(socket_);
}

void UDPSender::input(uint8_t *data, size_t len, uint8_t *privateData) {
    size_t total_packets = (len + MAX_UDP_PAYLOAD - UDP_PACKET_HEADER_SIZE - 1) / (MAX_UDP_PAYLOAD - UDP_PACKET_HEADER_SIZE);
    for (int i = 0; i < total_packets; i++) {
        int packet_len = len > (MAX_UDP_PAYLOAD - UDP_PACKET_HEADER_SIZE) ? (MAX_UDP_PAYLOAD - UDP_PACKET_HEADER_SIZE) : len;
        auto* header = (UdpPacketHeader*)packet_;
        header->seq = i;
        header->total_packets = total_packets;
        memcpy(packet_ + UDP_PACKET_HEADER_SIZE, data + i * (MAX_UDP_PAYLOAD - UDP_PACKET_HEADER_SIZE), packet_len);
        auto sent = wait_send(packet_, packet_len + UDP_PACKET_HEADER_SIZE);
        LOGD(LOG_TAG, "wait_send return %d, total_packets = %d", sent, total_packets);
        len -= packet_len;
    }
}

int UDPSender::wait_send(const char *data, int len) {
    ssize_t bytesSend = 0;
    while (1) {
        fd_set writefds;
        FD_ZERO(&writefds);
        FD_SET(socket_, &writefds);

        struct timeval timeout{};
        timeout.tv_sec = 5;
        timeout.tv_usec = 0;

        int ready = select(socket_ + 1, NULL, &writefds, NULL, &timeout);
        if (ready < 0) {
            LOGD(LOG_TAG, "select error.");
            return -1;
        }

        if (FD_ISSET(socket_, &writefds)) {
            bytesSend = sendto(socket_, data, len, 0, (struct sockaddr*)&peerSocketAddr_, sizeof(struct sockaddr_in));
            if (bytesSend < 0) {
                if (errno == EAGAIN || errno == EWOULDBLOCK || errno == EINTR) {
                    LOGD(LOG_TAG, "sendto try again...");
                    std::this_thread::sleep_for(std::chrono::milliseconds(5));
                    continue;
                } else {
                    LOGD(LOG_TAG, "sendto error = %d", errno);
                    return -2;
                }
            } else {
                return (int)bytesSend;
            }
        } else {
            LOGD(LOG_TAG, "Socket is not ready for writing, waiting...\n");
            return -3;
        }
    }
}
