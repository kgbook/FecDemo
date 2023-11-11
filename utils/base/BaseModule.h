#ifndef FEC_BASEMODULE_H
#define FEC_BASEMODULE_H

#include <string>
#include <vector>

class BaseModule {
public:
    explicit BaseModule(std::string name);
    virtual ~BaseModule();

    virtual void input(uint8_t *data, size_t len, uint8_t *privateData);
    void connect(BaseModule *nextModule);
    void disconnect(BaseModule *nextModule);
    std::vector<BaseModule *> getNextModuleList();

    void setElapsedTimeDebug(bool debug);
    bool dumpOutputData(bool enable, const std::string& savePath);

protected:
    virtual void output(uint8_t *data, size_t len, uint8_t *privateData);

protected:
    std::string moduleName_;
    std::vector<BaseModule *> nextModuleList_;
    FILE *dumpFile_ = nullptr;
};


#endif //FEC_BASEMODULE_H
