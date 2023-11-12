#ifndef FEC_UDPSENDER_H
#define FEC_UDPSENDER_H

#include <arpa/inet.h>
#include <sys/socket.h>
#include <string>
#include "BaseModule.h"
#include "NetworkTypedef.h"

// udp server
class UDPSender : public BaseModule {
public:
    UDPSender(const std::string&  serverIP, uint16_t serverPort);
    ~UDPSender() override;
    void input(uint8_t *data, size_t len, uint8_t *privateData) override;

private:
    int sendMsg(const char* data, size_t len);
    bool waitWriteReady(int socket);

private:
    int socket_;
    uint32_t frame_seq_;
    uint32_t packet_seq_;
    struct sockaddr_in serverSocketAddr_{};
    volatile bool init_;
};


#endif //FEC_UDPSENDER_H
