//
// Created by dl on 2023/11/13.
//

#include "OpenFecDecoder.h"

OpenFecDecoder::OpenFecDecoder(FecAlgorithm type, const FecParameter &param) : OpenFEC(type, param) {

}

OpenFecDecoder::~OpenFecDecoder() = default;

bool OpenFecDecoder::decode(uint8_t *buf, uint32_t &len) {
    // of_decode_with_new_symbol
    // of_finish_decoding
    // of_is_decoding_complete
    // of_get_source_symbols_tab
    return false;
}
