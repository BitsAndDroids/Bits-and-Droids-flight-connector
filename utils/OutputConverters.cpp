//
// Created by DaveRiedel on 15-Jun-22.
//

#include <QString>
#include "OutputConverters.h"
#include "enums/ModeEnum.h"

OutputConverters::OutputConverters() = default;

int OutputConverters::radianToDegree(float rec) {
    double pi = 3.14159;
    double radian = rec;
    return (radian * (180 / pi));
}

std::string OutputConverters::formatOutgoingString(float received, Output output){
    int intVal = 0;
    std::string prefix = std::to_string(output.getPrefix());
    //Ensure the prefix is 4 characters long
    for (int i = (int)prefix.size(); i < 4; i++) {
        prefix += " ";
    }

    std::string input_string;

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


