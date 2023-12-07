//
// Created by DaveRiedel on 15-Jun-22.
//

#ifndef BITSANDDROIDSCONNECTOR_OUTPUTCONVERTERS_H
#define BITSANDDROIDSCONNECTOR_OUTPUTCONVERTERS_H


#include <utility>
#include <string>
#include "models/commands/output.h"

class OutputConverters {
public:
    OutputConverters();
    std::string formatOutgoingFloat(float received, Output output);
    std::string formatOutgoingString(std::string received, Output output);

private:

    int radianToDegree(float rec);


};


#endif //BITSANDDROIDSCONNECTOR_OUTPUTCONVERTERS_H
