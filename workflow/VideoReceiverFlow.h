#ifndef FEC_VIDEORECEIVERFLOW_H
#define FEC_VIDEORECEIVERFLOW_H

#include "WorkFlow.h"

class VideoReceiverFlow : public WorkFlow {
public:
    explicit VideoReceiverFlow(std::string proto);
    ~VideoReceiverFlow() override;

    void build() override;
    void start() override;
    void stop() override;
    void release() override;

private:
    std::string proto_;
};


#endif //FEC_VIDEORECEIVERFLOW_H
