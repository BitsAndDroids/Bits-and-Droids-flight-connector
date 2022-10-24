//
// Created by DaveRiedel on 12-10-2022.
//

#include "ComBundle.h"
bool ComBundle::isOutputInBundle(int id) {
    return this->outputs.contains(id);
}

ComBundle::ComBundle(QString port) {
    this->port = new SerialPort(port.toStdString().c_str());
}

QMap<int, Output *> ComBundle::getOutputs() {
    return outputs;
}
