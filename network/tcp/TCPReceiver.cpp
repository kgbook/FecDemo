#include <arpa/inet.h>
#include <sys/socket.h>
#include <cerrno>
#include <cstring>
#include <string>
#include "TCPReceiver.h"
#include "NetworkTypedef.h"
#include "ALog.h"

#define LOG_TAG "TCPReceiver"

TCPReceiver::TCPReceiver(uint16_t port) : BaseReceiver(std::string(LOG_TAG)), port_(port), socket_(-1), peerSocket_(-1) {
    buffer_ = static_cast<uint8_t *>(malloc(bufferSize_));
}

TCPReceiver::~TCPReceiver() {
    free(buffer_);
}

void TCPReceiver::start() {
    LOGD(LOG_TAG, "start...");
    socket_ = socket(AF_INET, SOCK_STREAM, 0);
     int optValue = 1;
     setsockopt(socket_, SOL_SOCKET, SO_REUSEADDR, &optValue, sizeof(optValue));

    bzero(&mySocketAddr_, sizeof(mySocketAddr_));
    mySocketAddr_.sin_family = AF_INET;
    mySocketAddr_.sin_addr.s_addr = htonl(INADDR_ANY);
    mySocketAddr_.sin_port = htons(port_);
    if (bind(socket_, (struct sockaddr *) &mySocketAddr_, socketLen_) < 0) {
        LOGE(LOG_TAG, "bind socket error:%s", strerror(errno));
        return;
    }
    if (listen(socket_, 2) < 0) {
        LOGE(LOG_TAG, "listen socket error:%s", strerror(errno));
        return;
    }

    LOGD(LOG_TAG, "listening %d ...", port_);
    Thread::start();
}

void TCPReceiver::stop() {
    LOGD(LOG_TAG, "stop...");
    shutdown(socket_, SHUT_RDWR);
    close(socket_);
    Thread::stop();
}

ssize_t TCPReceiver::readData(int fd, uint8_t *dst, size_t len, int flag) {
    ssize_t nRead;
    size_t nLeft = len;
    const uint8_t *ptr = dst;
    while (nLeft > 0) {
        nRead = recv(fd, dst, nLeft, flag);
        if (nRead > 0) {
            nLeft -= nRead;
            ptr += nRead;
        } else if (nRead == 0) {
            LOGE(LOG_TAG, "read = 0, peer disconnect");
            return 0;
        } else {
            if ((errno == EINTR || errno == EWOULDBLOCK || errno == EAGAIN)) {
                LOGE(LOG_TAG, "errno = %d:%s. read again", errno, strerror(errno));
                continue;
            } else {
                LOGE(LOG_TAG, "errno = %d:%s", errno, strerror(errno));
                return -1;
            }
        }
    }
    return len - nLeft;
}

bool TCPReceiver::threadLoop() {
    TcpPacketHeader header{0};
    if (peerSocket_ == 0) {
        LOGD(LOG_TAG, "waiting peer connect");
        peerSocket_ = accept(socket_, (struct sockaddr *) &peerSocketAddr_, &socketLen_);
        if ((peerSocket_) < 0) {
            LOGE(LOG_TAG, "accept socket error:%s", strerror(errno));
            peerSocket_ = 0;
            return true;
        }
        LOGD(LOG_TAG, "accept peer connect");
    }

    ssize_t nRead;
    while (true) {
        nRead = readData(peerSocket_, reinterpret_cast<uint8_t *>(&header), 8, MSG_WAITALL);
        if (nRead == 8) {
            nRead = readData(peerSocket_, buffer_, header.payloadSize, MSG_WAITALL);
            if (nRead == header.payloadSize) {
                output(buffer_, nRead, reinterpret_cast<uint8_t *>(&header));
            } else {
                LOGE(LOG_TAG, "read frame error, expect size = %u, but get recv size = %ld", header.payloadSize, nRead);
            }
            continue;
        } else if (nRead <= 0) {
            peerSocket_ = 0;
            return true;
        } else {
            LOGE(LOG_TAG, "never reach this");
        }
    }
}

