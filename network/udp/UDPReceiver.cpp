#include <unistd.h>
#include <cstring>
#include "UDPReceiver.h"
#include "NetworkTypedef.h"
#include "UdpFrameBuf.h"
#include "ALog.h"

#define LOG_TAG "UDPReceiver"
#define UDP_MAX_FRAME_BUFFER_SIZE (1024*1024)

UDPReceiver::UDPReceiver(const std::string&  serverIP, uint16_t serverPort)
    : BaseReceiver(LOG_TAG), curBufIndex_(0) {
    socket_ = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (socket_ < 0) {
        ALOGE("create socket failed, fd:%d", socket_);
        return ;
    }
    struct sockaddr_in serverAddr{};
    bzero(&serverAddr, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(serverPort);
    serverAddr.sin_addr.s_addr = htons(INADDR_ANY);
    if (bind(socket_, (struct sockaddr *) &serverAddr, sizeof(serverAddr)) < 0) {
        ALOGE("bind socket error:%s", strerror(errno));
        return;
    }
    for (auto & bufItem : framebuf_) {
        bufItem = new UdpFrameBuf(UDP_MAX_FRAME_BUFFER_SIZE);
    }
    ALOGD("init recv socket ok!");
}

UDPReceiver::~UDPReceiver() {
    close(socket_);
    for (auto & bufItem : framebuf_) {
        delete bufItem;
        bufItem = nullptr;
    }
}

bool UDPReceiver::recvFrame(int socket) {
    if (socket < 0) {
        ALOGE("create socket first please!");
        return false;
    }
    int32_t bufNum = sizeof(framebuf_) / sizeof(framebuf_[0]);
    UdpFrameBuf *curBuf = framebuf_[curBufIndex_];
    UdpPacketTail *curTail;
    ssize_t rc, packetlen;
    struct sockaddr_in peerAddr{};
    socklen_t addrLen = sizeof(peerAddr);

    if (curBuf->buflen() < NETWORK_MAX_UDP_PAYLOAD_SIZE) {
        curBuf->print(LOG_TAG);
        ALOGW("OOM, buf is not big enough, remain:%d, need:%d", curBuf->buflen(), NETWORK_MAX_UDP_PAYLOAD_SIZE);
        goto output;
    }
    rc = recvfrom(socket, curBuf->buf(), curBuf->buflen(), 0, (struct sockaddr*)&peerAddr, &addrLen);
    if (rc < 0)
    {
        ALOGW("recvfrom failed, seq:%d, recvByte:%u, reason:%s", curBuf->seq(), curBuf->size(), strerror(errno));
        goto output;
    }
    packetlen = rc -  NETWORK_UDP_PACKET_TAIL_SIZE;
    curTail = (UdpPacketTail *)(curBuf->buf() + packetlen);
    if (curBuf->seq() != curTail->frame_seq) { // next packet
        ALOGV("new frame, seq: %d ---< %u, packetseq:%u, packetnum:%u, nextByte:%d", curTail->frame_seq, curBuf->seq(), curTail->packet_seq, curTail->total_packets, packetlen);
        int32_t nextBufIndex = (curBufIndex_ + 1) % bufNum;
        UdpFrameBuf *nextBuf = framebuf_[nextBufIndex];
        nextBuf->copy(curTail->frame_seq, curTail->total_packets, curBuf->buf(), packetlen);
        curBufIndex_ = nextBufIndex;
        goto output;
    }
    curBuf->setOffset(packetlen);
    if (curBuf->seq() == NETWORK_INIT_PACKET_SEQ_NO) {
        curBuf->packetNum() = curTail->total_packets;
    }
    ALOGV("seq:%u, consume:%d, collected:%u, total:%u, curTotalByte:%u, curByte:%d, curBufIndex_:%d", curBuf->seq(), curBuf->consume(), curBuf->collected(), curBuf->packetNum(), curBuf->size(), packetlen, curBufIndex_);

output:
    if (curBuf->consume()) {
        output((uint8_t *)curBuf->base(), curBuf->size(), nullptr);
        curBuf->clear();
        return true;
    }
    return false;
}

bool UDPReceiver::threadLoop() {
    if (!waitReadReady(socket_)) {
        return true;
    }
    recvFrame(socket_);
    return true;
}

bool UDPReceiver::waitReadReady(int socket) {
    fd_set readfds;
    FD_ZERO(&readfds);
    FD_SET(socket, &readfds);

    struct timeval timeout{};
    timeout.tv_sec = 2;
    timeout.tv_usec = 0;

    int ready = select(socket + 1, &readfds, nullptr, nullptr, &timeout);
    if (ready < 0) {
        LOGD(LOG_TAG, "select error.");
        return false;
    }
    return FD_ISSET(socket_, &readfds);
}

