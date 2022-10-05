#include "dualworker.h"
#include "utils/OutputConverters.h"

#include <strsafe.h>
#include <windows.h>

#include <string>
#include <utility>

#define DATA_LENGTH 255
#define MAX_RETURNED_ITEMS 255

using namespace std;
SerialPort *dualPorts[10];

// Attempt at WASM

HANDLE dualSimConnect = nullptr;

struct StructOneDatum {
    int id;
    float value;
};
char receivedString[DATA_LENGTH];


float dualDataRecv = 1.2f;
enum GROUP_ID {
    GROUP0 = 2, GROUP_A = 1
};
enum INPUT_ID {
    INPUT0,
};
enum EVENT_ID {
    EVENT_SIM_START,
    EVENT_WASM = 2,
    EVENT_WASMINC = 3,
};
struct dataStr {
    float val;
};

enum DATA_DEFINE_ID {
    DEFINITION_PDR_RADIO,
    DEFINITION_STRING,
    DEFINITION_ELEVATOR_TRIM_PCT,
    DEFINITION_1 = 12,

};

struct StructDatum {
    StructOneDatum datum[MAX_RETURNED_ITEMS];
};

enum DATA_REQUEST_ID {
    REQUEST_PDR_RADIO,
    REQUEST_STRING,
    REQUEST_1 = 10,
};

void sendCommand(SIMCONNECT_CLIENT_EVENT_ID eventID) {
    SimConnect_TransmitClientEvent(dualSimConnect, SIMCONNECT_OBJECT_ID_USER,
                                   eventID, 0, SIMCONNECT_GROUP_PRIORITY_HIGHEST,
                                   SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
}

DualWorker::DualWorker() {
}

void sendDualToArduino(float received, const std::string &prefix, int index,
                       int mode) {
    int intVal;
    std::string prefixString = prefix;
    if (stoi(prefix) < 1000) {
        prefixString += " ";
    }
    std::string input_string;

    if (mode != 99) {
        intVal = static_cast<int>(received);
    } else {
        if (received == 0) {
            intVal = 0;
        } else {
            intVal = 1;
        }
        input_string = prefixString + std::to_string(intVal);
    }

    if (mode == 3 || mode == 97) {
        input_string = prefixString + std::to_string(received);

    } else {
        const auto value = intVal;
        input_string = prefixString + std::to_string(value);
    }

    auto *const c_string = new char[input_string.size() + 1];
    std::copy(input_string.begin(), input_string.end(), c_string);
    c_string[input_string.size()] = '\n';
    cout << strlen(c_string) << endl;

    if (mode == 1) {
        if (received < 0) {
            dualPorts[index]->writeSerialPort(c_string, 7);
        } else {
            dualPorts[index]->writeSerialPort(c_string, 6);
        }
    } else {
        dualPorts[index]->writeSerialPort(c_string, input_string.size() + 1);
    }

    input_string.clear();

    delete[] c_string;
}

void DualWorker::MyDispatchProcInput(SIMCONNECT_RECV *pData, DWORD cbData,
                                     void *pContext) {
    HRESULT hr;
    auto *dualCast = static_cast<DualWorker *>(pContext);

    switch (pData->dwID) {
        case SIMCONNECT_RECV_ID_EVENT: {
            auto *evt = (SIMCONNECT_RECV_EVENT *) pData;

            switch (evt->uEventID) {

                case EVENT_SIM_START: {

                    // Now the sim is running, request information on the user aircraft
//                    SimConnect_RequestDataOnSimObject(
//                            dualSimConnect, REQUEST_PDR_RADIO, DEFINITION_PDR_RADIO,
//                            SIMCONNECT_OBJECT_ID_USER, SIMCONNECT_PERIOD_ONCE,
//                            SIMCONNECT_DATA_REQUEST_FLAG_DEFAULT,
//                            0, 0);

                    SimConnect_RequestDataOnSimObject(
                            dualSimConnect, REQUEST_PDR_RADIO, DEFINITION_PDR_RADIO,
                            SIMCONNECT_OBJECT_ID_USER, SIMCONNECT_PERIOD_VISUAL_FRAME,
                            SIMCONNECT_DATA_REQUEST_FLAG_CHANGED | SIMCONNECT_DATA_REQUEST_FLAG_TAGGED);
                    break;
                }
                case EVENT_WASMINC: {
                    cout << "hit" << endl;
                    break;
                }
                default:
                    break;
            }
            break;
        }
        case SIMCONNECT_RECV_ID_CLIENT_DATA: {
            auto pObjData = (SIMCONNECT_RECV_CLIENT_DATA *) pData;
            int bundle = 0;

            Output *output =
                    dualCast->outputHandler.findOutputById(pObjData->dwRequestID);
            for (int i = 0; i < dualCast->outputBundles->size(); i++) {
                if (dualCast->outputBundles->at(i)->isOutputInBundle(output->getId())) {
                    qDebug() << "FOUND IN SET";
                    bundle = i;
                }
            }
            auto *data = (dataStr *) &pObjData->dwData;
            qDebug() << "DATA: " << pObjData->dwData << "ID: " << pObjData->dwID
                     << pObjData->dwRequestID << pObjData->dwDefineID
                     << pObjData->dwObjectID;

            if (pObjData->dwRequestID > 999 && pObjData->dwRequestID < 9999) {
                sendDualToArduino(
                        data->val, std::to_string(pObjData->dwRequestID), bundle,
                        dualCast->outputHandler.findOutputById(pObjData->dwRequestID)
                                ->getType());
            }
        }
            break;
        case SIMCONNECT_RECV_ID_SIMOBJECT_DATA: {
            cout << "hiyi" << endl;
            auto *pObjData = (SIMCONNECT_RECV_SIMOBJECT_DATA *) pData;

            switch (pObjData->dwRequestID) {
                case REQUEST_PDR_RADIO: {

                    cout << "RADIO" << endl;
                    int count = 0;
                    auto pS = reinterpret_cast<StructDatum *>(&pObjData->dwData);
                    cout << pObjData->dwDefineCount << " COUNT" << endl;
                    while (count < (int) pObjData->dwDefineCount) {
                        string valString = std::to_string(pS->datum[count].value);
                        int id = pS->datum[count].id;
                        Output *output = dualCast->outputHandler.findOutputById(id);
                        int bundle = 0;

                        for (int i = 0; i < dualCast->outputBundles->size(); i++) {
                            if (dualCast->outputBundles->at(i)->isOutputInBundle(
                                    output->getId())) {
                                bundle = i;
                            }
                        }

                        int mode = output->getType();
                        string prefix = std::to_string(output->getPrefix());
                        cout << prefix << endl;
                        float value = dualCast->converter.converOutgoingFloatValue(pS->datum[count].value, mode);
                        sendDualToArduino(value, prefix, bundle, mode);
                        std::this_thread::sleep_for(std::chrono::milliseconds(output->getDelay()));
                        count++;

                        emit dualCast->logMessage(
                                "Send data: " + std::to_string((int) value) + " | prefix " + prefix + " -> " +
                                dualPorts[bundle]->getPortName(), LogLevel::DEBUGLOG);
                    }
                    break;
                }
                case SIMCONNECT_RECV_ID_QUIT: {
                    // quit = 1;
                    dualCast->setConnected(false);
                    break;
                }
            }
        }
    }
}

void DualWorker::addBundle(outputBundle *bundle) {
    outputBundles->append(bundle);
}

void DualWorker::eventLoop() {
    HRESULT hr;
    keys = settingsHandler.retrieveKeys("runningDualComs");
    int keySize = (int) keys->size();
    int successfullyConnected = 0;
    for (int i = 0; i < keySize; i++) {
        dualPorts[i] = new SerialPort(
                settingsHandler.retrieveSetting("runningDualComs", keys->at(i))
                        ->toString()
                        .toStdString()
                        .c_str());

        if (dualPorts[i]->isConnected()) {
            cout << "CONNECTED" << endl;
            emit boardConnectionMade(1);
            successfullyConnected++;
        } else {
            cout << "NOT CONNECTED" << endl;
        }
    }
    if (successfullyConnected == keySize) {
        emit boardConnectionMade(2);
    }

    while (!connected && !abortDual) {
        emit logMessage("Attempt connecting to SimConnect", LogLevel::DEBUGLOG);
        if (SUCCEEDED(SimConnect_Open(&dualSimConnect, "dualConnect", nullptr, 0,
                                      nullptr, 0))) {

            emit logMessage("Connected to SimConnect", LogLevel::DEBUGLOG);
            connected = true;

            SimConnect_MapClientDataNameToID(dualSimConnect, "shared", ClientDataID);

            SimConnect_CreateClientData(dualSimConnect, ClientDataID, 256,
                                        SIMCONNECT_CLIENT_DATA_REQUEST_FLAG_CHANGED);


            SimConnect_SetClientData(dualSimConnect, ClientDataID, DEFINITION_1,
                                     SIMCONNECT_CLIENT_DATA_SET_FLAG_DEFAULT, 0, 256,
                                     &dataF);

            SimConnect_AddToClientDataDefinition(
                    dualSimConnect, 12, SIMCONNECT_CLIENTDATAOFFSET_AUTO, 256, 0);
            dualInputHandler = new InputSwitchHandler(inputs, dualSimConnect);
            connect(dualInputHandler, &InputSwitchHandler::logMessage, this, &DualWorker::logMessage);
            dualInputHandler->setRanges();
            dualInputHandler->object = SIMCONNECT_OBJECT_ID_USER;

            dualInputMapper.mapEvents(dualSimConnect);

            SimConnect_MapClientDataNameToID(dualSimConnect, "wasm.responses", 2);

            SimConnect_CreateClientData(dualSimConnect, 2, 4096,
                                        SIMCONNECT_CREATE_CLIENT_DATA_FLAG_DEFAULT);

            SimConnect_AddToClientDataDefinition(dualSimConnect, 0, 0,
                                                 sizeof(dualDataRecv), 0, 0);

            SimConnect_RequestClientData(dualSimConnect, ClientDataID, REQUEST_1,
                                         DEFINITION_1,
                                         SIMCONNECT_CLIENT_DATA_PERIOD_SECOND,
                                         SIMCONNECT_CLIENT_DATA_REQUEST_FLAG_DEFAULT);

            dualOutputMapper->mapOutputs(outputsToMap, dualSimConnect);
            SimConnect_SubscribeToSystemEvent(dualSimConnect, EVENT_SIM_START,
                                              "6Hz");

            SimConnect_RequestDataOnSimObject(
                    dualSimConnect, REQUEST_PDR_RADIO, DEFINITION_PDR_RADIO,
                    SIMCONNECT_OBJECT_ID_USER, SIMCONNECT_PERIOD_VISUAL_FRAME,
                    SIMCONNECT_DATA_REQUEST_FLAG_CHANGED |
                    SIMCONNECT_DATA_REQUEST_FLAG_TAGGED);

            while (!abortDual && connected) {
                SimConnect_CallDispatch(dualSimConnect, MyDispatchProcInput, this);

                for (int i = 0; i < keys->size(); i++) {
                    const auto hasRead = dualPorts[i]->readSerialPort(
                            dualInputHandler->receivedString[i], DATA_LENGTH);

                    if (hasRead) {
                        if (connected) {
                            dualInputHandler->switchHandling(i);
                        }
                    }
                }
                std::this_thread::sleep_for(std::chrono::milliseconds(1));
            }

            SimConnect_Close(dualSimConnect);
            connected = false;
        }
        //TODO replace by proper timer mechanism that can be interrupted
        //This check ensures that we don't wait for the app to close when we want it to close
        //
        if (!abortDual) {
            uint8_t counter = 0;
            while (counter < 10 && !abortDual) {
                std::this_thread::sleep_for(std::chrono::seconds(1));
                counter++;
            }
        }

    }

    for (int i = 0; i < keys->size(); i++) {
        if (dualPorts[i]->isConnected()) {
            dualPorts[i]->closeSerial();
        }
    }
    //We want to update the dashboard indicator when we've closed the connection
    emit boardConnectionMade(0);
    QThread::currentThread()->quit();
}

void DualWorker::clearBundles() { this->outputBundles->clear(); }

DualWorker::~DualWorker() {
    abortDual = true;
    for (int i = 0; i < keys->size(); i++) {
        if (dualPorts[i]->isConnected()) {
            cout << dualPorts[i]->getPortName() + " CLOSED" << endl;
            dualPorts[i]->closeSerial();
        }
    }
    mutex.lock();
    condition.wakeOne();
    mutex.unlock();

}

void DualWorker::setInputs(std::map<int, Input> inputsToSet) {
    this->inputs = std::move(inputsToSet);
}

void DualWorker::setConnected(bool connectedToSim) {
    this->connected = connectedToSim;
}
