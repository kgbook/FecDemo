//
// Created by cory on 2023/11/12.
//

#ifndef FEC_FECENCODER_H
#define FEC_FECENCODER_H

#include "BaseModule.h"
#include "FecTypedef.h"

class OpenFEC;

class FecEncoder : BaseModule {
public:
    FecEncoder(std::string name, FecAlgorithm type, const FecParameter &param);
    ~FecEncoder() override;

    bool encode(uint8_t *buf, size_t len);

private:
    bool initEncoder(FecAlgorithm type, const FecParameter &param);

private:
    OpenFEC *openFec;
};


#endif //FEC_FECENCODER_H
