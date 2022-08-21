//
// Created by DaveRiedel on 20-8-2022.
//

#ifndef BITSANDDROIDSCONNECTOR_OUTPUTDETAILSBUILDER_H
#define BITSANDDROIDSCONNECTOR_OUTPUTDETAILSBUILDER_H


#include <QWidget>
#include "models/commands/Set.h"

class OutputDetailsBuilder: public QWidget {
Q_OBJECT
private:
    Set setToShow;
public:
    OutputDetailsBuilder(QWidget *parent = nullptr);
public slots:
    QWidget* buildOutputDetailsContainer(QString id);


};


#endif //BITSANDDROIDSCONNECTOR_OUTPUTDETAILSBUILDER_H
