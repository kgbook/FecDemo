//
// Created by cory on 2023/11/11.
//

#include "NetworkTransportFactory.h"
#include "AppTypedef.h"

int main(int argc, char **argv) {
    int16_t port = SERVER_PORT;
    const char *proto = "udp";
//    const char *ip = SERVER_IP;
    MediaMimeType type = MediaMimeType::kVideo;
    auto receiver = NetworkTransportFactory::createReceiver(port, proto, type);
    return 0;
}