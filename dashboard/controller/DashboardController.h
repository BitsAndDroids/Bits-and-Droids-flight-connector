//
// Created by dave- on 19-9-2022.
//

#ifndef BITSANDDROIDSCONNECTOR_DASHBOARDCONTROLLER_H
#define BITSANDDROIDSCONNECTOR_DASHBOARDCONTROLLER_H


#include <QObject>
#include <QMainWindow>
#include "handlers/pathhandler.h"
#include "logging/MessageCaster.h"
#include "dashboard/workers/MFSWorker.h"
#include "services/ServiceWorker.h"
#include "elements/ModeIndexCombobox.h"
#include "elements/ModeIndexCheckbox.h"

class DashboardController : public QObject {
Q_OBJECT
public:
    DashboardController(QMainWindow *parent);
    void setServiceWorker(ServiceWorker *serviceWorker);
    void initController();
signals:
    void sendWASMCommand(const char *data);

    void logMessage(const QString &message, const QString &type);

    void gameConnectionMade(int state);

    void boardConnectionMade(int state);

    void wasmConnectionMade(int state);

    void exitProgram();

    void updateAvailable();


private:
    PathHandler pathHandler = PathHandler();
    MessageCaster messageCaster = MessageCaster();
    QMainWindow *parent;
    QString applicationEventsPath = pathHandler.getWritableEventPath();
    ServiceWorker *serviceWorker = new ServiceWorker();
    void installWasm();
    bool checkIfComboIsEmpty(const QList<ModeIndexCombobox *> &toCheck);
private slots:
    void updateEventFile();

    QList<ModeIndexCheckbox *> getCheckboxesByPattern(const QRegularExpression &pattern);


};


#endif //BITSANDDROIDSCONNECTOR_DASHBOARDCONTROLLER_H
