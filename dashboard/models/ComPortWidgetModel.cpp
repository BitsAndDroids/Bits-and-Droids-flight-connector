//
// Created by DaveRiedel on 19-9-2022.
//

#include <QSerialPortInfo>
#include "ComPortWidgetModel.h"

ComPortWidgetModel::ComPortWidgetModel() {
    availableSets = new QList<Set>();
    availableComPorts = QList<QString>();
    availableSets = setHandler.getSets();
}

void ComPortWidgetModel::refresh() {
    availableSets->clear();
    availableComPorts.clear();
    availableSets = setHandler.getSets();
    availableComPorts = loadAvailableComPorts();

}

void ComPortWidgetModel::addComPort(QString comPort) {
    availableComPorts.append(comPort);
}

void ComPortWidgetModel::clearComPortData() {
    availableComPorts.clear();
}

QList<QString> ComPortWidgetModel::loadAvailableComPorts() {
    QList<QString> comPorts;
    for (const QSerialPortInfo &serialPortInfo : QSerialPortInfo::availablePorts()) {
        comPorts.append(serialPortInfo.portName());
    }
    return comPorts;
}

