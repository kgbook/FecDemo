//
// Created by dl on 2023/11/13.
//

#include <cstdlib>
#include "of_openfec_api.h"
#include "ALog.h"
#include "CmaCache.h"
#include "OpenFecEncoder.h"

#define LOG_TAG "OpenFecEncoder"

OpenFecEncoder::OpenFecEncoder(FecAlgorithm type, const FecParameter &param)
    :OpenFEC(type, param), packet_count_(0), raw_packet_count_(0),
     last_group_id_(0), fec_packet_count_(0), have_fec_packet_(false)
{
    cache_ =  new CmaCache(params_->nb_source_symbols + params_->nb_repair_symbols, params_->encoding_symbol_length);
}

OpenFecEncoder::~OpenFecEncoder()
{
    delete cache_;
}

bool OpenFecEncoder::getPacket(uint8_t **buf, uint32_t &len)
{
    if (nullptr == buf) {
        ALOGE("buf nullptr!");
        return false;
    }
    if((raw_packet_count_ != 0) && ((raw_packet_count_ % params_->nb_source_symbols) == 0)){
        have_fec_packet_ = true;
    }
    if (!have_fec_packet_) {
        return false;
    }
    uint32_t cache_id = params_->nb_source_symbols + fec_packet_count_ % params_->nb_repair_symbols;
    *buf = cache_->at(cache_id);
    ALOGV("get fec packet, cached_id:%u, fec_packet_coun:%u, have_fec_packet_:%d", cache_id, fec_packet_count_, have_fec_packet_);
    fec_packet_count_++;
    if (fec_packet_count_ % params_->nb_repair_symbols == 0) {
        have_fec_packet_ = false;
        fec_packet_count_ = 0;
    }
    return true;
}

bool OpenFecEncoder::encode(uint8_t* buf, uint32_t len)
{
    uint32_t cache_id = raw_packet_count_ % params_->nb_source_symbols;
    bool should_build_fec_packet = (raw_packet_count_ != 0 && cache_id == 0);
    packet_count_++;
    raw_packet_count_++;
    cache_->copy(cache_id, buf, len);
    if (should_build_fec_packet) {
        for (uint32_t esi = params_->nb_source_symbols; esi < (params_->nb_source_symbols + params_->nb_repair_symbols); ++esi) {
            if (of_build_repair_symbol(session_, (void **)cache_->at(0), esi) != OF_STATUS_OK) {
                ALOGE("of_build_repair_symbol() failed for esi=%u", esi);
                continue;
            }
            packet_count_++;
        }
        last_group_id_++;
    }
    ALOGD("packet_count:%u, raw_packet_count:%u, cache_id:%u, last_group_id:%d", packet_count_, raw_packet_count_, cache_id, last_group_id_);
    return true;
}

void OpenFecEncoder::clear()
{
    packet_count_ = 0;
    raw_packet_count_ = 0;
}
