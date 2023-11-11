#include <unistd.h>
#include <cstring>
#include "TCPSender.h"
#include "ALog.h"
#include "NetworkTypedef.h"

#define LOG_TAG "TCPSender"

TCPSender::TCPSender(std::string peerIP, uint16_t port) : BaseModule("TCPSender"),
                                                        peerIp_(std::move(peerIP)), port_(port) {
    if ((socket_ = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        LOGE(LOG_TAG, "create socket error:%s", strerror(errno));
        return;
    }
    bzero(&peerSocketAddr_, socketLen_);
    peerSocketAddr_.sin_family = AF_INET;
    peerSocketAddr_.sin_port = htons(port_);
    if (inet_pton(AF_INET, peerIp_.c_str(), &peerSocketAddr_.sin_addr) < 0) {
        LOGE(LOG_TAG, "peer ip %s error:%s", peerIp_.c_str(), strerror(errno));
        return;
    }

    LOGD(LOG_TAG, "try connect to peer %s", peerIp_.c_str());
    if (::connect(socket_, (struct sockaddr *) &peerSocketAddr_, socketLen_) < 0) {
        LOGE(LOG_TAG, "connect peer error:%s", strerror(errno));
        return;
    }
    socketConnected_ = true;
    LOGD(LOG_TAG, "connect to peer %s:%d success", peerIp_.c_str(), port_);
}

TCPSender::~TCPSender() {
    LOGD(LOG_TAG, "~TCPSender");
    close(socket_);
}

ssize_t TCPSender::writeData(int fd, const void *data, size_t len, int flag) {
    ssize_t nWritten;
    size_t nLeft = len;
    const char *ptr = static_cast<const char *>(data);
    while (nLeft > 0) {
        nWritten = send(fd, ptr, nLeft, flag);
        if (nWritten > 0) {
            nLeft -= nWritten;
            ptr += nWritten;
        } else if (nWritten == 0) {
            LOGE(LOG_TAG, "write = 0, disconnect");
            socketConnected_ = false;
            return (-1);
        } else {
            if ((errno == EINTR || errno == EWOULDBLOCK || errno == EAGAIN)) {
                LOGE(LOG_TAG, "write again");
                usleep(5000);
            } else{
                LOGE(LOG_TAG, "errno = %d:%s", errno, strerror(errno));
                socketConnected_ = false;
                return -1;
            }
        }
    }
    return nWritten;
}

void TCPSender::input(uint8_t *data, size_t len, uint8_t *privateData) {
    TcpPacketHeader header{0};
    header.payloadSize = len;
    header.flags = 0;

    if (socketConnected_) {
        writeData(socket_, &header, 8, MSG_MORE);
        writeData(socket_, data, len, 0);
    }
}