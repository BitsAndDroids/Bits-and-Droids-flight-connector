//
// Created by DaveRiedel on 19-9-2022.
//

#ifndef BITSANDDROIDSCONNECTOR_COMPORTWIDGETCONTROLLER_H
#define BITSANDDROIDSCONNECTOR_COMPORTWIDGETCONTROLLER_H


#include "dashboard/models/ComPortWidgetModel.h"
#include "elements/ModeIndexCombobox.h"
#include "enums/ModeEnum.h"
#include "workers/dualworker.h"

class ComPortWidgetController : public QObject {
Q_OBJECT
public:
    explicit ComPortWidgetController(QWidget *parent);

signals:
    void boardConnectionMade(int state);
    void gameConnectionMade(int state);
public slots:

    void start();

    void stop();

    void add();

    void refresh();

    void removeComPortRow();


private:
    void loadComPortData();

    void stopInput();

    void stopDual();

    void stopOutput();

    int getComboxIndex(ModeIndexCombobox *comboBox, const QString &value);

    void refreshComs();

    void startDual(bool autoStart);
    ComPortWidgetModel *comPortModel;
    DualWorker dualWorker = DualWorker();
    QWidget *parent;
    SettingsHandler settingsHandler = SettingsHandler();


    bool checkIfComboIsEmpty(const QList<ModeIndexCombobox *> &toCheck);
};


#endif //BITSANDDROIDSCONNECTOR_COMPORTWIDGETCONTROLLER_H
