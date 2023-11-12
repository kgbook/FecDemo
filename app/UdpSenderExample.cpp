//
// Created by cory on 2023/11/11.
//

#include <cstdio>
#include "VideoSenderFlow.h"
#include "ALog.h"
#include "AppTypedef.h"

#define LOG_TAG "UdpSenderExample"

int main(int argc, char **argv) {
    VideoSenderFlow senderFlow(SERVER_IP, SERVER_PORT, NETWORK_PROTO); // ip not used on server side, INADDR_ANY by default
    senderFlow.build();
    senderFlow.start();

    while (true) {
        char c = fgetc(stdin);
        if ('q' == c || 'Q' == c) {
            LOGI(LOG_TAG, "exit now...");
            break;
        }
    }

    senderFlow.stop();
    senderFlow.release();
    return 0;
}