//
// Created by cory on 11/12/23.
//

#ifndef FECDEMO_FECTYPEDEF_H
#define FECDEMO_FECTYPEDEF_H

enum class FecAlgorithm {
    kReedSolomonGF2M,
    kLdpcStaircase,
    k2DParityMatrix,
    kFecal,
    kwirehair,
};

union FecParameter {
    struct {
        uint32_t codec_id;
        uint32_t k;
        uint32_t n;
    } openfec;

    struct {

    } fecal;
};

#endif //FECDEMO_FECTYPEDEF_H
