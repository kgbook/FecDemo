//
// Created by cory on 2023/11/12.
//

#ifndef FEC_FECENCODER_H
#define FEC_FECENCODER_H

#include "BaseModule.h"
#include "FecTypedef.h"

class OpenFecEncoder;

class FecEncoder : public BaseModule {
public:
    FecEncoder(FecAlgorithm type, const FecParameter &param);
    ~FecEncoder() override;

    void input(uint8_t *data, size_t len, uint8_t *privateData) override;

private:
    bool encode(uint8_t *buf, size_t len);
    bool initEncoder(FecAlgorithm type, const FecParameter &param);

private:
    OpenFecEncoder *open_fec_encoder;
    FecAlgorithm type_;
};


#endif //FEC_FECENCODER_H
