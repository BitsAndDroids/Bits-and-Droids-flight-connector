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
    std::string formatOutgoingString(float received, Output output);
    float converOutgoingFloatValue(float toConvert, int mode);
private:


    int radianToDegree(double rec);

    float radianToDegreeFloat(double rec);

    float radianToDegreeFloat(float rec);

    int radianToDegree(float rec);
};


#endif //BITSANDDROIDSCONNECTOR_OUTPUTCONVERTERS_H
