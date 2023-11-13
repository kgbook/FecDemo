//
// Created by cory on 2023/11/12.
//

#include "FecEncoder.h"
#include "FecDecoder.h"
#include "FecFactory.h"

BaseModule *FecFactory::createEncoder(FecAlgorithm type, const FecParameter &param) {
    BaseModule *encoder = nullptr;
    switch (type) {
    case FecAlgorithm::kReedSolomonGF2M:
    case FecAlgorithm::kLdpcStaircase:
    case FecAlgorithm::k2DParityMatrix: {
        encoder = new FecEncoder(type, param);
        break;
    }
    case FecAlgorithm::kFecal:
    default: {
        break;
    }
    }
    return encoder;
}

BaseReceiver *FecFactory::createDecoder() {
    return nullptr;
}
