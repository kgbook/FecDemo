//
// Created by jingle on 2023/9/23.
//

#ifndef FEC_TCPSENDER_H
#define FEC_TCPSENDER_H

#include<arpa/inet.h>
#include<sys/socket.h>
#include <string>
#include "BaseModule.h"
#include "thread.h"

class TCPSender : public BaseModule {
public:
    TCPSender(std::string peerIP, uint16_t port);
    virtual ~TCPSender();
    void input(uint8_t *data, size_t len, uint8_t *privateData) override;

private:
    ssize_t writeData(int fd, const void *data, size_t len, int flag);

private:
    std::string peerIp_;
    uint16_t port_;
    int socket_ = 0;
    struct sockaddr_in peerSocketAddr_{};
    socklen_t socketLen_ = sizeof(struct sockaddr_in);
    bool socketConnected_ = false;
};


#endif //FEC_TCPSENDER_H
