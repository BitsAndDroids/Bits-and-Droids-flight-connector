//
// Created by dave- on 19-9-2022.
//

#ifndef BITSANDDROIDSCONNECTOR_DASHBOARDCONTROLLER_H
#define BITSANDDROIDSCONNECTOR_DASHBOARDCONTROLLER_H


#include <QObject>
#include <QMainWindow>
#include "handlers/pathhandler.h"
#include "logging/MessageCaster.h"
#include "workers/dualworker.h"
#include "workers/ServiceWorker.h"
#include "elements/ModeIndexCombobox.h"
#include "elements/ModeIndexCheckbox.h"

class DashboardController : public QObject {
Q_OBJECT
public:
    DashboardController(QMainWindow *parent);
signals:
    void sendWASMCommand(const char *data);

    void gameConnectionMade(int state);

    void boardConnectionMade(int state);
private:
    PathHandler pathHandler = PathHandler();
    MessageCaster messageCaster = MessageCaster();
    QMainWindow *parent;
    ServiceWorker serviceWorker = ServiceWorker();
    bool checkIfComboIsEmpty(const QList<ModeIndexCombobox *> &toCheck);
private slots:
    void updateEventFile();

    QList<ModeIndexCheckbox *> getCheckboxesByPattern(const QRegularExpression &pattern);

    void checkForUpdates(bool silentCheck);
};


#endif //BITSANDDROIDSCONNECTOR_DASHBOARDCONTROLLER_H
