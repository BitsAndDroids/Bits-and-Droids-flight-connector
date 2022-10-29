//
// Created by DaveRiedel on 15-Jun-22.
//

#include <QString>
#include "OutputConverters.h"
#include "enums/ModeEnum.h"

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
std::string OutputConverters::formatOutgoingString(float received, Output output){
    int intVal = 0;
    std::string prefix = std::to_string(output.getPrefix());
    //Ensure the prefix is 4 characters long
    for (int i = prefix.size(); i < 4; i++) {
        prefix += " ";
    }

    std::string input_string = "";

    switch (output.getType()) {
        case BOOLMODE: {
            intVal = (received == 0) ? 0 : 1;
            input_string = prefix + std::to_string(intVal);
            break;
        }
        case 8:{

        }
        case INTEGERMODE: {
            intVal = (int)received;
            input_string = prefix + std::to_string(intVal);
            break;
        }
        case FLOATMODE: {
            input_string = prefix + std::to_string(received);
            break;
        }
        case PERCENTAGEMODE: {
            intVal = (int)(received * 100);
            input_string = prefix + std::to_string(intVal);
            break;
        }
        default: {
            intVal = (int)received;
            input_string = prefix + std::to_string(intVal);
            break;
        }
    }

    input_string = input_string + "\n";
    return input_string;
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

