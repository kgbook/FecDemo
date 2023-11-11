#include <errno.h>
#include <string.h>
#include "BaseModule.h"
#include "ALog.h"

#define LOG_TAG "BaseModule"

BaseModule::BaseModule(std::string name): moduleName_(std::move(name)) {
    LOGD(LOG_TAG, "create module %s", moduleName_.c_str());
}

BaseModule::~BaseModule() {

}

void BaseModule::input(uint8_t *data, size_t len, uint8_t *privateData) {

}

void BaseModule::connect(BaseModule *nextModule) {
    nextModuleList_.push_back(nextModule);
}

void BaseModule::disconnect(BaseModule *nextModule) {
    for (auto it = nextModuleList_.begin(); it != nextModuleList_.end(); it++) {
        if (*it == nextModule) {
            nextModuleList_.erase(it);
            break;
        }
    }
}

void BaseModule::setElapsedTimeDebug(bool debug) {

}

bool BaseModule::dumpOutputData(bool enable, const std::string& savePath) {
    if (enable) {
        if (dumpFile_ == nullptr) {
            dumpFile_ = fopen(savePath.c_str(), "w");
            if (dumpFile_ == nullptr) {
                LOGE(LOG_TAG, "open file %s error: %s", savePath.c_str(), strerror(errno));
                return false;
            }
        }
    } else {
        if (dumpFile_) {
            fflush(dumpFile_);
            fclose(dumpFile_);
            dumpFile_ = nullptr;
        }
    }
    return true;
}

void BaseModule::output(uint8_t *data, size_t len, uint8_t *privateData) {
    for (auto & it : nextModuleList_) {
        it->input(data, len, privateData);
    }

    if (dumpFile_) {
        auto writeSize = fwrite(data, 1, len, dumpFile_);
        if (writeSize != len) {
            LOGE(LOG_TAG, "write size = %d, expect %d, errno = %s", writeSize, len, strerror(errno));
        }
    }
}

std::vector<BaseModule *> BaseModule::getNextModuleList() {
    return nextModuleList_;
}