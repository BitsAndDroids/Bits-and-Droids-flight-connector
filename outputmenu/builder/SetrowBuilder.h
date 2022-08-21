//
// Created by DaveRiedel on 17-8-2022.
//

#ifndef BITSANDDROIDSCONNECTOR_SETROWBUILDER_H
#define BITSANDDROIDSCONNECTOR_SETROWBUILDER_H


#include <QWidget>
#include <QList>
#include <QVBoxLayout>
#include <models/set.h>
#include "outputmenu/elements/SetRow.h"

class SetrowBuilder {
private:

    void buildSetrow(const Set set, QVBoxLayout* parent);
    QList<SetRow*> setRows = QList<SetRow*>();
    OutputMenu *parent;
public:
    SetrowBuilder(OutputMenu *parent);
    QWidget* buildSetrowContainer();

public slots:
    void showSetDetails(QString id);
};


#endif //BITSANDDROIDSCONNECTOR_SETROWBUILDER_H
