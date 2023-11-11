#ifndef FEC_WORKFLOW_H
#define FEC_WORKFLOW_H

#include <string>

class WorkFlow {
public:
    explicit WorkFlow(std::string name);
    virtual ~WorkFlow();

    virtual void build() = 0;
    virtual void start() = 0;
    virtual void stop() = 0;
    virtual void release() = 0;

    virtual void input(uint8_t *data, size_t len, uint8_t *privateData) {};

protected:
    std::string name_;
};


#endif //FEC_WORKFLOW_H
