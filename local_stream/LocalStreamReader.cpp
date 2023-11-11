//
// Created by cory on 2023/11/12.
//

#include <cerrno>
#include <cstring>
#include "ALog.h"
#include "LocalStreamReader.h"

#define LOG_TAG "LocalStreamReader"
#define BUFFER_SIZE  (1024*1024)

LocalStreamReader::LocalStreamReader(const std::string& path) : BaseModule(LOG_TAG), Thread(LOG_TAG) {
    ALOGD("open file %s", path.c_str());
    inputFile_ = fopen(path.c_str(), "r");
    if (inputFile_ == nullptr) {
        ALOGE("open file %s error: %s", path.c_str(), strerror(errno));
    }

    buffer_ = static_cast<uint8_t *>(malloc(BUFFER_SIZE + 1));
}

LocalStreamReader::~LocalStreamReader() {
    if (inputFile_) {
        fclose(inputFile_);
        inputFile_ = nullptr;
    }

    if (buffer_) {
        free(buffer_);
        buffer_ = nullptr;
    }
}

bool LocalStreamReader::threadLoop() {
    size_t nRead = fread(buffer_, 1, BUFFER_SIZE, inputFile_);
    if(nRead <= 0) {
        output(nullptr, 0, nullptr);
        return false;
    }
    output(buffer_, nRead, nullptr);
    return true;
}
