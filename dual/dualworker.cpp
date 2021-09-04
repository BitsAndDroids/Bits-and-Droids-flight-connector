#include "dualworker.h"


#include <strsafe.h>
#include <tchar.h>
#include <windows.h>

#include <string>

#define DATA_LENGTH 255
#define MAX_RETURNED_ITEMS 255

using namespace std;
SerialPort *dualPorts[10];

HANDLE dualSimConnect = NULL;


InputEnum radioDefs = InputEnum();
InputMapper radioMap = InputMapper();

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
enum GROUP_ID {
  GROUP0,
};
enum INPUT_ID {
  INPUT0,
};
enum EVENT_ID { EVENT_SIM_START };

enum DATA_DEFINE_ID {
  DEFINITION_PDR_RADIO,
  DEFINITION_STRING,
  DEFINITION_ELEVATOR_TRIM_PCT,

};

struct StructDatum {
  StructOneDatum datum[MAX_RETURNED_ITEMS];
};

enum DATA_REQUEST_ID {
  REQUEST_PDR_RADIO,
  REQUEST_STRING,
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
  std::string input_string;

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
  DualWorker *dualCast = static_cast<DualWorker *>(pContext);
  switch (pData->dwID) {
    case SIMCONNECT_RECV_ID_EVENT: {
      SIMCONNECT_RECV_EVENT *evt = (SIMCONNECT_RECV_EVENT *)pData;

      switch (evt->uEventID) {
        case EVENT_SIM_START:

          // Now the sim is running, request information on the user aircraft
          hr = SimConnect_RequestDataOnSimObject(
              dualSimConnect, REQUEST_PDR_RADIO, DEFINITION_PDR_RADIO,
              SIMCONNECT_OBJECT_ID_USER, SIMCONNECT_PERIOD_VISUAL_FRAME,
              SIMCONNECT_DATA_REQUEST_FLAG_CHANGED |
                  SIMCONNECT_DATA_REQUEST_FLAG_TAGGED,
              0, 3);
          cout << hr << endl;

          break;

        default:
          break;
      }
      break;
    }

    case SIMCONNECT_RECV_ID_SIMOBJECT_DATA: {
      cout << "hiyi" << endl;
      SIMCONNECT_RECV_SIMOBJECT_DATA *pObjData =
          (SIMCONNECT_RECV_SIMOBJECT_DATA *)pData;

      switch (pObjData->dwRequestID) {
        case REQUEST_PDR_RADIO: {
          int count = 0;
          auto pS = reinterpret_cast<StructDatum *>(&pObjData->dwData);

          while (count < (int)pObjData->dwDefineCount)

          {
            string valString = std::to_string(pS->datum[count].value);
            int id = pS->datum[count].id;
            Output *output = dualCast->outputHandler.findOutputById(id);
            int bundle = NULL;

            for (int i = 0; i < dualCast->outputBundles->size(); i++) {
              if (dualCast->outputBundles->at(i)->isOutputInBundle(
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

void DualWorker::addBundle(outputBundle *bundle) {
  this->outputBundles->append(bundle);
}

void DualWorker::RadioEvents() {
  HRESULT hr;

  keys = settingsHandler.retrieveKeys("dualComs");
  for (int i = 0; i < keys->size(); i++) {
    dualPorts[i] =
        new SerialPort(settingsHandler.retrieveSetting("dualComs", keys->at(i))
                           ->toString()
                           .toStdString()
                           .c_str());

    if (dualPorts[i]->isConnected()) {
      cout << "CONNECTED" << endl;
    } else {
      cout << "NOT CONNECTED" << endl;
    }
  }

  bool connected = false;

  while (!abortDual && !connected) {
    // timerStart = QTime::currentTime();
    if (SUCCEEDED(
            SimConnect_Open(&dualSimConnect, "dual data", NULL, 0, 0, 0))) {
      connected = true;

      dualInputHandler->connect = dualSimConnect;
      dualInputHandler->object = SIMCONNECT_OBJECT_ID_USER;
      dualInputMapper.mapEvents(dualSimConnect);

      dualOutputMapper.mapOutputs(outputsToMap, dualSimConnect);
      cout << "Connection made" << outputsToMap.size() << endl;
      hr = SimConnect_RequestDataOnSimObject(
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
  mutex.lock();
  abortDual = true;
  condition.wakeOne();
  mutex.unlock();
}
