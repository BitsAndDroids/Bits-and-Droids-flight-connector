#define DATA_LENGTH 255
#define MAX_RETURNED_ITEMS 255
#include "outputworker.h"

#include <qsettings.h>
#include <qstandardpaths.h>
#include <tchar.h>
#include <windows.h>

#include <string>

#include "stdio.h"
#include "strsafe.h"
#include <headers/SimConnect.h>

#include "outputmapper.h"

#include <headers/SerialPort.hpp>
char output[DATA_LENGTH];
bool strincProcessing = false;
bool connectionError = false;
float prevSpeed = 0.0f;
float currentSpeed;
int eps = 1;
bool lastConnectionState = false;

SerialPort *arduino;

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

void sendToArduino(float received, std::string prefix) {
  auto intVal = static_cast<int>(received);

  if (!arduinoTest->isConnected()) {
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
  arduinoTest->writeSerialPort(c_string, strlen(c_string));
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
void sendLengthToArduino(float received, std::string prefix, int strLength) {
  auto intVal = static_cast<int>(received);

  if (!arduinoTest->isConnected()) {
    connectionError = true;
  } else {
    connectionError = false;
  }
  const auto value = intVal;
  auto input_string = prefix + std::to_string(value);
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
  arduinoTest->writeSerialPort(c_string, strlen(c_string));
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
  QString path =
      QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
  QSettings settings(path + "/" + "settings.ini", QSettings::IniFormat);
  OutputWorker *outputCast = static_cast<OutputWorker *>(pContext);
  settings.beginGroup("Settings");
  int updatePerXFrames = settings.value("updateEveryXFramesLineEdit").toInt();
  if (settings.value("updateEveryXFramesLineEdit").isNull()) {
    updatePerXFrames = 15;
  }
  HRESULT hr;
  switch (pData->dwID) {
    case SIMCONNECT_RECV_ID_EVENT: {
      SIMCONNECT_RECV_EVENT *evt = (SIMCONNECT_RECV_EVENT *)pData;

      switch (evt->uEventID) {
        case EVENT_SIM_START:
          outputMapper outputMapper;
          //outputMapper.mapOutputs(NULL,hSimConnect);
          // Now the sim is running, request information on the user aircraft
          hr = SimConnect_RequestDataOnSimObject(
              hSimConnect, REQUEST_PDR, DEFINITION_PDR,
              SIMCONNECT_OBJECT_ID_USER, SIMCONNECT_PERIOD_VISUAL_FRAME,
              SIMCONNECT_DATA_REQUEST_FLAG_CHANGED |
                  SIMCONNECT_DATA_REQUEST_FLAG_TAGGED,
              0, updatePerXFrames);
          if (strincProcessing) {
            hr = SimConnect_RequestDataOnSimObject(
                hSimConnect, REQUEST_STRING, DEFINITION_STRING,
                SIMCONNECT_OBJECT_ID_USER, SIMCONNECT_PERIOD_VISUAL_FRAME,
                SIMCONNECT_DATA_REQUEST_FLAG_CHANGED |
                    SIMCONNECT_DATA_REQUEST_FLAG_TAGGED,
                0, 15);
          }

          break;
      }
      break;
    }

    case SIMCONNECT_RECV_ID_SIMOBJECT_DATA: {
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
            Output* output = outputCast->outputHandler.findOutputById(pS->datum->id);
            switch (pS->datum[count].id) {
            int mode = output->getType();
            string prefix = std::to_string(output->getPrefix());
            switch (mode){
                case 0: {
                sendToArduino(pS->datum[count].value, prefix);
                break;
            }
            case 1:{
                sendLengthToArduino(pS->datum[count].value * 100, prefix, 3);
                break;
            }
            case 2: {
                sendToArduino(radianToDegree(pS->datum[count].value), prefix);
                break;
            }
                case 3: ;break;
            case 4:{
                sendBoolToArduino(pS->datum[count].value, prefix);
                break;
            }
                case 5: ;break;
            case 6: {
                int inHg = pS->datum[count].value * 1000;
                                if (inHg % (inHg / 10) >= 5) {
                                  inHg += 10;
                                }
                                sendToArduino(inHg / 10, prefix);
                                break;
            }
            case 7:{
                sendToArduino(pS->datum[count].value * 1.94, prefix);
                break;
            }

            case 8:{
                sendLengthToArduino(pS->datum[count].value / 1000,prefix,4);
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

      }
      break;
    }
  }

    case SIMCONNECT_RECV_ID_QUIT: {
      // quit = 1;
      break;
    }

    default:
      printf("\n Unknown dwID: %d", pData->dwID);
      cout << pData->dwVersion << endl;
      break;
  }
}
void OutputWorker::testDataRequest() {
  HRESULT hr;

  if (SUCCEEDED(
          SimConnect_Open(&hSimConnect, "Tagged Data", NULL, 0, nullptr, 0))) {
    printf("\nConnected to Flight Simulator!");

    emit updateLastValUI("Connected to the game");
    // Set up the data definition, ensuring that all the elements are in Float32
    // units, to match the StructDatum structure The number of entries in the
    // DEFINITION_PDR definition should be equal to the maxReturnedItems define

    // DATA



    settings->beginGroup("Coms");
    string val = settings->value("outputComActiveBase").toString().toStdString();
    const char *valPort = val.c_str();
    arduinoTest = new SerialPort(valPort);
    settings->endGroup();
    settings->sync();

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

      SimConnect_CallDispatch(hSimConnect, MyDispatchProcRD, nullptr);
      Sleep(1);
      quit();
    }
    arduinoTest->closeSerial();
    hr = SimConnect_Close(hSimConnect);
  }
}
OutputWorker::~OutputWorker() {
  lastConnectionState = false;
  connectionError = false;
  // arduinoTest->closeSerial();
  mutex.lock();
  abort = true;
  condition.wakeOne();
  mutex.unlock();
}
