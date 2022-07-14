//
// Created by dave- on 13-7-2022.
//

#ifndef BITSANDDROIDSCONNECTOR_SERVICEWORKER_H
#define BITSANDDROIDSCONNECTOR_SERVICEWORKER_H


class ServiceWorker: Public QThread {
    Q_OBJECT
    void run() override { startServices(); }
private:
    static void MyDispatchProcRD(SIMCONNECT_RECV* pData, DWORD cbData,
                                 void* pContext);
public:
    ServiceWorker();
    ~ServiceWorker();
    QMutex mutex;
    QWaitCondition condition;


};


#endif //BITSANDDROIDSCONNECTOR_SERVICEWORKER_H
