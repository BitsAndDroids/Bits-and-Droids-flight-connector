//
// Created by DaveRiedel on 15-Jun-22.
//

#include <QString>
#include "OutputConverters.h"

OutputConverters::OutputConverters() {}
int OutputConverters::radianToDegree(float rec) {
    double pi = 3.14159;
    double radian = rec;
    return (radian * (180 / pi));
}

float OutputConverters::radianToDegreeFloat(float rec) {
    double pi = 3.14159;
    double radian = rec;
    return (radian * (180 / pi));
}
std::pair<int, std::string> OutputConverters::parseOutputString(float received, const std::string &prefix, int mode){
    int bufferSize = 0;

    int intVal;
    std::string prefixString = prefix;

    //To support more inputs changed from 100 - 1000
    //In order to support legacy outputs (<100) we prefix with 0
    if (stoi(prefix) < 1000) {
        prefixString += " ";
    }

    std::string input_string;

    if (mode != 4) {
        intVal = static_cast<int>(received);
    } else {
        if (received == 0) {
            intVal = 0;
        } else {
            intVal = 1;
        }
        input_string = prefixString + std::to_string(intVal);
    }

    if (mode == 3) {
        input_string = prefixString + std::to_string(received);
    } else {
        const auto value = intVal;
        input_string = prefixString + std::to_string(value);
    }

    std::pair<int, std::string> pairToSend(bufferSize,input_string);
    return pairToSend;
}

float OutputConverters::converOutgoingFloatValue(float toConvert, int mode){
                float converted;
                switch (mode) {
                    case 0: {
                        converted = toConvert;
                        break;
                    }
                    case 1: {
                        converted = toConvert * 100;
                        break;
                    }
                    case 2: {
                        converted = radianToDegree(toConvert);
                        break;
                    }
                    case 3: {
                        converted = toConvert;
                        break;
                    }
                    case 4: {
                        converted = toConvert;
                        break;
                    }
                    case 5:
                        converted = toConvert;
                        break;
                    case 6: {
                        int inHg = toConvert * 1000;
                        if (inHg % (inHg / 10) >= 5) {
                            inHg += 10;
                        }
                        converted = inHg;
                        break;
                    }
                    case 7: {
                        converted = toConvert * 1.94;
                        break;
                    }

                    case 8: {
                        converted = toConvert;
                        break;
                    }

                    default:
                        converted = toConvert;
                        break;
                }
    return converted;
    }

