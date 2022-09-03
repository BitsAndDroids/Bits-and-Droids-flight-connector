//
// Created by DaveRiedel on 17-8-2022.
//

#ifndef BITSANDDROIDSCONNECTOR_SETROW_H
#define BITSANDDROIDSCONNECTOR_SETROW_H


#include <QWidget>
#include "models/commands/Set.h"
#include "outputmenu/handlers/sethandler.h"

class SetRow : public QWidget {
Q_OBJECT
private:
    SetHandler setHandler = SetHandler();
private slots:

    void deleteSet();

    void exportSet();

    void editSet();
protected:
    void mousePressEvent(QMouseEvent*);

public:
    explicit SetRow(const Set &setForRow, QWidget *parent = nullptr);

signals:
    void editSetSignal(QString id);

    void clickedSignal(QString id);

    void deleteSetSignal(QString id);
};


#endif //BITSANDDROIDSCONNECTOR_SETROW_H
