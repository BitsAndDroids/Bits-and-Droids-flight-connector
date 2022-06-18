//
// Created by DaveRiedel on 15-Jun-22.
//

#ifndef BITSANDDROIDSCONNECTOR_OUTPUTCONVERTERS_H
#define BITSANDDROIDSCONNECTOR_OUTPUTCONVERTERS_H


#include <utility>
#include <string>

class OutputConverters {
public:
    OutputConverters();
    std::pair<int, std::string> parseOutputString(float received, const std::string &prefix, int mode);
    float converOutgoingFloatValue(float toConvert, int mode);
private:


    int radianToDegree(double rec);

    float radianToDegreeFloat(double rec);

    float radianToDegreeFloat(float rec);

    int radianToDegree(float rec);
};


#endif //BITSANDDROIDSCONNECTOR_OUTPUTCONVERTERS_H
