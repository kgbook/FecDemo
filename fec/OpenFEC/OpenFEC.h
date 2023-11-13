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

private:
    bool init(FecAlgorithm type, const FecParameter &param);

protected:
    FecAlgorithm type_;
    of_session *session_;
    of_parameters *params_;
};


#endif //FECDEMO_OPENFEC_H
