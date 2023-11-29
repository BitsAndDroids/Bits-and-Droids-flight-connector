//
// Created by DaveRiedel on 12-10-2022.
//

#include "ComBundle.h"

#include <thread>

bool ComBundle::isOutputInBundle(int id){
    return this->outputs.contains(id);
}

ComBundle::ComBundle(QString port){
    this->port = new SerialPort(port.toStdString().c_str());
    if (this->port->getPortName().find("due") != std::string::npos){
        //sleep for x seconds to allow for due boards to reset
        std::this_thread::sleep_for(std::chrono::seconds(3));
    }
}

QMap<int, Output*> ComBundle::getOutputs(){
    return outputs;
}
