//
// Created by DaveRiedel on 19-9-2022.
//

#ifndef BITSANDDROIDSCONNECTOR_COMPORTBUTTONROW_H
#define BITSANDDROIDSCONNECTOR_COMPORTBUTTONROW_H


#include <QWidget>
#include <elements/Element.h>
#include "dashboard/controller/ComPortWidgetController.h"

class ComPortButtonRow : public QWidget, public Element {
Q_OBJECT
public:
    QWidget *generateElement() override;

    ComPortButtonRow(ComPortWidgetController *controller,int mode);

private:
    int mode;
    QString headerText;
    ComPortWidgetController *controller;

};


#endif //BITSANDDROIDSCONNECTOR_COMPORTBUTTONROW_H
