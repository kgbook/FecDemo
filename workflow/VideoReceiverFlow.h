#ifndef FEC_VIDEORECEIVERFLOW_H
#define FEC_VIDEORECEIVERFLOW_H

#include "WorkFlow.h"

class BaseReceiver;
class LocalStreamWriter;

class VideoReceiverFlow : public WorkFlow {
public:
    VideoReceiverFlow(std::string ip, int16_t port, std::string proto);
    ~VideoReceiverFlow() override;

    void build() override;
    void start() override;
    void stop() override;
    void release() override;

private:
    std::string ip_;
    int16_t port_;
    std::string proto_;
    BaseReceiver *receiver_;
    LocalStreamWriter *writer_;
};


#endif //FEC_VIDEORECEIVERFLOW_H
