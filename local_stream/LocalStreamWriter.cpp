//
// Created by cory on 2023/11/12.
//

#include <cerrno>
#include <cstring>
#include "ALog.h"
#include "LocalStreamWriter.h"

#define LOG_TAG "LocalStreamWriter"

LocalStreamWriter::LocalStreamWriter(const std::string& path): BaseModule(LOG_TAG) {
    outFile_ = fopen(path.c_str(), "w");
    if (outFile_ == nullptr) {
        ALOGE("open file %s error: %s", path.c_str(), strerror(errno));
    }
}

LocalStreamWriter::~LocalStreamWriter() {
    if (outFile_) {
        fflush(outFile_);
        fclose(outFile_);
        outFile_ = nullptr;
    }
}

void LocalStreamWriter::input(uint8_t *data, size_t len, uint8_t *privateData) {
    if (outFile_) {
        fwrite(data, len, 1, outFile_);
    }
}