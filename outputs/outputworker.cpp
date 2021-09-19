#define DATA_LENGTH 255
#define MAX_RETURNED_ITEMS 255
#include "outputworker.h"

#include <qsettings.h>
#include <qstandardpaths.h>
#include <tchar.h>
#include <windows.h>

#include <cstdio>
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

enum EVENT_ID { EVENT_SIM_START, EVENT_WASM = 2 };

enum DATA_DEFINE_ID {
  DEFINITION_PDR,
  DEFINITION_STRING,

};

enum DATA_REQUEST_ID {
  REQUEST_PDR,
  REQUEST_STRING,
};

using namespace std;
SerialPort *arduinoTest;
OutputWorker::OutputWorker() {}
void sendToArduino(float received, const std::string &prefix, int index,
                   int mode) {
  int intVal;
  std::string input_string;
  //  SimConnect_TransmitClientEvent(hSimConnect, objectID, EVENT_WASM, 2,
  //                                 SIMCONNECT_GROUP_PRIORITY_HIGHEST,
  //                                 SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
  if (mode != 4) {
    cout << "0 checked" << endl;
    intVal = static_cast<int>(received);
  } else {
    if (received == 0) {
      intVal = 0;
    } else {
      intVal = 1;
    }
  }

  if (mode == 3) {
    input_string = prefix + std::to_string(received);
  } else {
    const auto value = intVal;
    input_string = prefix + std::to_string(value);
  }

  cout << "size: " << input_string.size() << endl;
  auto *const c_string = new char[input_string.size() + 1];
  std::copy(input_string.begin(), input_string.end(), c_string);
  c_string[input_string.size()] = '\n';
  cout << strlen(c_string) << endl;
  cout << input_string << endl;

  if (mode == 1) {
    if (received < 0) {
      ports[index]->writeSerialPort(c_string, 7);
    } else {
      ports[index]->writeSerialPort(c_string, 6);
    }
  } else {
    cout << "WHERE DOEST THIS COME FROM " << c_string << endl;
    ports[index]->writeSerialPort(c_string, input_string.size() + 1);
  }
  input_string.clear();
  delete[] c_string;
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

int radianToDegree(double rec) {
  double pi = 3.14159;
  double radian = rec;
  return (radian * (180 / pi));
}

float radianToDegreeFloat(double rec) {
  double pi = 3.14159;
  double radian = rec;
  return (radian * (180 / pi));
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
        case EVENT_SIM_START:

          hr = SimConnect_RequestDataOnSimObject(
              hSimConnect, REQUEST_PDR, DEFINITION_PDR,
              SIMCONNECT_OBJECT_ID_USER, SIMCONNECT_PERIOD_VISUAL_FRAME,
              SIMCONNECT_DATA_REQUEST_FLAG_CHANGED |
                  SIMCONNECT_DATA_REQUEST_FLAG_TAGGED,
              0, updatePerXFrames);

          hr = SimConnect_RequestDataOnSimObject(
              hSimConnect, REQUEST_STRING, DEFINITION_STRING,
              SIMCONNECT_OBJECT_ID_USER, SIMCONNECT_PERIOD_VISUAL_FRAME,
              SIMCONNECT_DATA_REQUEST_FLAG_CHANGED |
                  SIMCONNECT_DATA_REQUEST_FLAG_TAGGED,
              0, 15);

          break;

        default:
          break;
      }
      break;
    }

    case SIMCONNECT_RECV_ID_SIMOBJECT_DATA: {
      qDebug() << "si";
      auto *pObjData = (SIMCONNECT_RECV_SIMOBJECT_DATA *)pData;

      switch (pObjData->dwRequestID) {
        case REQUEST_STRING: {
          auto *pS = (Struct1 *)&pObjData->dwData;
          sendCharToArduino(pS->title, "999");
          cout << "Plane: " << pS->title << endl;
          break;
        }

        case REQUEST_PDR: {
          int count = 0;
          auto pS = reinterpret_cast<StructDatum *>(&pObjData->dwData);

          while (count < (int)pObjData->dwDefineCount)

          {
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
                sendToArduino(pS->datum[count].value, prefix, bundle, 0);
                break;
              }
              case 1: {
                cout << "daaa" << endl;
                sendToArduino(pS->datum[count].value * 100, prefix, bundle, 0);
                break;
              }
              case 2: {
                sendToArduino(radianToDegree(pS->datum[count].value), prefix,
                              bundle, 0);
                break;
              }
              case 3: {
                sendToArduino(pS->datum[count].value, prefix, bundle, 3);
                break;
              }; break;
              case 4: {
                sendToArduino(pS->datum[count].value, prefix, bundle, 4);
                break;
              }
              case 5:;
                break;
              case 6: {
                int inHg = pS->datum[count].value * 1000;
                if (inHg % (inHg / 10) >= 5) {
                  inHg += 10;
                }
                sendToArduino(inHg / 10, prefix, bundle, 0);
                break;
              }
              case 7: {
                sendToArduino(pS->datum[count].value * 1.94, prefix, bundle, 0);
                break;
              }

              case 8: {
                sendToArduino(pS->datum[count].value / 1000, prefix, bundle, 0);
                break;
              }
              case 9: {
                qDebug() << "interesting" << pS->datum[count].value;
                sendToArduino(pS->datum[count].value, prefix, bundle, 0);

                break;
              }

              default:
                printf("\nUnknown datum ID: %i", pS->datum[count].id);
                break;
            }
            ++count;
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

    default:
      // printf("\n Unknown dwID: %d", pData->dwID);
      // cout << pData->dwVersion << "id" << pData->dwID << endl;
      break;
  }
}
void OutputWorker::testDataRequest() {
  HRESULT hr;
  abort = false;
  keys = settingsHandler.retrieveKeys("outputcoms");

  for (int i = 0; i < keys->size(); i++) {
    ports[i] = new SerialPort(
        settingsHandler.retrieveSetting("outputcoms", keys->at(i))
            ->toString()
            .toStdString()
            .c_str());

    if (ports[i]->isConnected()) {
      cout << "CONNECTED" << endl;
    } else {
      cout << "NOT CONNECTED" << endl;
    }
  }
  connected = false;
  qDebug() << "attempted";
  while (!connected && !abort) {
    qDebug() << "attempted";
    if (SUCCEEDED(SimConnect_Open(&hSimConnect, "outputs", NULL, 0, 0, 0))) {
      printf("\nConnected to Flight Simulator!");
      connected = true;
      //      SimConnect_MapClientDataNameToID(hSimConnect, "outputs",
      //      ClientDataID);
      // SimConnect_CreateClientData(hSimConnect,
      //      ClientDataID, sizeof(dataF),
      //                                  SIMCONNECT_CLIENT_DATA_REQUEST_FLAG_CHANGED);
      //      hr = SimConnect_MapClientEventToSimEvent(hSimConnect, EVENT_WASM,
      //                                               "LVAR_ACCESS.EFIS");
      emit updateLastValUI("Connected to the game");

      // DATA

      outputMapper.mapOutputs(outputsToMap, hSimConnect);

      // Request an event when the simulation starts

      hr = SimConnect_SubscribeToSystemEvent(hSimConnect, EVENT_SIM_START,
                                             "1sec");

      while (!abort) {
        SimConnect_CallDispatch(hSimConnect, MyDispatchProcRD, this);
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
      }
    }
  }
  SimConnect_Close(hSimConnect);
  for (int i = 0; i < keys->size(); i++) {
    if (ports[i]->isConnected()) {
      cout << keys->size() << " keys and hit" << endl;
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
