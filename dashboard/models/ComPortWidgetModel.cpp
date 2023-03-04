//
// Created by DaveRiedel on 19-9-2022.
//

#include <QSerialPortInfo>
#include "ComPortWidgetModel.h"
#include "services/LoggerService.h"

ComPortWidgetModel::ComPortWidgetModel() {
    availableComPorts = loadAvailableComPorts();
    availableSets = setHandler->getSets();
}

void ComPortWidgetModel::refresh() {
    LoggerService::getInstance()->logDebug("Refreshing ports");
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
        LoggerService::getInstance()->logDebug("Found com port: " + serialPortInfo.portName().toStdString() +
        " | " + serialPortInfo.description().toStdString());
        comPorts->append(serialPortInfo.portName() + " | " + serialPortInfo.description());
    }
    return comPorts;
}

