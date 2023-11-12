#include "NetworkTransportFactory.h"
#include "udp/UDPSender.h"
#include "udp/UDPReceiver.h"

BaseModule* NetworkTransportFactory::createSender(const std::string& ip, uint16_t port, const std::string& proto, const MediaMimeType type) {
    BaseModule* sender = nullptr;
    if (proto.find("udp") != std::string::npos) {
        sender = new UDPSender(ip, port);
    }

    return sender;
}

BaseReceiver* NetworkTransportFactory::createReceiver(const std::string& ip, uint16_t port, const std::string& proto, const MediaMimeType type) {
    BaseReceiver* receiver = nullptr;
    if (proto.find("udp") != std::string::npos) {
        receiver = new UDPReceiver(ip, port);
    }
    return receiver;
}
