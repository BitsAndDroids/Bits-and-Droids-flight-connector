#include "dualworker.h"

#include <strsafe.h>
#include <tchar.h>
#include <windows.h>

#include <string>
#include <utility>
#include <thread>

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

int radianDualToDegree(double rec) {
  double pi = 3.14159;
  double radian = rec;
  return (radian * (180 / pi));
}

float radianDualToDegreeFloat(double rec) {
  double pi = 3.14159;
  double radian = rec;
  return (radian * (180 / pi));
}
float dualDataRecv = 1.2f;
enum GROUP_ID { GROUP0 = 2, GROUP_A = 1 };
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

// struct dualSimVar {
//  int ID;
//  int Offset;
//  float data;
//};

// dualSimVar dualTestVar = {1000, 0, 1.0f};

// dualSimVar dualTestVarB = {1001, sizeof(float) * 1, 1.0f};
// dualSimVar dualSimVars[2] = {dualTestVar, dualTestVarB};
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

DualWorker::DualWorker() {}

void sendDualToArduino(float received, std::string prefix, int index,
                       int mode) {
  int intVal;
  std::string prefixString = prefix;
  if (stoi(prefix) < 1000) {
    prefixString += " ";
  }
  std::string input_string;
  //  SimConnect_TransmitClientEvent(hSimConnect, objectID, EVENT_WASM, 2,
  //                                 SIMCONNECT_GROUP_PRIORITY_HIGHEST,
  //                                 SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
  if (mode != 99) {
    cout << "0 checked" << endl;
    cout << "VALUE = " << received << endl;
    intVal = static_cast<int>(received);
  } else {
    if (received == 0) {
      intVal = 0;
    } else {
      intVal = 1;
    }
    cout << "VALUE = " << std::fixed << received << endl;
    input_string = prefixString + std::to_string(intVal);
  }

  if (mode == 3 || mode == 97) {
    input_string = prefixString + std::to_string(received);
    cout << "float " << input_string << endl;
  } else {
    const auto value = intVal;
    input_string = prefixString + std::to_string(value);
  }

  cout << "size: " << input_string.size() << endl;
  auto *const c_string = new char[input_string.size() + 1];
  std::copy(input_string.begin(), input_string.end(), c_string);
  c_string[input_string.size()] = '\n';
  cout << strlen(c_string) << endl;
  cout << input_string << endl;

  if (mode == 1) {
    if (received < 0) {
      dualPorts[index]->writeSerialPort(c_string, 7);
    } else {
      dualPorts[index]->writeSerialPort(c_string, 6);
    }
  } else {
    cout << "WHERE DOEST THIS COME FROM " << c_string << endl;
    dualPorts[index]->writeSerialPort(c_string, input_string.size() + 1);
  }
  input_string.clear();

  delete[] c_string;
}
void DualWorker::lastReceived(QString value) {
  emit updateLastValUI(std::move(value));
}
void DualWorker::MyDispatchProcInput(SIMCONNECT_RECV *pData, DWORD cbData,
                                     void *pContext) {
  HRESULT hr;
  auto *dualCast = static_cast<DualWorker *>(pContext);

  switch (pData->dwID) {
    case SIMCONNECT_RECV_ID_EVENT: {
      auto *evt = (SIMCONNECT_RECV_EVENT *)pData;
      cout << "EVENT ID" << evt->uEventID;
      switch (evt->uEventID) {
        case EVENT_SIM_START: {
          // Now the sim is running, request information on the user aircraft
          SimConnect_RequestDataOnSimObject(
              dualSimConnect, REQUEST_PDR_RADIO, DEFINITION_PDR_RADIO,
              SIMCONNECT_OBJECT_ID_USER, SIMCONNECT_PERIOD_VISUAL_FRAME,
              SIMCONNECT_DATA_REQUEST_FLAG_CHANGED |
                  SIMCONNECT_DATA_REQUEST_FLAG_TAGGED,
              0, 3);
          cout << "started" << endl;

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
      auto pObjData = (SIMCONNECT_RECV_CLIENT_DATA *)pData;
      int bundle = 0;

      Output *output =
          dualCast->outputHandler.findOutputById(pObjData->dwRequestID);
      for (int i = 0; i < dualCast->outputBundles->size(); i++) {
        if (dualCast->outputBundles->at(i)->isOutputInBundle(output->getId())) {
          qDebug() << "FOUND IN SET";
          bundle = i;
        }
      }
      dataStr *data = (dataStr *)&pObjData->dwData;
      cout << "STRUCT TEST" << data->val << endl;
      qDebug() << "DATA: " << pObjData->dwData << "ID: " << pObjData->dwID
               << pObjData->dwRequestID << pObjData->dwDefineID
               << pObjData->dwObjectID;

      if (pObjData->dwRequestID > 999 && pObjData->dwRequestID < 9999) {
        sendDualToArduino(
            data->val, std::to_string(pObjData->dwRequestID), bundle,
            dualCast->outputHandler.findOutputById(pObjData->dwRequestID)
                ->getType());
      }
    } break;
    case SIMCONNECT_RECV_ID_SIMOBJECT_DATA: {
      cout << "hiyi" << endl;
      auto *pObjData = (SIMCONNECT_RECV_SIMOBJECT_DATA *)pData;

      switch (pObjData->dwRequestID) {
        case REQUEST_PDR_RADIO: {
          cout << "RADIO" << endl;
          int count = 0;
          auto pS = reinterpret_cast<StructDatum *>(&pObjData->dwData);

          while (count < (int)pObjData->dwDefineCount) {
            string valString = std::to_string(pS->datum[count].value);
            int id = pS->datum[count].id;
            Output *output = dualCast->outputHandler.findOutputById(id);
            int bundle = 0;

            for (int i = 0; i < dualCast->outputBundles->size(); i++) {
              if (dualCast->outputBundles->at(i)->isOutputInBundle(
                      output->getId())) {
                qDebug() << "FOUND IN SET " << i << " bundles"
                         << dualCast->outputBundles->size();
                bundle = i;
              }
            }

            //            while (bundle == NULL) {
            //              if
            //              (outputCast->outputBundles->at(counter)->isOutputInBundle(
            //                      output->getId())) {
            //                bundle = counter;
            //              }
            //              counter++;
            //            }

            qDebug() << "id" << id << "BUNDLE ID" << bundle;
            int mode = output->getType();
            string prefix = std::to_string(output->getPrefix());
            qDebug() << "MODE" << mode << "PREFIX"
                     << QString::fromStdString(prefix)
                     << pS->datum[count].value;

            switch (mode) {
              case 0: {
                qDebug() << "normal";
                sendDualToArduino(pS->datum[count].value, prefix, bundle, 0);
                break;
              }
              case 1: {
                qDebug() << "YARRR";
                sendDualToArduino(pS->datum[count].value * 100, prefix, bundle,
                                  0);
                break;
              }
              case 2: {
                sendDualToArduino(radianDualToDegree(pS->datum[count].value),
                                  prefix, bundle, 0);
                break;
              }
              case 3: {
                sendDualToArduino(pS->datum[count].value, prefix, bundle, 3);
                break;
              }; break;
              case 4: {
                sendDualToArduino(pS->datum[count].value, prefix, bundle, 4);
                break;
              }
              case 5:;
                break;
              case 6: {
                int inHg = pS->datum[count].value * 1000;
                if (inHg % (inHg / 10) >= 5) {
                  inHg += 10;
                }
                sendDualToArduino(inHg / 10, prefix, bundle, 0);
                break;
              }
              case 7: {
                sendDualToArduino(pS->datum[count].value * 1.94, prefix, bundle,
                                  0);
                break;
              }

              case 8: {
                sendDualToArduino(pS->datum[count].value / 1000, prefix, bundle,
                                  0);
                break;
              }

              default:
                printf("\nUnknown datum ID: %i", pS->datum[count].id);
                break;
            }
            ++count;
            dualCast->lastReceived(QString::fromStdString(prefix) + " " +
                                   QString::number(pS->datum[count].value));
          }

          break;
        }
        default:
          break;
      }

      break;
    }
    case SIMCONNECT_RECV_ID_QUIT: {
      // quit = 1;
      break;
    }
  }
}
void DualWorker::sendWASMCommand(char cmd) {
  char arrayTest[256] = "9999";
  arrayTest[0] = '9';
  arrayTest[1] = '9';
  arrayTest[2] = '9';
  arrayTest[3] = (char)cmd;

  puts(arrayTest);
  qDebug() << arrayTest;
  cout << arrayTest << " WASM send" << endl;
  //  SimConnect_TransmitClientEvent(
  //      connect, object, 2, index, SIMCONNECT_GROUP_PRIORITY_HIGHEST,
  //      SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);

  SimConnect_SetClientData(dualSimConnect, 1, 12,
                           SIMCONNECT_CLIENT_DATA_SET_FLAG_DEFAULT, 0, 256,
                           &arrayTest);
}
void DualWorker::addBundle(outputBundle *bundle) {
  outputBundles->append(bundle);
}

void DualWorker::RadioEvents() {
  HRESULT hr;

  keys = settingsHandler.retrieveKeys("runningDualComs");
  int keySize = keys->size();
  int successfullyConnected = 0;
  for (int i = 0; i < keySize; i++) {
    dualPorts[i] = new SerialPort(
        settingsHandler.retrieveSetting("runningDualComs", keys->at(i))
            ->toString()
            .toStdString()
            .c_str());

    if (dualPorts[i]->isConnected()) {
      cout << "CONNECTED" << endl;
      emit(BoardConnectionMade(1, 3));
      successfullyConnected++;
    } else {
      cout << "NOT CONNECTED" << endl;
    }
  }
  if (successfullyConnected == keySize) {
    emit(BoardConnectionMade(2, 3));
  }

  bool connected = false;

  while (!abortDual && !connected) {
    // timerStart = QTime::currentTime();
    emit(GameConnectionMade(1, 3));
    if (SUCCEEDED(SimConnect_Open(&dualSimConnect, "dualConnect", nullptr, 0,
                                  nullptr, 0))) {
      connected = true;
      emit(GameConnectionMade(2, 3));

      cout << "Connection made" << outputsToMap.size() << endl;

      SimConnect_MapClientDataNameToID(dualSimConnect, "shared", ClientDataID);

      SimConnect_CreateClientData(dualSimConnect, ClientDataID, 256,
                                  SIMCONNECT_CLIENT_DATA_REQUEST_FLAG_CHANGED);

      //      SimConnect_MapClientEventToSimEvent(dualSimConnect, EVENT_WASM,
      //                                          "LVAR_ACCESS.EFIS");

      //      SimConnect_AddClientEventToNotificationGroup(dualSimConnect,
      //      GROUP_A,
      //                                                   EVENT_WASM, true);

      //      SimConnect_SetNotificationGroupPriority(
      //          dualSimConnect, GROUP_A, SIMCONNECT_GROUP_PRIORITY_HIGHEST);

      SimConnect_SetClientData(dualSimConnect, ClientDataID, DEFINITION_1,
                               SIMCONNECT_CLIENT_DATA_SET_FLAG_DEFAULT, 0, 256,
                               &dataF);

      SimConnect_AddToClientDataDefinition(
          dualSimConnect, 12, SIMCONNECT_CLIENTDATAOFFSET_AUTO, 256, 0);
      dualInputHandler->connect = dualSimConnect;
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

      //      for (auto &simVar : dualSimVars) {
      //        SimConnect_AddToClientDataDefinition(
      //            dualSimConnect, simVar.ID, simVar.Offset, sizeof(float), 0,
      //            0);
      //        SimConnect_RequestClientData(
      //            dualSimConnect, 2, simVar.ID, simVar.ID,
      //            SIMCONNECT_CLIENT_DATA_PERIOD_ON_SET,
      //            SIMCONNECT_CLIENT_DATA_REQUEST_FLAG_CHANGED, 0, 0, 0);
      //      }
      sendWASMCommand('8');
      dualOutputMapper->mapOutputs(outputsToMap, dualSimConnect);
      SimConnect_SubscribeToSystemEvent(dualSimConnect, EVENT_SIM_START,
                                        "1sec");

      SimConnect_RequestDataOnSimObject(
          dualSimConnect, REQUEST_PDR_RADIO, DEFINITION_PDR_RADIO,
          SIMCONNECT_OBJECT_ID_USER, SIMCONNECT_PERIOD_VISUAL_FRAME,
          SIMCONNECT_DATA_REQUEST_FLAG_CHANGED |
              SIMCONNECT_DATA_REQUEST_FLAG_TAGGED,
          0, 3);

      while (!abortDual) {
        SimConnect_CallDispatch(dualSimConnect, MyDispatchProcInput, this);

        // timerCheck = QTime::currentTime();

        for (int i = 0; i < keys->size(); i++) {
          const auto hasRead = dualPorts[i]->readSerialPort(
              dualInputHandler->receivedString[i], DATA_LENGTH);

          if (hasRead) {
            if (connected) {
              dualInputHandler->switchHandling(i);
              // timerStart = QTime::currentTime();
            }
          }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
      }
      SimConnect_Close(dualSimConnect);
    }
  }
  for (int i = 0; i < keys->size(); i++) {
    if (dualPorts[i]->isConnected()) {
      dualPorts[i]->closeSerial();
    }
  }
  quit();
}

void DualWorker::clearBundles() { this->outputBundles->clear(); }

DualWorker::~DualWorker() {
  for (int i = 0; i < keys->size(); i++) {
    if (dualPorts[i]->isConnected()) {
      dualPorts[i]->closeSerial();
    }
  }
  abortDual = true;
  mutex.lock();

  condition.wakeOne();
  mutex.unlock();
}
