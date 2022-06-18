#define DATA_LENGTH 255
#define MAX_RETURNED_ITEMS 255

#include "outputworker.h"
#include "utils/OutputConverters.h"

#include <qsettings.h>
#include <qstandardpaths.h>
#include <windows.h>

#include <cstdio>
#include <iostream>
#include <string>

bool connectionError = false;
float prevSpeed = 0.0f;
float currentSpeed;
int eps = 1;

bool lastConnectionState = false;
SerialPort *ports[10];

// SIMCONNECT_OBJECT_ID objectID = SIMCONNECT_OBJECT_ID_USER;
//// Attempt at WASM
// double dataF = 1.;
// SIMCONNECT_CLIENT_DATA_ID ClientDataID = 1;

int quit = 0;
HANDLE hSimConnect = nullptr;
float dataRecv = 1.0f;
struct Struct1 {
  char title[256];
};

struct StructOneDatum {
  int id;
  float value;
};
struct dataChange {
  float val;
  std::string prefix;
};

struct StructDatum {
  StructOneDatum datum[MAX_RETURNED_ITEMS];
};

struct SimVar {
  int ID;
  int Offset;
  float data;
};

SimVar testVar = {1000, sizeof(float), 1.0f};

SimVar testVarB = {1001, sizeof(float) * 2, 1.0f};
SimVar simVars[2] = {testVar, testVarB};

enum EVENT_ID { EVENT_SIM_START, EVENT_WASM = 3 };

enum DATA_DEFINE_ID {
  DEFINITION_PDR,
  DEFINITION_STRING,
  DEFINITION_WASM_REQUEST = 13

};

enum DATA_REQUEST_ID {
  REQUEST_PDR,
  REQUEST_STRING,
  REQUEST_WASM,
};

int outputClientDataId = 2;
using namespace std;
SerialPort *arduinoTest;

OutputWorker::OutputWorker() {}




void sendToArduino(float received, const std::string &prefix, int bundle, int mode) {
    OutputConverters converter = OutputConverters();
    qDebug()<<"TEST RECEIVED "<<received;
    std::pair<int,std::string> toSend = converter.parseOutputString(received, prefix,mode);
    ports[bundle]->writeSerialPort(toSend.second.c_str(),toSend.first);
}

void sendCharToArduino(const char *received, const std::string &prefix) {

  if (!arduinoTest->isConnected()) {
    connectionError = true;
  } else {
    connectionError = false;
  }
  const auto value = received;
  auto input_string = prefix + value;
  auto *const c_string = new char[input_string.size() + 1];
  std::copy(input_string.begin(), input_string.end(), c_string);
  c_string[input_string.size()] = '\n';
  arduinoTest->writeSerialPort(c_string, strlen(c_string));
  delete[] c_string;
}



void OutputWorker::MyDispatchProcRD(SIMCONNECT_RECV *pData, DWORD cbData,
                                    void *pContext) {
  HRESULT hr;

  auto *outputCast = static_cast<OutputWorker *>(pContext);

  int updatePerXFrames = 3;

  switch (pData->dwID) {
    case SIMCONNECT_RECV_ID_EVENT: {
      auto *evt = (SIMCONNECT_RECV_EVENT *)pData;

      switch (evt->uEventID) {
        case 3: {
          qDebug() << "EVENT WASM TRIGGERED";
          break;
        }
        case EVENT_SIM_START:

            SimConnect_RequestDataOnSimObject(
                    hSimConnect, REQUEST_PDR, DEFINITION_PDR,
                    SIMCONNECT_OBJECT_ID_USER, SIMCONNECT_PERIOD_VISUAL_FRAME,
                    SIMCONNECT_DATA_REQUEST_FLAG_CHANGED |
                    SIMCONNECT_DATA_REQUEST_FLAG_TAGGED,
                    0, updatePerXFrames);

          SimConnect_RequestDataOnSimObject(
              hSimConnect, REQUEST_PDR, DEFINITION_PDR,
              SIMCONNECT_OBJECT_ID_USER, SIMCONNECT_PERIOD_VISUAL_FRAME,
              SIMCONNECT_DATA_REQUEST_FLAG_CHANGED |
                  SIMCONNECT_DATA_REQUEST_FLAG_TAGGED,
              0, updatePerXFrames);


          SimConnect_RequestDataOnSimObject(
              hSimConnect, REQUEST_STRING, DEFINITION_STRING,
              SIMCONNECT_OBJECT_ID_USER, SIMCONNECT_PERIOD_VISUAL_FRAME,
              SIMCONNECT_DATA_REQUEST_FLAG_CHANGED |
                  SIMCONNECT_DATA_REQUEST_FLAG_TAGGED,
              0, 15);

          break;

        default:
          qDebug() << evt->uEventID << "que";
          break;
      }
      break;
    }
    case SIMCONNECT_RECV_ID_CLIENT_DATA: {
      auto pObjData = (SIMCONNECT_RECV_CLIENT_DATA *)pData;
      int bundle = 0;

      Output *output =
          outputCast->outputHandler.findOutputById(pObjData->dwRequestID);
      for (int i = 0; i < outputCast->outputBundles->size(); i++) {
        if (outputCast->outputBundles->at(i)->isOutputInBundle(
                output->getId())) {
          qDebug() << "FOUND IN SET";
          bundle = i;
        }
      }
      qDebug() << "DATA: " << pObjData->dwData << "ID: " << pObjData->dwID
               << pObjData->dwRequestID << pObjData->dwDefineID
               << pObjData->dwObjectID;
      if (pObjData->dwRequestID > 999 && pObjData->dwRequestID < 2000) {
        sendToArduino(pObjData->dwData, std::to_string(pObjData->dwRequestID),
                      bundle, 4);
      }
    } break;

    case SIMCONNECT_RECV_ID_SIMOBJECT_DATA: {
        auto *pObjData = (SIMCONNECT_RECV_SIMOBJECT_DATA *) pData;

        switch (pObjData->dwRequestID) {
            case REQUEST_STRING: {
                auto *pS = (Struct1 *) &pObjData->dwData;
                sendCharToArduino(pS->title, "999");
                cout << "Plane: " << pS->title << endl;
                break;
            }
                qDebug() << "found something";
            case REQUEST_PDR: {
                int count = 0;
                auto pS = reinterpret_cast<StructDatum *>(&pObjData->dwData);

                while (count < (int) pObjData->dwDefineCount) {
                    string valString = std::to_string(pS->datum[count].value);
                    int id = pS->datum[count].id;
                    Output *output = outputCast->outputHandler.findOutputById(id);
                    int bundle = 0;

                    for (int i = 0; i < outputCast->outputBundles->size(); i++) {
                        if (outputCast->outputBundles->at(i)->isOutputInBundle(
                                output->getId())) {
                            qDebug() << "FOUND IN SET";
                            bundle = i;
                        }
                    }

                    int mode = output->getType();
                    string prefix = std::to_string(output->getPrefix());


                    OutputConverters converter = OutputConverters();
                    float value = converter.converOutgoingFloatValue(pS->datum[count].value, mode);
                    sendToArduino(value,prefix,bundle,mode);
                }
            }
        }
    }

    case SIMCONNECT_RECV_ID_QUIT: {
      // quit = 1;
      break;
    }

    default:
      printf("\n Unknown dwID: %d", pData->dwID);
      break;
  }
}

void OutputWorker::testDataRequest() {
  HRESULT hr;

  abort = false;
  keys = settingsHandler.retrieveKeys("runningOutputcoms");
  int keySize = keys->size();
  int succesfullConnected = 0;
  emit(BoardConnectionMade(0, 2));
  for (int i = 0; i < keySize; i++) {
    ports[i] = new SerialPort(
        settingsHandler.retrieveSetting("runningOutputcoms", keys->at(i))
            ->toString()
            .toStdString()
            .c_str());

    if (ports[i]->isConnected()) {
      cout << "CONNECTED" << endl;
      emit(BoardConnectionMade(1, 2));
      succesfullConnected++;
    } else {
      cout << "NOT CONNECTED" << endl;
    }
  }
  if (succesfullConnected == keySize) {
    emit(BoardConnectionMade(2, 2));
  }

  connected = false;

  while (!connected && !abort) {
    emit(GameConnectionMade(1, 2));

    if (SUCCEEDED(
            SimConnect_Open(&hSimConnect, "outputs", nullptr, 0, nullptr, 0))) {
      printf("\nConnected to Flight Simulator!");
      outputMapper->mapOutputs(outputsToMap, hSimConnect);
      SimConnect_MapClientDataNameToID(hSimConnect, "wasm.responses", 2);

      SimConnect_CreateClientData(hSimConnect, 2, 4096,
                                  SIMCONNECT_CREATE_CLIENT_DATA_FLAG_DEFAULT);

      SimConnect_AddToClientDataDefinition(hSimConnect, 12, 0, sizeof(dataRecv),
                                           0, 0);

      emit(GameConnectionMade(2, 2));

      SimConnect_CallDispatch(hSimConnect, MyDispatchProcRD, this);

      connected = true;

      // DATA

      SimConnect_SubscribeToSystemEvent(hSimConnect, EVENT_SIM_START, "1sec");

      while (!abort) {
        SimConnect_CallDispatch(hSimConnect, MyDispatchProcRD, this);

        std::this_thread::sleep_for(std::chrono::milliseconds(10));
      }
    }
  }

  SimConnect_Close(hSimConnect);

  for (int i = 0; i < keys->size(); i++) {
    if (ports[i]->isConnected()) {
      ports[i]->closeSerial();
    }
  }

  quit();
}

OutputWorker::~OutputWorker() {
  for (int i = 0; i < keys->size(); i++) {
    if (ports[i]->isConnected()) {
      ports[i]->closeSerial();
    }
  }
  abort = true;
  mutex.lock();

  condition.wakeOne();
  mutex.unlock();
}

void OutputWorker::clearBundles() { outputBundles->clear(); }

void OutputWorker::addBundle(outputBundle *bundle) {
  outputBundles->append(bundle);
}
