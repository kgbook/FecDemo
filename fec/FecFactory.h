//
// Created by cory on 2023/11/12.
//

#ifndef FEC_FECFACTORY_H
#define FEC_FECFACTORY_H

#include "BaseModule.h"
#include "BaseReceiver.h"

class FecFactory {
public:
    BaseModule* createEncoder();
    BaseReceiver* createDecoder();
};


#endif //FEC_FECFACTORY_H
