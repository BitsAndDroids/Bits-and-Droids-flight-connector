#include "MFSWorker.h"

#include "outputmenu/handlers/sethandler.h"
#include "settings/ComSettingsHandler.h"
#include "utils/InputReader.h"

#include <windows.h>

#include <string>
#include <utility>

#define DATA_LENGTH 255
#define MAX_RETURNED_ITEMS 255

using namespace std;

HANDLE dualSimConnect = nullptr;

struct StructOneDatum {
    int id;
    float value;
};

float dualDataRecv = 1.2f;

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

/*!
  \class MFSWorker
  \ingroup MFS2020Interaction
  \brief MFSWorker is a class that handles the connection to Microsoft Flight Simulator
  This class handles the connection to Microsoft Flight Simulator and the data that is sent to and from the simulator.
  This class inherits from QThread and is run in a separate thread from the main thread.
 */
MFSWorker::MFSWorker() {
}

/*!
  \brief MFSWorker::sendToArduino is responsible for sending data to the Arduino
  The modes are defined in the ModeEnum class.

  @param received is the data received from the simulator
  @param prefix prefix to tag the data
  @param index index of the comport
  @param mode defines the data type

  \sa ModeEnum
 */
void MFSWorker::sendToArduino(const std::string &formatedString, int index) {
    comBundles->at(index)->getSerialPort()->writeSerialPort(formatedString.c_str(), formatedString.length());
    emit logMessage(
            "Send data: " + formatedString + " -> " +
            comBundles->at(index)->getSerialPort()->getPortName(),
            LogLevel::DEBUGLOG);
}


/*!
  \brief MFSWorker::MyDispatchProcInput handles the data received from the simulator

  This function monitors the SimConnect data. The function handles SimConnect events provided by the simulator.
  If new data is available, the function will send the data to the Microcontroller using the sendToArduino function.
  It also handles the data received from the WASM module.

  @param *pData
  @param cbData
  @param *pContext Pointer to the calling MFSWorker object. This can be used to reference the MFSWorker object.

  \sa MFSWorker::sendToArduino
 */
void MFSWorker::MyDispatchProcInput(SIMCONNECT_RECV *pData, DWORD cbData,
                                    void *pContext) {
    HRESULT hr;
    auto *dualCast = static_cast<MFSWorker *>(pContext);

    switch (pData->dwID) {
        case SIMCONNECT_RECV_ID_EVENT: {
            auto *evt = (SIMCONNECT_RECV_EVENT *) pData;

            switch (evt->uEventID) {

                case EVENT_SIM_START: {

                    // Now the sim is running, request information on the user aircraft
                    SimConnect_RequestDataOnSimObject(
                            dualSimConnect, REQUEST_PDR_RADIO, DEFINITION_PDR_RADIO,
                            SIMCONNECT_OBJECT_ID_USER, SIMCONNECT_PERIOD_ONCE,
                            SIMCONNECT_DATA_REQUEST_FLAG_DEFAULT,
                            0, 0);

                    SimConnect_RequestDataOnSimObject(
                            dualSimConnect, REQUEST_PDR_RADIO, DEFINITION_PDR_RADIO,
                            SIMCONNECT_OBJECT_ID_USER, SIMCONNECT_PERIOD_VISUAL_FRAME,
                            SIMCONNECT_DATA_REQUEST_FLAG_CHANGED | SIMCONNECT_DATA_REQUEST_FLAG_TAGGED);
                    break;
                }
                case EVENT_WASMINC: {
                    break;
                }
                default:
                    break;
            }
            break;
        }
        case SIMCONNECT_RECV_ID_CLIENT_DATA: {
            auto pObjData = (SIMCONNECT_RECV_CLIENT_DATA *) pData;

            Output *output =
                    dualCast->outputHandler.findOutputById((int) pObjData->dwRequestID);
            for (int i = 0; i < dualCast->comBundles->size(); i++) {
                if (dualCast->comBundles->at(i)->isOutputInBundle(output->getId())) {

                    auto *data = (dataStr *) &pObjData->dwData;
                    if (pObjData->dwRequestID > 999 && pObjData->dwRequestID < 9999) {
                        dualCast->sendToArduino(dualCast->converter.formatOutgoingString(data->val, *output), i);
                    }
                }
            }
        }
            break;
        case SIMCONNECT_RECV_ID_SIMOBJECT_DATA: {
            auto *pObjData = (SIMCONNECT_RECV_SIMOBJECT_DATA *) pData;

            switch (pObjData->dwRequestID) {
                case REQUEST_PDR_RADIO: {
                    int count = 0;
                    auto pS = reinterpret_cast<StructDatum *>(&pObjData->dwData);
                    while (count < (int) pObjData->dwDefineCount) {
                        int id = pS->datum[count].id;
                        Output *output = dualCast->outputHandler.findOutputById(id);

                        for (int i = 0; i < dualCast->comBundles->size(); i++) {
                            if (dualCast->comBundles->at(i)->isOutputInBundle(
                                    output->getId())) {
                                dualCast->sendToArduino(
                                        dualCast->converter.formatOutgoingString(pS->datum[count].value, *output), i);
                            }
                        }

                        std::this_thread::sleep_for(std::chrono::milliseconds(output->getDelay()));
                        count++;
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

/*!
 * \fn void MFSWorker::loadRunningPortsAndSets
 * \brief MFSWorker::loadRunningPortsAndSets initiates the ports and sets that are saved in the settings file
 * It loads in the coms and bundles them into a single ComBundle object.
 * After this the app loads in all the inputs from the dist/Inputs.json file
 * \sa ComBundle
 */
void MFSWorker::loadRunningPortsAndSets() {
    comBundles->clear();
    auto comSettingsHandler = ComSettingsHandler();
    QList<QPair<QString, int>> comSettings = comSettingsHandler.loadComs();
    auto setHandler = SetHandler();

    int successfullyConnected = 0;
    setHandler.updateSets();
    setHandler.loadSets();
    for (const auto &comSetting: comSettings) {
        auto *bundle = new ComBundle(comSetting.first);
        if (comSetting.second != -1) {
            std::cout<<"Loading set: "<<comSetting.second<<std::endl;
            auto set = setHandler.getSetById(QString::number(comSetting.second));
            auto outputs = set.getOutputs();
            bundle->setOutputs(outputs);
        }
        if (bundle->getSerialPort()->isConnected()) {
            emit boardConnectionMade(1);
            emit logMessage("Connected to " + comSetting.first.toStdString(), LogLevel::DEBUGLOG);
            successfullyConnected++;
        } else {
            emit logMessage("Can't connect to " + comSetting.first.toStdString(), LogLevel::WARNINGLOG);
        }
        comBundles->append(bundle);
        for (auto &i: bundle->getOutputs()) {
            i = outputHandler.findOutputById(i->getId());
            outputsToMap.append(i);
        }
    }

    if (successfullyConnected == comBundles->size()) {
        emit boardConnectionMade(2);
    }

    InputReader inputReader = InputReader();
    inputReader.readInputs();
    this->inputs = inputReader.getInputs();

}

/*!
 * \fn void MFSWorker::eventLoop()
 * \brief MFSWorker::eventLoop is the main loop of the application.
 * It directs the connection to the sim and the microcontrollers
 * This loop is started from the Dashboard using the StartButton.
 * /sa Dashboard
 */
void MFSWorker::eventLoop() {
    HRESULT hr;

    loadRunningPortsAndSets();

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
            connect(dualInputHandler, &InputSwitchHandler::logMessage, this, &MFSWorker::logMessage);

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

                for (auto comBundle: *comBundles) {
                    const auto hasRead = comBundle->getSerialPort()->readSerialPort(
                            &comBundle->getReceivedStringAddress(), DATA_LENGTH);

                    if (hasRead) {
                        if (connected) {
                            dualInputHandler->switchHandling(&comBundle->getReceivedStringAddress());
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
        if (!abortDual) {
            uint8_t counter = 0;
            while (counter < 10 && !abortDual) {
                std::this_thread::sleep_for(std::chrono::seconds(1));
                counter++;
            }
        }
    }

    for (auto com: *comBundles) {
        if (com->getSerialPort()->isConnected()) {
            com->getSerialPort()->closeSerial();
        }
    }

    //We want to update the dashboard indicator when we've closed the connection
    emit boardConnectionMade(0);
    QThread::currentThread()->quit();
}

void MFSWorker::setInputs(std::map<int, Input> inputsToSet) {
    this->inputs = std::move(inputsToSet);
}

void MFSWorker::setConnected(bool connectedToSim) {
    this->connected = connectedToSim;
}

MFSWorker::~MFSWorker() {
    abortDual = true;
    for (auto com: *comBundles) {
        if (com->getSerialPort()->isConnected()) {
            emit logMessage("Closing connection to " + com->getSerialPort()->getPortName(),
                            LogLevel::DEBUGLOG);
            com->getSerialPort()->closeSerial();
        }
    }
    mutex.lock();
    condition.wakeOne();
    mutex.unlock();

}