//
// Created by DaveRiedel on 19-9-2022.
//

#ifndef BITSANDDROIDSCONNECTOR_COMPORTWIDGET_H
#define BITSANDDROIDSCONNECTOR_COMPORTWIDGET_H


#include <QWidget>
#include "elements/Element.h"

class ComportWidget: public QWidget, public Element {
    Q_OBJECT
public:
    QWidget* generateElement() override;
    ComportWidget();
private:

};


#endif //BITSANDDROIDSCONNECTOR_COMPORTWIDGET_H
