//
// Created by dave- on 13-7-2022.
//

#ifndef BITSANDDROIDSCONNECTOR_SERVICEWORKER_H
#define BITSANDDROIDSCONNECTOR_SERVICEWORKER_H

#include <QThread>
#include <QMutex>
#include <QWaitCondition>
#include "models/SimConnect.h"

class ServiceWorker: public QThread {
    Q_OBJECT
    void run() override { startServices(); }
private:
    static void MyDispatchProcRD(SIMCONNECT_RECV* pData, DWORD cbData,
                                 void* pContext);
    void startServices();
public:
    ServiceWorker();
    ~ServiceWorker();
    QMutex mutex;
    QWaitCondition condition;


};


#endif //BITSANDDROIDSCONNECTOR_SERVICEWORKER_H
