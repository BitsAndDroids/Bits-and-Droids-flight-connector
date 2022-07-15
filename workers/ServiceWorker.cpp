//
// Created by dave- on 13-7-2022.
//

#include "ServiceWorker.h"
#include "logging/LogWindow.h"
HANDLE serviceSimconnect;

ServiceWorker::ServiceWorker() {
}

void ServiceWorker::startServices() {
    std::cout<<"open log window"<<std::endl;
    bool connected = false;


        while (!connected) {
            if (SUCCEEDED(SimConnect_Open(&serviceSimconnect, "incSimConnect", NULL, 0,
                                          0, 0))) {
                connected = true;


            }
            sleep(1);
        }
        SimConnect_Close(serviceSimconnect);
        startServices();


    }


void ServiceWorker::openLogWindow() {
    auto *logWindow = new LogWindow();
    std::cout<<"open log window"<<std::endl;
    logWindow->openWindow();
}

void ServiceWorker::MyDispatchProcRD(SIMCONNECT_RECV *pData, DWORD cbData, void *pContext) {

}

ServiceWorker::~ServiceWorker() {
    mutex.lock();

    condition.wakeOne();
    mutex.unlock();
    delete this;
}
