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
    explicit ComPortWidgetController();

signals:

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

    void refreshComs(int mode);

    ComPortWidgetModel *comPortModel;
    DualWorker dualWorker = DualWorker();

    SettingsHandler settingsHandler = SettingsHandler();


};


#endif //BITSANDDROIDSCONNECTOR_COMPORTWIDGETCONTROLLER_H
