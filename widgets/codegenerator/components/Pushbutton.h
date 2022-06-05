//
// Created by Dave Riedel on 01-May-22.
//

#ifndef BITSANDDROIDSGUI_PUSHBUTTON_H
#define BITSANDDROIDSGUI_PUSHBUTTON_H


#include "Part.h"

class Pushbutton : public Part{
public:
    Pushbutton(std::string name);
private:
    bool latching = false;
    bool canConnectToDigital = true;
    bool canConnectToAnalog = true;
};


#endif //BITSANDDROIDSGUI_PUSHBUTTON_H
