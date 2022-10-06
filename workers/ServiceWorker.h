//
// Created by dave- on 13-7-2022.
//

#ifndef BITSANDDROIDSCONNECTOR_SERVICEWORKER_H
#define BITSANDDROIDSCONNECTOR_SERVICEWORKER_H

#include <QThread>
#include <QMutex>
#include <QWaitCondition>
#include "models/SimConnect.h"
#include "logging/Logger.h"
#include "services/LoggerService.h"
#include "logging/LogWindow.h"

class ServiceWorker : public QThread {
Q_OBJECT

    void run() override { startServices(); }

private:
    static void MyDispatchProcRD(SIMCONNECT_RECV *pData, DWORD cbData,
                                 void *pContext);
    void startServices();

    bool connectionClosed = true;
    enum EVENT_ID {
        EVENT_SIM_START,
        EVENT_1_SECOND
    };
    LoggerService* logger = new LoggerService();


public:
    ServiceWorker();

    ~ServiceWorker() override;

    LoggerService *getLoggerService();

    void setConnectionClosed(bool);
    void setStopServiceWorker(bool state);

    QMutex mutex;
    QWaitCondition condition;
    bool stopServiceWorker = false;

signals:
    void gameConnectionMade(int state);

public slots:

    void logMessage(const std::string& message, LogLevel level);

    void sendWASMData(const char *data);
};


#endif //BITSANDDROIDSCONNECTOR_SERVICEWORKER_H
