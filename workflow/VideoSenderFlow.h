#ifndef FEC_VIDEOSENDERFLOW_H
#define FEC_VIDEOSENDERFLOW_H

#include "WorkFlow.h"

class VideoSenderFlow : public WorkFlow {
public:
    VideoSenderFlow(std::string clientIP, std::string proto);
    ~VideoSenderFlow() override;

    void build() override;
    void start() override;
    void stop() override;
    void release() override;

private:
    std::string clientIP_;
    std::string proto_;
};


#endif //FEC_VIDEOSENDERFLOW_H
