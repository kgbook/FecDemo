//
// Created by cory on 11/12/23.
//

#include <cstdlib>
#include "OpenFEC.h"
#include "ALog.h"
#include "of_openfec_api.h"
#include "NetworkTypedef.h"

#define LOG_TAG "OpenFEC"

#define OPENFEC_LOG_LEVEL  (2)

OpenFEC::OpenFEC(FecAlgorithm type, const FecParameter &param)
    : type_(type), session_(nullptr), params_(nullptr) {
    if (!init(type_, param)) {
        ALOGE("init fec failed!");
        return;
    }
    if (((recvd_symbols_tab_ = (void**) calloc(param.openfec.n, sizeof(void*))) == nullptr) ||
        ((src_symbols_tab_ = (void**) calloc(param.openfec.n, sizeof(void*))) == nullptr))
    {
        ALOGE("OOM, calloc failed for enc_symbols_tab, n=%u", param.openfec.n);
        return;
    }
}

OpenFEC::~OpenFEC() {

}

bool OpenFEC::encode(uint8_t *buf, int32_t len) {
    return false;
}

bool OpenFEC::decode(uint8_t *buf, int32_t len) {
    return false;
}

bool OpenFEC::init(FecAlgorithm type, const FecParameter &param) {
    of_codec_id_t codecId;
    switch (type) {
        case FecAlgorithm::kReedSolomonGF2M: {
            of_rs_2_m_parameters_t	*rs_params;
            if ((rs_params = (of_rs_2_m_parameters_t *)calloc(1, sizeof(*rs_params))) == NULL)
            {
                ALOGE("no memory for codec %d", static_cast<int>(type));
                break;
            }
            rs_params->m = 8;
            params_ = (of_parameters_t *) rs_params;
            break;
        }
        default: {
            ALOGE("%d type not support yet!", static_cast<int32_t>(type));
            return false;
        }
    }
    if (nullptr == params_) {
        ALOGE("params_ nullptr!");
        return false;
    }
    params_->nb_source_symbols	= param.openfec.k;		/* fill in the generic part of the of_parameters_t structure */
    params_->nb_repair_symbols	= param.openfec.n - param.openfec.k;
    params_->encoding_symbol_length	= NETWORK_MAX_UDP_PAYLOAD_SIZE;
    int rc = of_create_codec_instance(&session_, codecId, OF_DECODER, OPENFEC_LOG_LEVEL);
    if (rc != OF_STATUS_OK) {
        ALOGE("of_create_codec_instance() failed");
        return false;
    }
    if (of_set_fec_parameters(session_, params_) != OF_STATUS_OK)
    {
        ALOGE("of_set_fec_parameters() failed for codec_id %d", codecId);
        return false;
    }
    return true;
}
