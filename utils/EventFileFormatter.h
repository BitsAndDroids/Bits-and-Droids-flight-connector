//
// Created by DaveRiedel on 27-8-2022.
//

#ifndef BITSANDDROIDSCONNECTOR_EVENTFILEFORMATTER_H
#define BITSANDDROIDSCONNECTOR_EVENTFILEFORMATTER_H


#include <QString>
#include "models/commands/output.h"

class EventFileFormatter {
private:

public:
    EventFileFormatter();
    QString outputToQString(Output output);
};


#endif //BITSANDDROIDSCONNECTOR_EVENTFILEFORMATTER_H
