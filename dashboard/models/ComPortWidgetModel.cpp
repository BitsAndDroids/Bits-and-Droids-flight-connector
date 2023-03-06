//
// Created by DaveRiedel on 19-9-2022.
//

#include <QSerialPortInfo>
#include "ComPortWidgetModel.h"
#include "logging/Logger.h"

ComPortWidgetModel::ComPortWidgetModel() {
    availableComPorts = loadAvailableComPorts();
    availableSets = setHandler->getSets();
}

void ComPortWidgetModel::refresh() {
    Logger::getInstance()->logDebug("Refreshing ports");
    availableSets->clear();
    availableComPorts->clear();
    availableSets = setHandler->getSets();
    availableComPorts = loadAvailableComPorts();

}

void ComPortWidgetModel::addComPort(QString comPort) {
    availableComPorts->append(comPort);
}

void ComPortWidgetModel::clearComPortData() {
    if(!availableComPorts->empty()){
        availableComPorts->clear();
    }
}

QList<QString> *ComPortWidgetModel::loadAvailableComPorts() {
    auto *comPorts = new QList<QString>();
    for (const QSerialPortInfo &serialPortInfo : QSerialPortInfo::availablePorts()) {
        Logger::getInstance()->logDebug("Found com port: " + serialPortInfo.portName().toStdString() +
                                        " | " + serialPortInfo.description().toStdString());
        comPorts->append(serialPortInfo.portName() + " | " + serialPortInfo.description());
    }
    return comPorts;
}

