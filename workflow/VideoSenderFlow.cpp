#include "VideoSenderFlow.h"

#include <utility>
#include "NetworkTransportFactory.h"
#include "LocalStreamReader.h"
#include "ALog.h"

#define LOG_TAG "VideoSenderFlow"
#define TEST_FILE "miracast-dump.ts"

VideoSenderFlow::VideoSenderFlow(std::string clientIP, int16_t port, std::string proto)
    : WorkFlow(LOG_TAG),
    clientIP_(std::move(clientIP)), port_(port), proto_(std::move(proto)),
    sender_(nullptr), reader_(nullptr) {
    //
}

VideoSenderFlow::~VideoSenderFlow() = default;

void VideoSenderFlow::build() {
    sender_ = NetworkTransportFactory::createSender(clientIP_, port_, proto_, MediaMimeType::kVideo);
    reader_ = new LocalStreamReader(TEST_FILE);
    reader_->connect(sender_);
}

void VideoSenderFlow::start() {
    if (nullptr == sender_ || nullptr == reader_) {
        ALOGE("sender or reader nullptr, build workflow first please!");
        return;
    }
    reader_->start();
}

void VideoSenderFlow::stop() {
    reader_->stop();
}

void VideoSenderFlow::release() {
    if (nullptr != sender_) {
        delete sender_;
        sender_ = nullptr;
    }

    if (nullptr != reader_) {
        delete reader_;
        reader_ = nullptr;
    }
}