#ifndef FEC_UDPRECEIVER_H
#define FEC_UDPRECEIVER_H

#include <arpa/inet.h>
#include <sys/socket.h>
#include <string>
#include <map>
#include "BaseReceiver.h"
#include "NetworkTypedef.h"

class UDPReceiver : public BaseReceiver {
public:
    explicit UDPReceiver(uint16_t port);
    ~UDPReceiver() override;

protected:
    bool threadLoop() override;

private:
    uint16_t port_;
    int socket_ = 0;
    struct sockaddr_in mySocketAddr_{};
    struct sockaddr_in peerSocketAddr_{};
    socklen_t socketLen_;
    uint8_t *recvBuffer_;
    char packet_[MAX_UDP_PAYLOAD]{};
    std::map<int, std::vector<char>> packets_;
};


#endif //FEC_UDPRECEIVER_H
