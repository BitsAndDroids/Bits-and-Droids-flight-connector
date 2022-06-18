//
// Created by DaveRiedel on 18-Jun-22.
//

#ifndef BITSANDDROIDSCONNECTOR_INPUT_H
#define BITSANDDROIDSCONNECTOR_INPUT_H

#include "string"
#include "enums/inputenum.h"

class Input {
private:
    int type;
    std::string event;
    int prefix;

public:
    Input();
    Input(int type, std::string event, int prefix);

    int getType() const;

    const std::string &getEvent() const;

    int getPrefix() const;

};


#endif //BITSANDDROIDSCONNECTOR_INPUT_H
