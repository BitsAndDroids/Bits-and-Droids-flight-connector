//
// Created by dave- on 13-7-2022.
//

#include <ntstatus.h>

#include <utility>
#include "ServiceWorker.h"
#include "logging/LogWindow.h"

HANDLE serviceSimconnect;

ServiceWorker::ServiceWorker() = default;

SIMCONNECT_CLIENT_DATA_ID serviceLayerDataID = 3;
enum DATA_DEFINE_ID {
    DEFINITION_SERVICE_DATA = 14
};

void ServiceWorker::startServices() {

    bool connected = false;


    while (!connected) {
        if (SUCCEEDED(SimConnect_Open(&serviceSimconnect, "serviceSimconnect", nullptr, 0,
                                      nullptr, 0))) {

            connected = true;

            std::cout << "CONNECTED TO GAME" << std::endl;

            SimConnect_SubscribeToSystemEvent(serviceSimconnect, EVENT_SIM_START, "SimStart");

            SimConnect_MapClientDataNameToID(serviceSimconnect, "wasm.servicelayer", serviceLayerDataID);

            SimConnect_CreateClientData(serviceSimconnect, serviceLayerDataID, 4096,
                                        SIMCONNECT_CREATE_CLIENT_DATA_FLAG_DEFAULT);

            SimConnect_AddToClientDataDefinition(
                    serviceSimconnect, 14, SIMCONNECT_CLIENTDATAOFFSET_AUTO, 256, 0);

            SimConnect_AddToClientDataDefinition(serviceSimconnect, DEFINITION_SERVICE_DATA,
                                                 SIMCONNECT_CLIENTDATAOFFSET_AUTO,
                                                 256, 0);

            SimConnect_RequestClientData(serviceSimconnect,
                                         1,
                                         0,
                                         DEFINITION_SERVICE_DATA,
                                         SIMCONNECT_CLIENT_DATA_PERIOD_ON_SET,
                                         SIMCONNECT_CLIENT_DATA_REQUEST_FLAG_DEFAULT,
                                         0,
                                         0,
                                         0);


            connectionClosed = false;

            //SimConnect events return a STATUS_REMOTE_DISCONNECT error
            //if the user closes the simulator. This is a workaround.
            //We want to return to the main loop and try to reconnect.
            while (!connectionClosed) {
                SimConnect_CallDispatch(serviceSimconnect, MyDispatchProcRD, this);
            }
        }
        sleep(1);
    }
    std::cout << "DISCONNECTED FROM GAME" << std::endl;
    SimConnect_Close(serviceSimconnect);
    if (!stopServiceWorker) {
        sleep(100);
        startServices();
    }
}


void ServiceWorker::openLogWindow() {
    connect(&logger, &LoggerService::logReceived, logWindow, &LogWindow::addLogRow);
    logWindow->openWindow();
}

void ServiceWorker::MyDispatchProcRD(SIMCONNECT_RECV *pData, DWORD cbData, void *pContext) {
    auto *serviceWorker = static_cast<ServiceWorker *>(pContext);
    switch (pData->dwID) {
        case SIMCONNECT_RECV_ID_EVENT: {
            auto *evt = (SIMCONNECT_RECV_EVENT *) pData;

            switch (evt->uEventID) {

                case EVENT_SIM_START: {
                    SimConnect_SubscribeToSystemEvent(serviceSimconnect, EVENT_1_SECOND, "1sec");
                    break;
                }
                case EVENT_1_SECOND: {
                    break;
                }
                    break;
            }
            break;
        }
        case SIMCONNECT_RECV_ID_QUIT: {
            serviceWorker->setConnectionClosed(true);
            break;
        }
    }
}


ServiceWorker::~ServiceWorker() {
    mutex.lock();

    condition.wakeOne();
    mutex.unlock();
    delete this;
}

void ServiceWorker::sendWASMData(const char *data) {
    std::cout << "SENDING WASM DATA: " << data << std::endl;
    SimConnect_SetClientData(serviceSimconnect, serviceLayerDataID, DEFINITION_SERVICE_DATA,
                             SIMCONNECT_CLIENT_DATA_SET_FLAG_DEFAULT, 0, 256,
                             &data);

}

void ServiceWorker::setConnectionClosed(bool toSet) {
    connectionClosed = toSet;
}

void ServiceWorker::logMessage(std::string message, LogLevel level) {
    std::cout<<"LOG MESSAGE: "<<message<<std::endl;
    switch (level) {
        case LogLevel::WARNINGLOG:
            logger.logWarning(message);
            break;
        case LogLevel::DEBUGLOG:
            logger.logDebug(message);
            break;
        case LogLevel::ERRORLOG:
            logger.logError(message);
            break;
    }
}

