#include "VideoSenderFlow.h"
#include "NetworkTransportFactory.h"
#include "ALog.h"

#define LOG_TAG "VideoSenderFlow"

VideoSenderFlow::VideoSenderFlow(std::string clientIP, std::string proto) : WorkFlow(LOG_TAG),
                                                                            clientIP_(std::move(clientIP)),
                                                                            proto_(std::move(proto)) {
    //
}

VideoSenderFlow::~VideoSenderFlow() {
    //
}

void VideoSenderFlow::build() {
    //
}

void VideoSenderFlow::start() {
    //
}

void VideoSenderFlow::stop() {
    //
}

void VideoSenderFlow::release() {
    //
}