//
// Created by cory on 11/12/23.
//

#ifndef FECDEMO_OPENFEC_H
#define FECDEMO_OPENFEC_H

#include <cstdint>
#include "FecTypedef.h"

struct of_parameters;
struct of_session;

class OpenFEC {
public:
    OpenFEC(FecAlgorithm type, const FecParameter &param);
    ~OpenFEC();

    bool encode(uint8_t *buf, int32_t len);
    bool decode(uint8_t *buf, int32_t len);

private:
    bool init(FecAlgorithm type, const FecParameter &param);

private:
    FecAlgorithm type_;
    of_session *session_;
    of_parameters *params_;
    void **recvd_symbols_tab_;
    void **src_symbols_tab_;
};


#endif //FECDEMO_OPENFEC_H
