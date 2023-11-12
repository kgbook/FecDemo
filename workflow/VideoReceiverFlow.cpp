#include "VideoReceiverFlow.h"
#include "LocalStreamWriter.h"
#include "NetworkTransportFactory.h"
#include "ALog.h"

#define LOG_TAG "VideoReceiverFlow"
#define OUTPUT_FILE "../../res/out.ts"

VideoReceiverFlow::VideoReceiverFlow(std::string ip, int16_t port, std::string proto)
    : WorkFlow(LOG_TAG),
      ip_(std::move(ip)), port_(port), proto_(std::move(proto)),
      receiver_(nullptr), writer_(nullptr) {
    //
}

VideoReceiverFlow::~VideoReceiverFlow() = default;

void VideoReceiverFlow::build() {
    receiver_ = NetworkTransportFactory::createReceiver(ip_, port_, proto_, MediaMimeType::kVideo);
    writer_ = new LocalStreamWriter(OUTPUT_FILE);
    receiver_->connect(writer_);
}

void VideoReceiverFlow::start() {
    if (nullptr == receiver_ || nullptr == writer_) {
        ALOGE("receiver or write nullptr, build workflow first please!");
        return;
    }
    receiver_->start();
}

void VideoReceiverFlow::stop() {
    receiver_->stop();
}

void VideoReceiverFlow::release() {
    if (nullptr != receiver_) {
        delete receiver_;
        receiver_ = nullptr;
    }
    if (nullptr != writer_) {
        delete writer_;
        writer_ = nullptr;
    }
}
