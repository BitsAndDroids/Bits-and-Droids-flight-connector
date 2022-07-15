//
// Created by dave- on 13-7-2022.
//

#include <ntstatus.h>
#include "ServiceWorker.h"
#include "logging/LogWindow.h"

HANDLE serviceSimconnect;

ServiceWorker::ServiceWorker() = default;

void ServiceWorker::startServices() {

    bool connected = false;

    while (!connected) {
        if (SUCCEEDED(SimConnect_Open(&serviceSimconnect, "serviceSimconnect", nullptr, 0,
                                      nullptr, 0))) {
            connected = true;
            HRESULT hr;
            //SimConnect events return a STATUS_REMOTE_DISCONNECT error
            //if the user closes the simulator. This is a workaround.
            //We want to return to the main loop and try to reconnect.
            while (hr != STATUS_REMOTE_DISCONNECT) {
                hr = SimConnect_RequestResponseTimes(serviceSimconnect, 0, nullptr);
            }
        }
        sleep(1);
    }
    SimConnect_Close(serviceSimconnect);
    if (!stopServiceWorker) {
        sleep(100);
        startServices();
    }
}


void ServiceWorker::openLogWindow() {
    auto *logWindow = new LogWindow();
    std::cout << "open log window" << std::endl;
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
