//
// Created by Dave Riedel on 31-Mar-22.
//

#include "EventType.h"

QStringList EventType::availableTypes = {"Input", "Input with value", "Axis", "Output"};
QList<int> EventType::eventCodes = {0,1,9,3};


EventType::EventType(int index) {
    this->typeIndex = index;
    this->typeString = availableTypes.at(index).toStdString();
}
