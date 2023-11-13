//
// Created by dl on 2023/11/13.
//

#ifndef FECDEMO_OPENFECENCODER_H
#define FECDEMO_OPENFECENCODER_H

#include "OpenFEC.h"

class CmaCache;

class OpenFecEncoder : public OpenFEC {
public:
    OpenFecEncoder(FecAlgorithm type, const FecParameter &param);
    ~OpenFecEncoder();

    bool encode(uint8_t *buf, uint32_t len);
    bool getPacket(uint8_t **buf, uint32_t &len); // zero-copy FEC packet, no source packet

private:
    void clear();

private:
    CmaCache *cache_;
    uint32_t packet_count_; // source packet + repair packet
    uint32_t raw_packet_count_; // source packet
    uint32_t last_group_id_; // k source packet + (n - k) repair packet is a group
    uint32_t fec_packet_count_;
    bool have_fec_packet_;
};

#endif //FECDEMO_OPENFECENCODER_H
