//
// Created by DaveRiedel on 17-8-2022.
//

#ifndef BITSANDDROIDSCONNECTOR_SETROWBUILDER_H
#define BITSANDDROIDSCONNECTOR_SETROWBUILDER_H


#include <QWidget>
#include <QList>
#include <QVBoxLayout>
#include "outputmenu/elements/SetRow.h"
#include <models/commands/Set.h>


class SetrowBuilder: public QWidget{
    Q_OBJECT
private:
    void buildSetrow(const Set& set);

    QList<SetRow *> setRows = QList<SetRow *>();

public:
    SetrowBuilder(QWidget *parent = nullptr);

    QWidget *buildSetrowContainer();
    void createSet(QString id);
signals:
    void showSetDetailsSignal(QString);
    void editSetSignal(QString);
    void deleteSetSignal(QString);
    void createSetSignal();

};


#endif //BITSANDDROIDSCONNECTOR_SETROWBUILDER_H
