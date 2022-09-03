//
// Created by DaveRiedel on 18-8-2022.
//

#ifndef BITSANDDROIDSCONNECTOR_SETDETAILS_H
#define BITSANDDROIDSCONNECTOR_SETDETAILS_H


#include <QWidget>
#include "models/commands/Set.h"

class SetDetails: public QWidget{
    Q_OBJECT

private:
    Set* selectedSet;
public:
    SetDetails(QWidget *parent = nullptr);
    SetDetails* generateSetDetails( Set setToShow);
    struct QScrollArea * generateGridLayout();
};


#endif //BITSANDDROIDSCONNECTOR_SETDETAILS_H
