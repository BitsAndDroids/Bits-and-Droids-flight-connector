//
// Created by DaveRiedel on 19-9-2022.
//

#ifndef BITSANDDROIDSCONNECTOR_COMPORTWIDGET_H
#define BITSANDDROIDSCONNECTOR_COMPORTWIDGET_H


#include <QWidget>
#include <QMainWindow>
#include "elements/Element.h"
#include "dashboard/controller/ComPortWidgetController.h"

class ComPortWidget: public QWidget, public Element {
    Q_OBJECT
public:
    QWidget* generateElement() override;
    ComPortWidget(QMainWindow *parent, ComPortWidgetController *controller);
private:
    QMainWindow *parent;
    ComPortWidgetController *controller;
};


#endif //BITSANDDROIDSCONNECTOR_COMPORTWIDGET_H
