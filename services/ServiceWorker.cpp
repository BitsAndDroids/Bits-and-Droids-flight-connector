//
// Created by dave- on 13-7-2022.
//

#include "ServiceWorker.h"
#include "logging/Logger.h"
#include "settings/ServiceSettingsHandler.h"
#include "dashboard/handlers/WASMHandler.h"
#include "thread"
#include <ctime>

HANDLE serviceSimconnect;

ServiceWorker::ServiceWorker() = default;

SIMCONNECT_CLIENT_DATA_ID serviceLayerDataID = 3;
enum DATA_DEFINE_ID {
    DEFINITION_SERVICE_DATA = 14
};
void ServiceWorker::setStopServiceWorker(bool state) {
    stopServiceWorker = state;
}
void ServiceWorker::startServices() {

    bool connected = false;
    try {
        connect(this, &ServiceWorker::updateEventFile, &wasmHandler, &WASMHandler::updateEventFile);
        connect(&wasmHandler, &WASMHandler::sendWASMCommand, this,
                &ServiceWorker::sendWASMData);
        wasmFound = wasmHandler.isWASMModuleInstalled();
        if (!wasmFound) {
            logMessage("WASM module not found, please install the module", LogLevel::ERRORLOG);
            emit wasmConnectionMade(0);
        }


        while (!stopServiceWorker) {
            logMessage("Service worker trying to connect to Simconnect", LogLevel::DEBUGLOG);
            emit gameConnectionMade(1);
            if (SUCCEEDED(SimConnect_Open(&serviceSimconnect, "serviceSimconnect", nullptr, 0,
                                          nullptr, 0))) {

                connected = true;
                logMessage("Service worker connected to Simconnect", LogLevel::DEBUGLOG);
                emit gameConnectionMade(2);

                SimConnect_MapClientDataNameToID(serviceSimconnect, "wasm.servicelayer", serviceLayerDataID);

                SimConnect_CreateClientData(serviceSimconnect, serviceLayerDataID, 4096,
                                            SIMCONNECT_CREATE_CLIENT_DATA_FLAG_DEFAULT);

                SimConnect_AddToClientDataDefinition(serviceSimconnect, DEFINITION_SERVICE_DATA,
                                                     SIMCONNECT_CLIENTDATAOFFSET_AUTO,
                                                     256, 0);

                connectionClosed = false;
                SimConnect_SubscribeToSystemEvent(serviceSimconnect, EVENT_SIM_START, "SimStart");
                SimConnect_SubscribeToSystemEvent(serviceSimconnect, EVENT_1_SECOND, "1sec");
                SimConnect_RequestClientData(serviceSimconnect,
                                             3,
                                             0,
                                             DEFINITION_SERVICE_DATA,
                                             SIMCONNECT_CLIENT_DATA_PERIOD_ON_SET,
                                             SIMCONNECT_CLIENT_DATA_REQUEST_FLAG_DEFAULT,
                                             0,
                                             0,
                                             0);

                long currentTicks = 0;
                while (!connectionClosed && !stopServiceWorker) {
                    currentTicks++;
                    if (currentTicks > 100) {
                        if (wasmFound && !wasmConnected) {
                            sendWASMData("ping");
                            emit wasmConnectionMade(1);
                        }
                        currentTicks = 0;
                    }
                    SimConnect_CallDispatch(serviceSimconnect, MyDispatchProcRD, this);
                    std::this_thread::sleep_for(std::chrono::milliseconds(2));
                }
            }
            //TODO replace by proper timer mechanism that can be interrupted
            //This check ensures that we don't wait for the app to close when we want it to close
            if (!stopServiceWorker) {
                uint8_t counter = 0;
                while (counter < 10 && !stopServiceWorker) {
                    std::this_thread::sleep_for(std::chrono::seconds(1));
                    counter++;
                }
            }
        }
        logMessage("ServiceWorker disconnected from game", LogLevel::DEBUGLOG);

        SimConnect_Close(serviceSimconnect);
        QThread::currentThread()->quit();
    }
    catch (std::exception &e) {
        logMessage("ServiceWorker exception: " + std::string(e.what()), LogLevel::ERRORLOG);
    }
}

const uint8_t POLLRATESECONDS = 2;
clock_t lastPoll = clock();

void ServiceWorker::pollUnconnectedDevices() {
    //time based polling based on POLLRATESECONDS to prevent spamming the system
    clock_t now = clock();

    if (now - lastPoll > POLLRATESECONDS * CLOCKS_PER_SEC) {
        lastPoll = now;
        for (auto &serialPort: this->unconnectedDevices) {
            if (serialPort->isConnected()) {
                //TODO emit signal to update workerthread
                Logger::getInstance()->logDebug("Serial port connected: " + serialPort->getPortName());
                this->unconnectedDevices.erase(
                        std::remove(this->unconnectedDevices.begin(), this->unconnectedDevices.end(), serialPort),
                        this->unconnectedDevices.end());
            }
        }
    }
}

void ServiceWorker::MyDispatchProcRD(SIMCONNECT_RECV *pData, DWORD cbData, void *pContext) {
    auto *serviceWorker = static_cast<ServiceWorker *>(pContext);
    switch (pData->dwID) {
        case SIMCONNECT_RECV_ID_CLIENT_DATA:
        {
            auto* pObjData = (SIMCONNECT_RECV_CLIENT_DATA*)(pData);
            std::string stringReceived = (char*)&pObjData->dwData;
            if(stringReceived == "sping"){
                serviceWorker->wasmConnected = true;
                emit serviceWorker->wasmConnectionMade(2);
            }

        }
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
        //TODO HANDLE SIMCONNECT_RECV_ID_QUIT TO ALTER GAME CONNECTION STATE
        case SIMCONNECT_RECV_ID_SIMOBJECT_DATA: {
            auto *pObjData = (SIMCONNECT_RECV_SIMOBJECT_DATA *) pData;
            switch (pObjData->dwRequestID) {
                case SIMCONNECT_RECV_ID_QUIT: {
                    serviceWorker->setConnectionClosed(true);
                    break;
                }
            }
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
    //prefix the command with s to identify service commands
    toSend[0] = 's';
    for(int i = 1; i < strlen(data) + 1; i++) {
        toSend[i] = data[i-1];
    }
    SimConnect_SetClientData(serviceSimconnect, serviceLayerDataID, DEFINITION_SERVICE_DATA,
                             SIMCONNECT_CLIENT_DATA_SET_FLAG_DEFAULT, 0, 256,
                             &toSend);
    logMessage("Sending " + std::string(data) + " to WASM module", LogLevel::DEBUGLOG);
}

void ServiceWorker::setConnectionClosed(bool toSet) {
    connectionClosed = toSet;
}

void ServiceWorker::logMessage(const std::string& message, LogLevel level) {
    switch (level) {
        case LogLevel::WARNINGLOG:
            logger->logWarning(message);
            break;
        case LogLevel::DEBUGLOG:
            logger->logDebug(message);
            break;
        case LogLevel::ERRORLOG:
            logger->logError(message);
            break;
    }
}

Logger *ServiceWorker::getLoggerService() {
    return this->logger;
}

