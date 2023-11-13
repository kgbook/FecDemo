//
// Created by cory on 2023/11/12.
//

#ifndef FEC_FECFACTORY_H
#define FEC_FECFACTORY_H

#include "FecTypedef.h"

class BaseModule;
class BaseReceiver;

class FecFactory {
public:
    BaseModule* createEncoder(FecAlgorithm type, const FecParameter &param);
    BaseReceiver* createDecoder();
};


#endif //FEC_FECFACTORY_H
