//
// Created by cory on 2023/11/12.
//

#ifndef FEC_LOCALSTREAMWRITER_H
#define FEC_LOCALSTREAMWRITER_H

#include "BaseModule.h"

class LocalStreamWriter : public BaseModule {
public:
    explicit LocalStreamWriter(const std::string& path);
    ~LocalStreamWriter() override;
    void input(uint8_t *data, size_t len, uint8_t *privateData) override;

private:
    FILE *outFile_ = nullptr;
};


#endif //FEC_LOCALSTREAMWRITER_H
