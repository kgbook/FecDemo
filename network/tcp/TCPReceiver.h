//
// Created by jingle on 2023/9/23.
//

#ifndef FEC_TCPRECEIVER_H
#define FEC_TCPRECEIVER_H

#include <unistd.h>
#include "BaseReceiver.h"

class TCPReceiver : public BaseReceiver {
public:
    TCPReceiver(uint16_t port);
    virtual ~TCPReceiver();

    void start();
    void stop();
protected:
    bool threadLoop();

private:
    ssize_t readData(int fd, uint8_t *dst, size_t len, int flag);

private:
    uint16_t port_;
    int socket_;
    int peerSocket_;
    struct sockaddr_in mySocketAddr_{};
    struct sockaddr_in peerSocketAddr_{};
    socklen_t socketLen_ = sizeof(struct sockaddr_in);

    uint8_t *buffer_;
    size_t bufferSize_ = 1024 * 1024;
};


#endif //FEC_TCPRECEIVER_H
