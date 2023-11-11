#define LOG_TAG "VideoReceiverFlow"

#include <utility>
#include "VideoReceiverFlow.h"
#include "NetworkTransportFactory.h"
#include "ALog.h"

VideoReceiverFlow::VideoReceiverFlow(std::string proto) : WorkFlow(LOG_TAG),
                                                           proto_(std::move(proto)) {

}

VideoReceiverFlow::~VideoReceiverFlow() {

}

void VideoReceiverFlow::build() {
    //
}

void VideoReceiverFlow::start() {
    //
}

void VideoReceiverFlow::stop() {
    //
}

void VideoReceiverFlow::release() {
    //
}
