#ifndef FEC_VIDEOSENDERFLOW_H
#define FEC_VIDEOSENDERFLOW_H

#include "WorkFlow.h"

class BaseModule;
class LocalStreamReader;

class VideoSenderFlow : public WorkFlow {
public:
    VideoSenderFlow(std::string ip, int16_t port, std::string proto);
    ~VideoSenderFlow() override;

    void build() override;
    void start() override;
    void stop() override;
    void release() override;

private:
    std::string ip_;
    std::string proto_;
    int16_t port_;
    BaseModule *sender_;
    LocalStreamReader *reader_;
};


#endif //FEC_VIDEOSENDERFLOW_H
