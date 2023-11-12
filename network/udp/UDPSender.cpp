#include <unistd.h>
#include <cstring>
#include "UDPSender.h"
#include "NetworkTypedef.h"
#include "ALog.h"

#define LOG_TAG "UDPSender"

UDPSender::UDPSender(const std::string&  serverIP, uint16_t serverPort)
    : BaseModule(LOG_TAG), frame_seq_(NETWORK_INIT_FRAME_SEQ_NO), packet_seq_(NETWORK_INIT_PACKET_SEQ_NO), init_(false) {
    socket_ = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (socket_ < 0) {
        ALOGE("create socket failed, fd:%d", socket_);
        return ;
    }

    memset(&serverSocketAddr_, 0, sizeof(serverSocketAddr_));
    serverSocketAddr_.sin_family = AF_INET;
    serverSocketAddr_.sin_port = htons(serverPort);
    if (inet_pton(AF_INET, serverIP.c_str(), &serverSocketAddr_.sin_addr) != 1)
    {
        ALOGE("inet_pton failed!");
        return ;
    }
    init_ = true;
}

UDPSender::~UDPSender() {
    close(socket_);
}

void UDPSender::input(uint8_t *data, size_t len, uint8_t *privateData) {
    if (!init_) {
        ALOGE("init first please!");
        return;
    }
    const int32_t bytesPerTime = NETWORK_MAX_UDP_PAYLOAD_SIZE - NETWORK_UDP_PACKET_TAIL_SIZE;
    size_t packetnum = len / bytesPerTime + 1;
    size_t remainBytes = len % bytesPerTime;
    volatile uint8_t *visitor = (uint8_t*)data;
    size_t payloadLen;
    char sndbuf[2048];
    if (remainBytes == 0) {
        packetnum -= 1;
    }

    for (int i = 0; i < packetnum; ++i) {
        if (i == (packetnum - 1)) {
            payloadLen = (0 == remainBytes) ?  bytesPerTime : remainBytes;
        } else {
            payloadLen = bytesPerTime;
        }
        memcpy(&sndbuf[0], (const void *)visitor, payloadLen);
        visitor += payloadLen;
        auto *tail = (UdpPacketTail *) &sndbuf[payloadLen];
        tail->packet_seq = packet_seq_++;
        if (packet_seq_ >= UINT32_MAX) {
            packet_seq_ = NETWORK_INIT_PACKET_SEQ_NO;
        }
        tail->frame_seq = frame_seq_;
        tail->total_packets = packetnum;
        if (frame_seq_ >= UINT32_MAX) {
            frame_seq_ = NETWORK_INIT_FRAME_SEQ_NO;
        }

        payloadLen += NETWORK_UDP_PACKET_TAIL_SIZE;
        if (sendMsg(sndbuf, payloadLen) < 0)
        {
            ALOGV("sendMsg failed");
            continue;
        }
        ALOGV("i:%d, packetnum:%d, seq:%u, payloadLen:%d, total len:%d", i, packetnum, frame_seq_, payloadLen - NETWORK_UDP_PACKET_TAIL_SIZE, len);
    }
    if (++frame_seq_ >= UINT32_MAX) {
        frame_seq_ = NETWORK_INIT_FRAME_SEQ_NO;
    }
}

int UDPSender::sendMsg(const char *data, size_t len) {
    ssize_t bytesSend = -1;
    if (waitWriteReady(socket_)) {
        bytesSend = sendto(socket_, data, len, 0, (struct sockaddr*)&serverSocketAddr_, sizeof(struct sockaddr_in));
        if (bytesSend < 0) {
            LOGD(LOG_TAG, "sendto error = %d, %s", errno, strerror(errno));
            return -1;
        }
    }
    return (int)bytesSend;
}

bool UDPSender::waitWriteReady(int socket) {
    fd_set writefds;
    FD_ZERO(&writefds);
    FD_SET(socket, &writefds);

    struct timeval timeout{};
    timeout.tv_sec = 2;
    timeout.tv_usec = 0;

    int ready = select(socket + 1, nullptr, &writefds, nullptr, &timeout);
    if (ready < 0) {
        LOGD(LOG_TAG, "select error.");
        return false;
    }
    return FD_ISSET(socket_, &writefds);
}
