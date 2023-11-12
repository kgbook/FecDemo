#ifndef FEC_VIDEOSENDERFLOW_H
#define FEC_VIDEOSENDERFLOW_H

#include "WorkFlow.h"

class BaseModule;
class LocalStreamReader;

class VideoSenderFlow : public WorkFlow {
public:
    VideoSenderFlow(std::string clientIP, int16_t port, std::string proto);
    ~VideoSenderFlow() override;

    void build() override;
    void start() override;
    void stop() override;
    void release() override;

private:
    std::string clientIP_;
    std::string proto_;
    int16_t port_;
    BaseModule *sender_;
    LocalStreamReader *reader_;
};


#endif //FEC_VIDEOSENDERFLOW_H
