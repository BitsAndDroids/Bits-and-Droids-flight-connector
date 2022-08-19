//
// Created by DaveRiedel on 17-8-2022.
//

#ifndef BITSANDDROIDSCONNECTOR_SETROWBUILDER_H
#define BITSANDDROIDSCONNECTOR_SETROWBUILDER_H


#include <QWidget>
#include <QList>
#include <QVBoxLayout>
#include <models/set.h>
#include "outputmenu/elements/Setrow.h"

class SetrowBuilder {
private:

    void buildSetrow(const Set set, QVBoxLayout* parent);
    QList<Setrow*> setRows = QList<Setrow*>();
public:
    SetrowBuilder();
    QWidget* buildSetrowContainer();



};


#endif //BITSANDDROIDSCONNECTOR_SETROWBUILDER_H
