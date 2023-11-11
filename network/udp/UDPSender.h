#ifndef FEC_UDPSENDER_H
#define FEC_UDPSENDER_H

#include <arpa/inet.h>
#include <sys/socket.h>
#include <string>
#include "BaseModule.h"
#include "NetworkTypedef.h"

class UDPSender : public BaseModule {
public:
    UDPSender(std::string clientIP, uint16_t port);
    ~UDPSender() override;
    void input(uint8_t *data, size_t len, uint8_t *privateData) override;

private:
    int wait_send(const char* data, int len);

private:
    std::string clientIP_;
    uint16_t port_;
    int socket_ = 0;
    struct sockaddr_in mySocketAddr_;
    struct sockaddr_in peerSocketAddr_;
    socklen_t socketLen_;
    char packet_[MAX_UDP_PAYLOAD]{};
};


#endif //FEC_UDPSENDER_H
