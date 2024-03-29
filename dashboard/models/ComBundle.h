//
// Created by DaveRiedel on 12-10-2022.
//

#ifndef BITSANDDROIDSCONNECTOR_COMBUNDLE_H
#define BITSANDDROIDSCONNECTOR_COMBUNDLE_H

#include <utility>

#include "models/commands/output.h"
#include "serial/SerialPort.hpp"

class ComBundle{
public:
    ComBundle(QString port, bool delayBoot);
    void setOutputs(QMap<int, Output *> outputsToSet) { this->outputs = outputsToSet; };
    bool isOutputInBundle(int id);
    SerialPort *getSerialPort() { return this->port; };
    char& getReceivedStringAddress(){ return *receivedString;}
    QMap<int, Output *> getOutputs();

private:
    SerialPort *port;
    char receivedString[255];
    QMap<int, Output *> outputs =  QMap<int, Output *>();

};
#endif //BITSANDDROIDSCONNECTOR_COMBUNDLE_H
