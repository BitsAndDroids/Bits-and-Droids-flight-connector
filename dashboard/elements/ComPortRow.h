//
// Created by DaveRiedel on 19-9-2022.
//

#ifndef BITSANDDROIDSCONNECTOR_COMPORTROW_H
#define BITSANDDROIDSCONNECTOR_COMPORTROW_H


#include <QWidget>
#include "elements/Element.h"
#include "dashboard/controller/ComPortWidgetController.h"

class ComPortRow : public QWidget, public Element {
    Q_OBJECT
public:
    QWidget* generateElement() override;
    ComPortRow(ComPortWidgetController *controller, int index);
private:
    bool setsNeeded = true;
    int index;
    ComPortWidgetController *controller;
};


#endif //BITSANDDROIDSCONNECTOR_COMPORTROW_H
