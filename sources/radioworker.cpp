#include <headers/InputMapper.h>
#include <headers/inputenum.h>
#include <headers/radioworker.h>
#include <stdio.h>
#include <strsafe.h>
#include <tchar.h>
#include <windows.h>

#include <QTime>
#include <headers/SerialPort.hpp>
#include <string>

#define DATA_LENGTH 255
#define MAX_RETURNED_ITEMS 255

using namespace std;

QTime timerStart;
QTime timerCheck;
HANDLE hRadioSimconnect = NULL;
SerialPort *arduinoRadio;
string prefix;
bool received = true;
int prefixVal;
InputEnum radioDefs = InputEnum();
InputMapper radioMap = InputMapper();
const char *valport = "\\\\.\\COM27";
struct StructOneDatum {
  int id;
  float value;
};
char receivedString[DATA_LENGTH];

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

};
enum DATA_NAMES {
  DATA_COM_FREQ_STANDBY1,
  DATA_COM_FREQ_ACTIVE1,
  DATA_COM_FREQ_STANDBY2,
  DATA_COM_FREQ_ACTIVE2,
  DATA_NAV_FREQ_STANDBY1,
  DATA_NAV_FREQ_ACTIVE1,
  DATA_NAV_FREQ_STANDBY2,
  DATA_NAV_FREQ_ACTIVE2
};
struct StructDatum {
  StructOneDatum datum[MAX_RETURNED_ITEMS];
};
struct frequencies {
  float activeCom1;
  float standbyCom1;
  float activeCom2;
  float standbyCom2;
  float activeNav1;
  float StandbyNav1;
  float activeNav2;
  float standbyNav2;
};

frequencies freqStruct;

enum DATA_REQUEST_ID {
  REQUEST_PDR_RADIO,
  REQUEST_STRING,
};
void sendCommand(SIMCONNECT_CLIENT_EVENT_ID eventID) {
  SimConnect_TransmitClientEvent(hRadioSimconnect, SIMCONNECT_OBJECT_ID_USER,
                                 eventID, 0, SIMCONNECT_GROUP_PRIORITY_HIGHEST,
                                 SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
}
RadioWorker::RadioWorker() {}
void sendRadioToArduino(float received, std::string prefix) {
  auto intVal = static_cast<int>(received);

  const auto value = intVal;

  auto input_string = prefix + std::to_string(value);
  cout << "size: " << input_string.size() << endl;
  auto *const c_string = new char[input_string.size() + 1];
  std::copy(input_string.begin(), input_string.end(), c_string);
  c_string[input_string.size()] = '\n';
  cout << strlen(c_string) << endl;
  cout << c_string << endl;
  arduinoRadio->writeSerialPort(c_string, strlen(c_string));
  delete[] c_string;
}
void RadioWorker::switchCom1() {
  sendCommand(radioDefs.DEFINITION_COM_STANDBY_SWAP);
}
void RadioWorker::switchNav1() {
  sendCommand(radioDefs.DEFINITION_NAV_1_STANDBY_SWAP);
}
void RadioWorker::MyDispatchProcInput(SIMCONNECT_RECV *pData, DWORD cbData,
                                      void *pContext) {
  HRESULT hr;
  RadioWorker *test = static_cast<RadioWorker *>(pContext);
  switch (pData->dwID) {
    case SIMCONNECT_RECV_ID_EVENT: {
      SIMCONNECT_RECV_EVENT *evt = (SIMCONNECT_RECV_EVENT *)pData;

      switch (evt->uEventID) {
        case EVENT_SIM_START:

          // Now the sim is running, request information on the user aircraft
          hr = SimConnect_RequestDataOnSimObject(
              hRadioSimconnect, REQUEST_PDR_RADIO, DEFINITION_PDR_RADIO,
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
        cout << "hiyi" << endl;
        case REQUEST_PDR_RADIO: {
          int count = 0;
          auto pS = reinterpret_cast<StructDatum *>(&pObjData->dwData);

          while (count < (int)pObjData->dwDefineCount) {
            string valString = std::to_string(pS->datum[count].value);
            switch (pS->datum[count].id) {
              case DATA_COM_FREQ_STANDBY1: {
                printf("\nactive freq 1 = %f", pS->datum[count].value);
                sendRadioToArduino(pS->datum[count].value, "901");
                Sleep(20);
                freqStruct.standbyCom1 = pS->datum[count].value;
                QString stCom1 = QString::number(pS->datum[count].value);
                stCom1.insert(3, '.');
                test->coms[0] = stCom1;
                emit test->updateActiveCom1(test->coms);
                break;
              }
              case DATA_COM_FREQ_ACTIVE1: {
                printf("\nactive freq 1 = %f", pS->datum[count].value);
                sendRadioToArduino(pS->datum[count].value, "900");
                freqStruct.activeCom1 = pS->datum[count].value;
                QString atCom1 = QString::number(pS->datum[count].value);
                Sleep(20);
                atCom1.insert(3, '.');
                test->coms[1] = atCom1;
                emit test->updateActiveCom1(test->coms);
                break;
              }
              case DATA_COM_FREQ_STANDBY2: {
                printf("\nactive freq 1 = %f", pS->datum[count].value);
                sendRadioToArduino(pS->datum[count].value, "903");
                freqStruct.standbyCom2 = pS->datum[count].value;
                Sleep(20);
                break;
              }
              case DATA_COM_FREQ_ACTIVE2: {
                printf("\nactive freq 1 = %f", pS->datum[count].value);
                sendRadioToArduino(pS->datum[count].value, "902");
                freqStruct.activeCom2 = pS->datum[count].value;
                Sleep(20);
                break;
              }
              case DATA_NAV_FREQ_STANDBY1: {
                printf("\nactive freq 1 = %f", pS->datum[count].value);
                sendRadioToArduino(pS->datum[count].value * 100, "911");
                freqStruct.StandbyNav1 = pS->datum[count].value * 100;
                QString sbNav1 = QString::number(freqStruct.StandbyNav1);
                sbNav1.insert(3, '.');
                test->coms[2] = sbNav1;
                emit test->updateActiveCom1(test->coms);
                Sleep(20);
                break;
              }
              case DATA_NAV_FREQ_ACTIVE1: {
                printf("\nactive freq 1 = %f", pS->datum[count].value);
                sendRadioToArduino(pS->datum[count].value * 100, "910");
                freqStruct.activeNav1 = pS->datum[count].value * 100;
                QString atNav1 = QString::number(pS->datum[count].value);
                atNav1.insert(3, '.');
                test->coms[3] = atNav1;
                emit test->updateActiveCom1(test->coms);
                Sleep(20);
                break;
              }
              case DATA_NAV_FREQ_STANDBY2: {
                printf("\nactive freq 1 = %f", pS->datum[count].value);
                sendRadioToArduino(pS->datum[count].value * 100, "913");
                freqStruct.standbyNav2 = pS->datum[count].value * 100;
                Sleep(20);
                break;
              }
              case DATA_NAV_FREQ_ACTIVE2: {
                printf("\nactive freq 1 = %f", pS->datum[count].value);
                sendRadioToArduino(pS->datum[count].value * 100, "912");
                freqStruct.activeNav2 = pS->datum[count].value * 100;
                Sleep(20);
                break;
              }
              default:
                printf("\nUnknown datum ID: %i", pS->datum[count].id);
                break;
            }
            count++;
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

void RadioWorker::RadioEvents() {
  HRESULT hr;
  settings->beginGroup("Coms");
  string val = settings->value("radioActiveBase").toString().toStdString();
  const char *valPortRadio = val.c_str();
  std::cout << valPortRadio << std::endl;
  settings->endGroup();
  settings->sync();

  arduinoRadio = new SerialPort(valPortRadio);

  bool connected = false;

  while (abortRadio != true) {
    if (!connected) {
      timerStart = QTime::currentTime();
      SimConnect_Open(&hRadioSimconnect, "Tagged data", NULL, 0, 0, 0);

      if (hr == S_OK) {
        connected = true;
        cout << "Connection made" << endl;
        hr = SimConnect_AddToDataDefinition(
            hRadioSimconnect, DEFINITION_PDR_RADIO, "COM STANDBY FREQUENCY:1",
            "Khz", SIMCONNECT_DATATYPE_FLOAT32, 0, DATA_COM_FREQ_STANDBY1);
        cout << "HR added status: " << hr << endl;
        hr = SimConnect_AddToDataDefinition(
            hRadioSimconnect, DEFINITION_PDR_RADIO, "COM ACTIVE FREQUENCY:1",
            "Khz", SIMCONNECT_DATATYPE_FLOAT32, 0, DATA_COM_FREQ_ACTIVE1);
        cout << "HR added status: " << hr << endl;
        hr = SimConnect_AddToDataDefinition(
            hRadioSimconnect, DEFINITION_PDR_RADIO, "COM STANDBY FREQUENCY:2",
            "Khz", SIMCONNECT_DATATYPE_FLOAT32, 0, DATA_COM_FREQ_STANDBY2);
        cout << "HR added status: " << hr << endl;
        hr = SimConnect_AddToDataDefinition(
            hRadioSimconnect, DEFINITION_PDR_RADIO, "COM ACTIVE FREQUENCY:2",
            "Khz", SIMCONNECT_DATATYPE_FLOAT32, 01, DATA_COM_FREQ_ACTIVE2);
        cout << "HR added status: " << hr << endl;
        cout << "HR added status: " << hr << endl;
        hr = SimConnect_AddToDataDefinition(
            hRadioSimconnect, DEFINITION_PDR_RADIO, "NAV STANDBY FREQUENCY:1",
            "Mhz", SIMCONNECT_DATATYPE_FLOAT32, 0, DATA_NAV_FREQ_STANDBY1);
        cout << "HR added status: " << hr << endl;
        hr = SimConnect_AddToDataDefinition(
            hRadioSimconnect, DEFINITION_PDR_RADIO, "NAV ACTIVE FREQUENCY:1",
            "Mhz", SIMCONNECT_DATATYPE_FLOAT32, 0, DATA_NAV_FREQ_ACTIVE1);
        cout << "HR added status: " << hr << endl;
        hr = SimConnect_AddToDataDefinition(
            hRadioSimconnect, DEFINITION_PDR_RADIO, "NAV STANDBY FREQUENCY:2",
            "Mhz", SIMCONNECT_DATATYPE_FLOAT32, 0, DATA_NAV_FREQ_STANDBY2);
        cout << "HR added status: " << hr << endl;
        hr = SimConnect_AddToDataDefinition(
            hRadioSimconnect, DEFINITION_PDR_RADIO, "NAV ACTIVE FREQUENCY:2",
            "Mhz", SIMCONNECT_DATATYPE_FLOAT32, 0, DATA_NAV_FREQ_ACTIVE2);
        cout << "HR added status: " << hr << endl;
        SIMCONNECT_OBJECT_ID objectID = SIMCONNECT_OBJECT_ID_USER;
        SimConnect_SubscribeToSystemEvent(hRadioSimconnect, EVENT_SIM_START,
                                          "1sec");
        SimConnect_SetNotificationGroupPriority(
            hRadioSimconnect, GROUP0, SIMCONNECT_GROUP_PRIORITY_HIGHEST);
        SimConnect_SetInputGroupState(hRadioSimconnect, INPUT0,
                                      SIMCONNECT_STATE_ON);
        radioMap.mapEvents(hRadioSimconnect);
        hr = SimConnect_RequestDataOnSimObject(
            hRadioSimconnect, REQUEST_PDR_RADIO, DEFINITION_PDR_RADIO,
            SIMCONNECT_OBJECT_ID_USER, SIMCONNECT_PERIOD_VISUAL_FRAME,
            SIMCONNECT_DATA_REQUEST_FLAG_CHANGED |
                SIMCONNECT_DATA_REQUEST_FLAG_TAGGED,
            0, 3);
        cout << hr << endl;
      }
    }

    SimConnect_CallDispatch(hRadioSimconnect, MyDispatchProcInput, this);
    timerCheck = QTime::currentTime();
    if (timerStart.msecsTo(timerCheck) > 200 && received) {
      sendRadioToArduino(freqStruct.activeCom1, "900");
      Sleep(10);
      sendRadioToArduino(freqStruct.standbyCom1, "901");
      Sleep(10);
      sendRadioToArduino(freqStruct.activeNav1, "910");
      Sleep(10);
      sendRadioToArduino(freqStruct.StandbyNav1, "911");
      Sleep(10);
      sendRadioToArduino(freqStruct.activeCom2, "902");
      Sleep(10);
      sendRadioToArduino(freqStruct.standbyCom2, "903");
      Sleep(10);
      sendRadioToArduino(freqStruct.activeNav2, "913");
      Sleep(10);
      sendRadioToArduino(freqStruct.standbyNav2, "912");
      Sleep(10);

      cout << "UpdateCheck" << endl;
      received = false;
    }

    const auto hasRead =
        arduinoRadio->readSerialPort(receivedString, DATA_LENGTH);

    if (hasRead) {
      if (connected) {
        try {
          received = true;
          prefix = std::string(&receivedString[0], &receivedString[3]);
          prefixVal = stoi(prefix);
          cout << "received: " << prefix << endl;
          switch (prefixVal) {
            case 111: {
              cout << "dec" << endl;
              sendCommand(radioDefs.DEFINITION_COM1_RADIO_WHOLE_DEC);

              break;
            }
            case 112: {
              sendCommand(radioDefs.DEFINITION_COM1_RADIO_WHOLE_INC);
              break;
            }
            case 113: {
              sendCommand(radioDefs.DEFINITION_COM2_RADIO_WHOLE_DEC);
              break;
            }
            case 114: {
              sendCommand(radioDefs.DEFINITION_COM2_RADIO_WHOLE_INC);
              break;
            }
            case 116: {
              sendCommand(radioDefs.DEFINITION_COM_STANDBY_SWAP);
              break;
            }
              // Swap com2
            case 117: {
              sendCommand(radioDefs.DEFINITION_COM_2_STANDBY_SWAP);
              break;
            }
              // Swap nav1
            case 118: {
              sendCommand(radioDefs.DEFINITION_NAV_1_STANDBY_SWAP);
              break;
            }
              // Swap nav1
            case 119: {
              sendCommand(radioDefs.DEFINITION_NAV_2_STANDBY_SWAP);
              break;
            }

            case 120: {
              sendCommand(radioDefs.DEFINITION_COM2_RADIO_FRACT_DEC);
              break;
            }
            case 121: {
              sendCommand(radioDefs.DEFINITION_COM2_RADIO_FRACT_INC);
              break;
            }
            case 122: {
              sendCommand(radioDefs.DEFINITION_COM2_RADIO_FRACT_DEC_CARRY);
              break;
            }
            case 123: {
              sendCommand(radioDefs.DEFINITION_COM2_RADIO_FRACT_INC_CARRY);
              break;
            }
            case 124: {
              sendCommand(radioDefs.DEFINITION_COM_RADIO_FRACT_DEC);
              break;
            }
            case 125: {
              sendCommand(radioDefs.DEFINITION_COM_RADIO_FRACT_INC);
              break;
            }
            case 126: {
              sendCommand(radioDefs.DEFINITION_COM_RADIO_FRACT_DEC_CARRY);
              break;
            }
            case 127: {
              sendCommand(radioDefs.DEFINITION_COM_RADIO_FRACT_INC_CARRY);
              break;
            }

            // NAV
            case 128: {
              sendCommand(radioDefs.DEFINITION_NAV_1_RADIO_FRACT_INC);
              break;
            }
            case 129: {
              sendCommand(radioDefs.DEFINITION_NAV_1_RADIO_FRACT_DEC);
              break;
            }
            case 130: {
              sendCommand(radioDefs.DEFINITION_NAV_2_RADIO_FRACT_INC);
              break;
            }
            case 131: {
              sendCommand(radioDefs.DEFINITION_NAV_2_RADIO_FRACT_DEC);
              break;
            }
            case 132: {
              sendCommand(radioDefs.DEFINITION_NAV_1_RADIO_WHOLE_INC);
              break;
            }
            case 133: {
              sendCommand(radioDefs.DEFINITION_NAV_1_RADIO_WHOLE_DEC);
              break;
            }
            case 134: {
              sendCommand(radioDefs.DEFINITION_NAV_2_RADIO_WHOLE_INC);
              break;
            }
            case 135: {
              sendCommand(radioDefs.DEFINITION_NAV_2_RADIO_WHOLE_DEC);
              break;
            }
            default:
              break;
          }

          timerStart = QTime::currentTime();
        } catch (const std::exception &e) {
        }
      }
    }
    Sleep(1);
    quit();
  }

  arduinoRadio->closeSerial();
  SimConnect_Close(hRadioSimconnect);
}

RadioWorker::~RadioWorker() {
  abortRadio = true;

  arduinoRadio->closeSerial();
  SimConnect_Close(hRadioSimconnect);
  mutex.lock();

  condition.wakeOne();
  mutex.unlock();
}
