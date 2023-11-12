#ifndef FEC_NETWORKTRANSPORTFACTORY_H
#define FEC_NETWORKTRANSPORTFACTORY_H

#include "BaseModule.h"
#include "BaseReceiver.h"
#include "NetworkTypedef.h"

class NetworkTransportFactory {
public:
    static BaseModule* createSender(const std::string& peerIp, uint16_t port, const std::string& proto, MediaMimeType type);
    static BaseReceiver* createReceiver(uint16_t port, const std::string& proto, MediaMimeType type);
};

#endif //FEC_NETWORKTRANSPORTFACTORY_H
