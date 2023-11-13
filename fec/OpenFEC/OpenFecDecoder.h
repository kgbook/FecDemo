//
// Created by dl on 2023/11/13.
//

#ifndef FECDEMO_OPENFECDECODER_H
#define FECDEMO_OPENFECDECODER_H

#include "OpenFEC.h"

class CmaCache;

class OpenFecDecoder : public OpenFEC {
public:
    OpenFecDecoder(FecAlgorithm type, const FecParameter &param);
    ~OpenFecDecoder();

    bool decode(uint8_t *buf, uint32_t &len);

private:
    CmaCache *cache_{};
};

#endif //FECDEMO_OPENFECDECODER_H
