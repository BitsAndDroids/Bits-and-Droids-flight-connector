//
// Created by DaveRiedel on 19-9-2022.
//

#ifndef BITSANDDROIDSCONNECTOR_COMPORTWIDGETCONTROLLER_H
#define BITSANDDROIDSCONNECTOR_COMPORTWIDGETCONTROLLER_H


#include "dashboard/models/ComPortWidgetModel.h"
#include "elements/ModeIndexCombobox.h"
#include "enums/ModeEnum.h"
#include "dashboard/workers/MFSWorker.h"
#include "elements/ModeIndexCheckbox.h"
#include "services/ServiceWorker.h"
#include <QLabel>

#include "dashboard/models/Preset.h"

class ComPortWidgetController : public QObject {
Q_OBJECT
public:
    explicit ComPortWidgetController(QWidget *parent, ServiceWorker *serviceWorker);

    void initComRows();

signals:
    void boardConnectionMade(int state);
    void gameConnectionMade(int state);
public slots:

    void start();

    void stop();

    void add();

    void refresh();

    void removeComPortRow();

    void updatePreset(Preset preset);


private:
    void loadComPortData();

    bool running = false;

    void toggleStartStopButtonState();

    static void clearChildrenFromLayout(QLayout *toClear);

    QList<ModeIndexCheckbox *> getCheckboxesByPattern(const QRegularExpression &pattern);

    QLabel *returnWarningString(int warningType);

    int getComboxIndex(ModeIndexCombobox *comboBox, const QString &value);

    void restoreStoredValuesComboBoxes();

    void refreshComs();

    void addComRow();
    uint8_t rows = 0;
    QString convertComPort(const QString& comText);
    ServiceWorker *serviceWorker;
    ComPortWidgetModel *comPortModel = new ComPortWidgetModel();
    MFSWorker dualWorker = MFSWorker();
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
