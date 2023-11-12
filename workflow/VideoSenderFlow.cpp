#include "VideoSenderFlow.h"

#include <utility>
#include "NetworkTransportFactory.h"
#include "LocalStreamReader.h"
#include "ALog.h"

#define LOG_TAG "VideoSenderFlow"
#define TEST_FILE "../../res/miracast-dump.ts"

VideoSenderFlow::VideoSenderFlow(std::string ip, int16_t port, std::string proto)
    : WorkFlow(LOG_TAG),
      ip_(std::move(ip)), port_(port), proto_(std::move(proto)),
      sender_(nullptr), reader_(nullptr) {
    //
}

VideoSenderFlow::~VideoSenderFlow() = default;

void VideoSenderFlow::build() {
    sender_ = NetworkTransportFactory::createSender(ip_, port_, proto_, MediaMimeType::kVideo);
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