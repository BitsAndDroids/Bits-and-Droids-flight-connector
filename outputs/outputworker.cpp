#define DATA_LENGTH 255
#define MAX_RETURNED_ITEMS 255
#include "outputworker.h"

#include <headers/SimConnect.h>
#include <qsettings.h>
#include <qstandardpaths.h>
#include <tchar.h>
#include <windows.h>

#include <headers/SerialPort.hpp>
#include <string>

#include "outputmapper.h"
#include "stdio.h"
#include "strsafe.h"
char output[DATA_LENGTH];
bool strincProcessing = false;
bool connectionError = false;
float prevSpeed = 0.0f;
float currentSpeed;
int eps = 1;
bool lastConnectionState = false;
SerialPort *ports[10];
// QList<SerialPort *> ports;
SerialPort *arduino;
SIMCONNECT_OBJECT_ID objectID = SIMCONNECT_OBJECT_ID_USER;
int quit = 0;
HANDLE hSimConnect = NULL;

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

enum EVENT_ID { EVENT_SIM_START };

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

void sendToArduino(float received, std::string prefix, int index) {
  auto intVal = static_cast<int>(received);

  if (!ports[index]->isConnected()) {
    connectionError = true;
  } else {
    connectionError = false;
  }
  const auto value = intVal;

  auto input_string = prefix + std::to_string(value);
  cout << "size: " << input_string.size() << endl;
  auto *const c_string = new char[input_string.size() + 1];
  std::copy(input_string.begin(), input_string.end(), c_string);
  c_string[input_string.size()] = '\n';
  cout << strlen(c_string) << endl;
  cout << c_string << endl;
  ports[index]->writeSerialPort(c_string, strlen(c_string));
  qDebug() << ports[index] << "YES";
  delete[] c_string;
}
void sendCharToArduino(const char *received, std::string prefix) {
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
void sendLengthToArduino(float received, std::string prefix, int strLength,
                         int index) {
  qDebug() << "MATEY INTVAL = " << received;
  auto intVal = static_cast<int>(received);

  if (!ports[index]->isConnected()) {
    connectionError = true;
    qDebug() << "TRUE";
  } else {
    connectionError = false;
    qDebug() << "FALSE";
  }

  auto input_string = prefix + std::to_string(intVal);

  switch ((strLength + 3) - input_string.size()) {
    case 1: {
      input_string += " ";
      break;
    }
    case 2: {
      input_string += "  ";
      break;
    }
    case 3: {
      input_string += "   ";
      break;
    }
    case 4: {
      input_string += "    ";
      break;
    }
    case 5: {
      input_string += "     ";
      break;
    }
    default:
      break;
  }

  auto *const c_string = new char[input_string.size() + 1];
  std::copy(input_string.begin(), input_string.end(), c_string);
  c_string[input_string.size()] = '\n';
  ports[index]->writeSerialPort(c_string, strlen(c_string));
  delete[] c_string;
}

void sendFloatToArduino(float received, std::string prefix) {
  if (!arduinoTest->isConnected()) {
    connectionError = true;
  } else {
    connectionError = false;
  }
  auto input_string = prefix + std::to_string(received);
  auto *const c_string = new char[input_string.size() + 1];
  std::copy(input_string.begin(), input_string.end(), c_string);
  c_string[input_string.size()] = '\n';
  if (received < 0) {
    arduinoTest->writeSerialPort(c_string, 7);
  } else {
    arduinoTest->writeSerialPort(c_string, 6);
  }

  delete[] c_string;
}

void sendBoolToArduino(float received, std::string prefix) {
  int intVal;
  if (received == 0) {
    intVal = 0;
  } else {
    intVal = 1;
  }

  if (!arduinoTest->isConnected()) {
    connectionError = true;
  } else {
    connectionError = false;
  }
  const auto value = intVal;
  auto input_string = prefix + std::to_string(value);
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
  OutputWorker *outputCast = static_cast<OutputWorker *>(pContext);

  int updatePerXFrames = 15;

  switch (pData->dwID) {
    case SIMCONNECT_RECV_ID_EVENT: {
      SIMCONNECT_RECV_EVENT *evt = (SIMCONNECT_RECV_EVENT *)pData;

      switch (evt->uEventID) {
        case EVENT_SIM_START:

          qDebug() << "SIM STARTED";
          // Now the sim is running, request information on the user aircraft
          hr = SimConnect_RequestDataOnSimObject(
              hSimConnect, REQUEST_PDR, DEFINITION_PDR,
              SIMCONNECT_OBJECT_ID_USER, SIMCONNECT_PERIOD_VISUAL_FRAME,
              SIMCONNECT_DATA_REQUEST_FLAG_CHANGED |
                  SIMCONNECT_DATA_REQUEST_FLAG_TAGGED,
              0, updatePerXFrames);
          qDebug() << hr;

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
      SIMCONNECT_RECV_SIMOBJECT_DATA *pObjData =
          (SIMCONNECT_RECV_SIMOBJECT_DATA *)pData;

      switch (pObjData->dwRequestID) {
        case REQUEST_STRING: {
          Struct1 *pS = (Struct1 *)&pObjData->dwData;
          sendCharToArduino(pS->title, "999");
          cout << "Plane: " << pS->title << endl;
          cout << "hi" << endl;
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
            int bundle = NULL;

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
                qDebug() << "YARRR";
                sendToArduino(pS->datum[count].value, prefix, bundle);
                break;
              }
              case 1: {
                qDebug() << "YARRR";
                sendLengthToArduino(pS->datum[count].value * 100, prefix, 3,
                                    bundle);
                break;
              }
              case 2: {
                sendToArduino(radianToDegree(pS->datum[count].value), prefix,
                              bundle);
                break;
              }
              case 3:;
                break;
              case 4: {
                sendBoolToArduino(pS->datum[count].value, prefix);
                break;
              }
              case 5:;
                break;
              case 6: {
                int inHg = pS->datum[count].value * 1000;
                if (inHg % (inHg / 10) >= 5) {
                  inHg += 10;
                }
                sendToArduino(inHg / 10, prefix, bundle);
                break;
              }
              case 7: {
                sendToArduino(pS->datum[count].value * 1.94, prefix, bundle);
                break;
              }

              case 8: {
                sendLengthToArduino(pS->datum[count].value / 1000, prefix, 4,
                                    bundle);
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
      printf("\n Unknown dwID: %d", pData->dwID);
      cout << pData->dwVersion << "id" << pData->dwID << endl;
      break;
  }
}
void OutputWorker::testDataRequest() {
  HRESULT hr;
  QStringList *keys = settingsHandler.retrieveKeys("outputcoms");
  for (int i = 0; i < keys->size(); i++) {
    ports[i] = new SerialPort(
        settingsHandler.retrieveSetting("outputcoms", keys->at(i))
            ->toString()
            .toStdString()
            .c_str());
    qDebug() << "hit";
    if (ports[i]->isConnected()) {
      cout << "CONNECTED" << endl;
    } else {
      cout << "NOT CONNECTED" << endl;
    }
  }
  if (SUCCEEDED(SimConnect_Open(&hSimConnect, "data", NULL, 0, 0, 0))) {
    printf("\nConnected to Flight Simulator!");

    emit updateLastValUI("Connected to the game");
    // Set up the data definition, ensuring that all the elements are in
    // Float32 units, to match the StructDatum structure The number of entries
    // in the DEFINITION_PDR definition should be equal to the
    // maxReturnedItems define

    // DATA

    outputMapper outputMapper;

    outputMapper.mapOutputs(outputsToMap, hSimConnect);

    // Request an event when the simulation starts

    hr =
        SimConnect_SubscribeToSystemEvent(hSimConnect, EVENT_SIM_START, "1sec");

    printf("\nLaunch a flight.");

    while (abort != true) {
      if (connectionError) {
        if (lastConnectionState != connectionError) {
          emit updateLastStatusUI("Error connecting to Arduino");
          lastConnectionState = connectionError;
        }
      } else {
        emit updateLastStatusUI("Succesfully connected to your Arduino");
      }

      SimConnect_CallDispatch(hSimConnect, MyDispatchProcRD, this);
      Sleep(1);
      quit();
    }
    for (int i = 0; i < outputBundles->size(); i++) {
      ports[i]->closeSerial();
    }
    hr = SimConnect_Close(hSimConnect);
  }
}
OutputWorker::~OutputWorker() {
  lastConnectionState = false;
  connectionError = false;
  for (int i = 0; i < outputBundles->size(); i++) {
    outputBundles->at(i)->getSerialPort()->closeSerial();
  }
  // arduinoTest->closeSerial();
  mutex.lock();
  abort = true;
  condition.wakeOne();
  mutex.unlock();
}

void OutputWorker::clearBundles() { outputBundles->clear(); }

void OutputWorker::addBundle(outputBundle *bundle) {
  outputBundles->append(bundle);
}
