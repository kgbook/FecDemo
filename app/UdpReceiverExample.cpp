//
// Created by cory on 2023/11/11.
//

#include <cstdio>
#include "VideoReceiverFlow.h"
#include "ALog.h"
#include "AppTypedef.h"

#define LOG_TAG "UdpReceiverExample"

int main(int argc, char **argv) {
    VideoReceiverFlow receiverFlow(SERVER_IP, SERVER_PORT, NETWORK_PROTO);

    receiverFlow.build();
    receiverFlow.start();

    while (true) {
        char c = fgetc(stdin);
        if ('q' == c || 'Q' == c) {
            LOGI(LOG_TAG, "exit now...");
            break;
        }
    }

    receiverFlow.stop();
    receiverFlow.release();
    return 0;
}