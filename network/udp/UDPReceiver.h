#ifndef FEC_UDPRECEIVER_H
#define FEC_UDPRECEIVER_H

#include <arpa/inet.h>
#include <sys/socket.h>
#include <string>
#include <map>
#include "BaseReceiver.h"
#include "NetworkTypedef.h"

class UdpFrameBuf;

// udp client
class UDPReceiver : public BaseReceiver {
public:
    UDPReceiver(const std::string&  serverIP, uint16_t serverPort);
    ~UDPReceiver() override;

protected:
    bool threadLoop() override;

private:
    bool recvFrame(int socket);
    bool waitReadReady(int socket);

private:
    int socket_;
    UdpFrameBuf *framebuf_[2]{};
    int32_t curBufIndex_;
};


#endif //FEC_UDPRECEIVER_H
