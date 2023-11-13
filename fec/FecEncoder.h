//
// Created by cory on 2023/11/12.
//

#ifndef FEC_FECENCODER_H
#define FEC_FECENCODER_H

#include "BaseModule.h"
#include "FecTypedef.h"

class OpenFecEncoder;

class FecEncoder {
public:
    FecEncoder(FecAlgorithm type, const FecParameter &param);
    ~FecEncoder();

    bool encode(uint8_t *buf, size_t len);

private:
    bool initEncoder(FecAlgorithm type, const FecParameter &param);

private:
    OpenFecEncoder *open_fec_encoder;
    FecAlgorithm type_;
};


#endif //FEC_FECENCODER_H
