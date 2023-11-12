//
// Created by cory on 2023/11/11.
//

#include "VideoSenderFlow.h"
#include "AppTypedef.h"

int main(int argc, char **argv) {
    VideoSenderFlow senderFlow(CLIENT_IP, SERVER_PORT, NETWORK_PROTO);
    return 0;
}