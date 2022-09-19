//
// Created by DaveRiedel on 19-9-2022.
//

#include "ComPortWidgetModel.h"

ComPortWidgetModel::ComPortWidgetModel() {
    availableSets = new QList<Set>();
    availableComPorts = QList<QString>();
    availableSets = setHandler.getSets();
}

void ComPortWidgetModel::addComPort(QString comPort) {
    availableComPorts.append(comPort);
}

void ComPortWidgetModel::clearComPortData() {
    availableComPorts.clear();
}
