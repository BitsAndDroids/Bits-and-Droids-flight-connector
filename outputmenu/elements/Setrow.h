//
// Created by DaveRiedel on 17-8-2022.
//

#ifndef BITSANDDROIDSCONNECTOR_SETROW_H
#define BITSANDDROIDSCONNECTOR_SETROW_H


#include <QWidget>
#include "models/commands/Set.h"
#include "outputmenu/outputmenu.h"

class Setrow : public QWidget {
Q_OBJECT


private:
    SetHandler setHandler = SetHandler();
private slots:

    void deleteSet();

    void exportSet();

    void editSet();

public:
    explicit Setrow(const Set &setForRow);

signals:
    void editSetSignal(QString id);
};


#endif //BITSANDDROIDSCONNECTOR_SETROW_H
