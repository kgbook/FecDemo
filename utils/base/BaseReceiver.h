#ifndef FEC_BASERECEIVER_H
#define FEC_BASERECEIVER_H

#include "BaseModule.h"
#include "thread.h"

class BaseReceiver : public BaseModule, public Thread {
public:
    explicit BaseReceiver(const std::string& name) : BaseModule(name), Thread(name) {};
    ~BaseReceiver() override = default;
};


#endif //FEC_BASERECEIVER_H
