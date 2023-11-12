//
// Created by cory on 2023/11/12.
//

#include "ALog.h"
#include "OpenFEC.h"
#include "FecEncoder.h"

#define LOG_TAG "FecEncoder"


FecEncoder::FecEncoder(std::string name, FecAlgorithm type, const FecParameter &param)
    : BaseModule(std::move(name)), openFec(nullptr) {
    if (!initEncoder(type, param)) {
        ALOGE("initEncoder failed!");
        return;
    }
}

FecEncoder::~FecEncoder() {
    if (nullptr != openFec) {
        delete openFec;
        openFec = nullptr;
    }
}

bool FecEncoder::encode(uint8_t *buf, size_t len) {
    return false;
}

bool FecEncoder::initEncoder(FecAlgorithm type, const FecParameter &param) {
    switch (type) {
        case FecAlgorithm::kReedSolomonGF2M:
        case FecAlgorithm::kLdpcStaircase:
        case FecAlgorithm::k2DParityMatrix: {
            openFec = new OpenFEC(type, param);
            break;
        }
        case FecAlgorithm::kFecal: {
            break;
        }
        default: {
            break;
        }
    }
    return true;
}