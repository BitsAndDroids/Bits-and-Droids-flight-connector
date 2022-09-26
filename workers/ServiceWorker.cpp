//
// Created by dave- on 13-7-2022.
//

#include <ntstatus.h>

#include <utility>
#include "ServiceWorker.h"
#include "logging/LogWindow.h"

HANDLE serviceSimconnect;

ServiceWorker::ServiceWorker(){
}

SIMCONNECT_CLIENT_DATA_ID serviceLayerDataID = 3;
enum DATA_DEFINE_ID {
    DEFINITION_SERVICE_DATA = 14
};
void ServiceWorker::setStopServiceWorker(bool state) {
    stopServiceWorker = state;
}
void ServiceWorker::startServices() {

    bool connected = false;


    while (!stopServiceWorker) {
        emit logMessage("Service worker trying to connect to Simconnect", LogLevel::DEBUGLOG);
        if (SUCCEEDED(SimConnect_Open(&serviceSimconnect, "serviceSimconnect", nullptr, 0,
                                      nullptr, 0))) {

            connected = true;
            emit logMessage("Service worker connected to Simconnect", LogLevel::DEBUGLOG);



            SimConnect_MapClientDataNameToID(serviceSimconnect, "wasm.servicelayer", serviceLayerDataID);

            SimConnect_CreateClientData(serviceSimconnect, serviceLayerDataID, 4096,
                                        SIMCONNECT_CREATE_CLIENT_DATA_FLAG_DEFAULT);

            SimConnect_AddToClientDataDefinition(serviceSimconnect, DEFINITION_SERVICE_DATA,
                                                 SIMCONNECT_CLIENTDATAOFFSET_AUTO,
                                                 256, 0);

            connectionClosed = false;
            SimConnect_SubscribeToSystemEvent(serviceSimconnect, EVENT_SIM_START, "SimStart");
            SimConnect_SubscribeToSystemEvent(serviceSimconnect, EVENT_1_SECOND, "1sec");
            while (!connectionClosed) {
                SimConnect_CallDispatch(serviceSimconnect, MyDispatchProcRD, this);
                std::this_thread::sleep_for(std::chrono::milliseconds(2));
            }
        }
        //TODO replace by proper timer mechanism that can be interrupted
        //This check ensures that we don't wait for the app to close when we want it to close
        if (!stopServiceWorker) {
            uint8_t counter = 0;
            while(counter < 10 && !stopServiceWorker) {
                std::this_thread::sleep_for(std::chrono::seconds(1));
                counter++;
            }
        }
    }
    emit logMessage("ServiceWorker disconnected from game", LogLevel::DEBUGLOG);
    SimConnect_Close(serviceSimconnect);
    QThread::currentThread()->quit();
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
    stopServiceWorker = true;
    mutex.lock();
    condition.wakeOne();
    mutex.unlock();
}

void ServiceWorker::sendWASMData(const char *data) {
    //The data size has te be pre-defined in the client data definition
    //data won't be read in the WASM module without this
    char toSend[256] = "";
    for(int i = 0; i < strlen(data); i++) {
        toSend[i] = data[i];
    }
    SimConnect_SetClientData(serviceSimconnect, serviceLayerDataID, DEFINITION_SERVICE_DATA,
                             SIMCONNECT_CLIENT_DATA_SET_FLAG_DEFAULT, 0, 256,
                             &toSend);
    emit logMessage("Sending " + std::string(data) + " to WASM module", LogLevel::DEBUGLOG);

}

void ServiceWorker::setConnectionClosed(bool toSet) {
    connectionClosed = toSet;
}

void ServiceWorker::logMessage(std::string message, LogLevel level) {

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

