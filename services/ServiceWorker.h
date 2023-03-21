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
#include "logging/Logger.h"
#include "logging/LogWindow.h"
#include "dashboard/handlers/WASMHandler.h"
#include "serial/SerialPort.hpp"

class ServiceWorker : public QThread {
Q_OBJECT



private:
    static void MyDispatchProcRD(SIMCONNECT_RECV *pData, DWORD cbData,
                                 void *pContext);
    void startServices();

    bool connectionClosed = true;
    enum EVENT_ID {
        EVENT_SIM_START,
        EVENT_1_SECOND
    };
    Logger* logger = Logger::getInstance();

    bool wasmConnected = false;
    bool wasmFound = false;

    WASMHandler wasmHandler = WASMHandler();

    std::vector<SerialPort*> unconnectedDevices = std::vector<SerialPort*>();
public:
    void run() override { startServices(); }
    ServiceWorker();

    ~ServiceWorker() override;

    Logger *getLoggerService();

    void setConnectionClosed(bool);
    void setStopServiceWorker(bool state);

    QMutex mutex;
    QWaitCondition condition;
    bool stopServiceWorker = false;

signals:
    void gameConnectionMade(int state);

    void wasmConnectionMade(int state);

    void updateEventFile();

public slots:

    void logMessage(const std::string& message, LogLevel level);

    void sendWASMData(const char *data);

    void pollUnconnectedDevices();
};


#endif //BITSANDDROIDSCONNECTOR_SERVICEWORKER_H
