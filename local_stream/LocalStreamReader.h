//
// Created by cory on 2023/11/12.
//

#ifndef FEC_LOCALSTREAMREADER_H
#define FEC_LOCALSTREAMREADER_H

#include "BaseModule.h"
#include "thread.h"

class LocalStreamReader : public BaseModule, public Thread  {
public:
    explicit LocalStreamReader(const std::string& path);
    ~LocalStreamReader() override;

protected:
    bool threadLoop() override;

private:
    FILE *inputFile_;
    uint8_t  *buffer_;
};


#endif //FEC_LOCALSTREAMREADER_H
