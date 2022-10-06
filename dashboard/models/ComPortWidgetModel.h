//
// Created by DaveRiedel on 19-9-2022.
//

#ifndef BITSANDDROIDSCONNECTOR_COMPORTWIDGETMODEL_H
#define BITSANDDROIDSCONNECTOR_COMPORTWIDGETMODEL_H


#include "models/commands/Set.h"
#include "outputmenu/handlers/sethandler.h"

class ComPortWidgetModel {
public:
    ComPortWidgetModel();
    QList<Set> *getAvailableSets() { return availableSets; };
    QList<QString> *getAvailableComPorts() { return availableComPorts; };
    QList<QString> *loadAvailableComPorts();
    void refresh();
    void addComPort(QString comPort);
    void clearComPortData();

private:
    QList<Set> *availableSets = new QList<Set>();
    QList<QString> *availableComPorts = new QList<QString>();
    SetHandler *setHandler = new SetHandler();
};


#endif //BITSANDDROIDSCONNECTOR_COMPORTWIDGETMODEL_H
