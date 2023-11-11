#include "NetworkTransportFactory.h"
#include "tcp/TCPSender.h"
#include "tcp/TCPReceiver.h"
#include "udp/UDPSender.h"
#include "udp/UDPReceiver.h"

BaseModule* NetworkTransportFactory::createSender(const std::string& peerIp, uint16_t port, const std::string& proto, const MediaMimeType type) {
    BaseModule* sender = nullptr;
    if (proto.find("tcp") != std::string::npos) {
        sender = new TCPSender(peerIp, port);
    } else if (proto.find("udp") != std::string::npos) {
        sender = new UDPSender(peerIp, port);
    }

    return sender;
}

BaseReceiver* NetworkTransportFactory::createReceiver(uint16_t port, const std::string& proto, const MediaMimeType type) {
    BaseReceiver* receiver = nullptr;
    if (proto.find("tcp") != std::string::npos) {
        receiver = new TCPReceiver(port);
    } else if (proto.find("udp") != std::string::npos) {
        receiver = new UDPReceiver(port);
    }
    return receiver;
}
