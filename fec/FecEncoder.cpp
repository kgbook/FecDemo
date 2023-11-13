//
// Created by cory on 2023/11/12.
//

#include "ALog.h"
#include "FecEncoder.h"

#ifdef ENABLE_OPENFEC
#include "OpenFecEncoder.h"
#endif

#define LOG_TAG "FecEncoder"


FecEncoder::FecEncoder(FecAlgorithm type, const FecParameter &param)
    : BaseModule(LOG_TAG), open_fec_encoder(nullptr), type_(type) {
    if (!initEncoder(type, param)) {
        ALOGE("initEncoder failed!");
        return;
    }
}

FecEncoder::~FecEncoder() {
#ifdef ENABLE_OPENFEC
    if (nullptr != open_fec_encoder) {
        delete open_fec_encoder;
        open_fec_encoder = nullptr;
    }
#endif
}

bool FecEncoder::initEncoder(FecAlgorithm type, const FecParameter &param) {
    switch (type) {
        case FecAlgorithm::kReedSolomonGF2M:
        case FecAlgorithm::kLdpcStaircase:
        case FecAlgorithm::k2DParityMatrix: {
#ifdef ENABLE_OPENFEC
            open_fec_encoder = new OpenFecEncoder(type, param);
            break;
#endif
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

bool FecEncoder::encode(uint8_t *buf, size_t len) {
    switch (type_) {
    case FecAlgorithm::kReedSolomonGF2M:
    case FecAlgorithm::kLdpcStaircase:
    case FecAlgorithm::k2DParityMatrix: {
#ifdef ENABLE_OPENFEC
        return open_fec_encoder->encode(buf, len);
#endif
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

void FecEncoder::input(uint8_t* data, size_t len, uint8_t* privateData)
{
    encode(data, len);
    // FIXME: get FEC packet ???
//    open_fec_encoder->getPacket()
}
