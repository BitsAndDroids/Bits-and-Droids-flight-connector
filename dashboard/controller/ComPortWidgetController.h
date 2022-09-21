//
// Created by DaveRiedel on 19-9-2022.
//

#ifndef BITSANDDROIDSCONNECTOR_COMPORTWIDGETCONTROLLER_H
#define BITSANDDROIDSCONNECTOR_COMPORTWIDGETCONTROLLER_H


#include "dashboard/models/ComPortWidgetModel.h"
#include "elements/ModeIndexCombobox.h"
#include "enums/ModeEnum.h"
#include "workers/dualworker.h"
#include "elements/ModeIndexCheckbox.h"
#include <QLabel>

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

    static void clearChildrenFromLayout(QLayout *toClear);

    QList<ModeIndexCheckbox *> getCheckboxesByPattern(const QRegularExpression &pattern);

    QLabel *returnWarningString(int warningType);

    int getComboxIndex(ModeIndexCombobox *comboBox, const QString &value);

    void restoreStoredValuesComboBoxes();

    void refreshComs();

    void addComRow();

    void startDual(bool autoStart);

    std::string convertComPort(QString comText);

    ComPortWidgetModel *comPortModel;
    DualWorker dualWorker = DualWorker();
    QWidget *parent;
    SetHandler setHandler = SetHandler();
    SettingsHandler settingsHandler = SettingsHandler();
    void saveAutoRunStates();
    enum warnings {
        NOSET, NOCOMPORT
    };


    bool checkIfComboIsEmpty(const QList<ModeIndexCombobox *> &toCheck);
};


#endif //BITSANDDROIDSCONNECTOR_COMPORTWIDGETCONTROLLER_H
