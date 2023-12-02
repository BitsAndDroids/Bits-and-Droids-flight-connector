//
// Created by DaveRiedel on 12-10-2022.
//

#include "ComBundle.h"

#include <thread>
#include <QDebug>

bool ComBundle::isOutputInBundle(int id){
    return this->outputs.contains(id);
}

ComBundle::ComBundle(QString port, bool delayBoot){
    this->port = new SerialPort(port.toStdString().c_str());
    if (delayBoot){
        //sleep for x seconds to allow for boards to reset (i.e. Arduino due)
        std::this_thread::sleep_for(std::chrono::seconds(3));
        qDebug() << "Sleeping for 3 seconds";
    }
}

QMap<int, Output*> ComBundle::getOutputs(){
    return outputs;
}
