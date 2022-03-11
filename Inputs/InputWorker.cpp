
#include "InputWorker.h"

#include <headers/constants.h>
#include <qsettings.h>

#include <cstdio>
#include <future>
#include <headers/SerialPort.hpp>
#include <iostream>
#include <string>

#include "InputMapper.h"
#include "InputSwitchHandler.h"

#define Bcd2Dec(BcdNum) HornerScheme(BcdNum, 0x10, 10)
#define Dec2Bcd(DecNum) HornerScheme(DecNum, 10, 0x10)

#define DATA_LENGTH 255

// Declare a global object
SerialPort *arduinoInput[10];

HANDLE hInputSimConnect;
//

//// Attempt at WASM
std::string dataF;

// Definition of the client data area format
using namespace std;
enum DATA_DEFINE_ID {
  DEFINITION_1 = 12,
};

InputWorker::InputWorker() {}

enum EVENT_ID {
  EVENT_SIM_START,
  EVENT_WASM = 2,
};
int x = 0;

void InputWorker::MyDispatchProcInput(SIMCONNECT_RECV *pData, DWORD cbData,
                                      void *pContext) {
  switch (pData->dwID) {
    case SIMCONNECT_RECV_ID_QUIT: {
      // quit = 1;
      break;
    }
    case SIMCONNECT_RECV_ID_CLIENT_DATA: {
      // Cast incoming data into interpretable format for this event.
      auto *pObjData = (SIMCONNECT_RECV_CLIENT_DATA *)pData;
      auto *pUserData = (double *)&pObjData->dwData;

      // For demonstration, the actual data value is pointed to by pUserData.
      double myData = *pUserData;

      printf("Request ID = %d.\n", pObjData->dwRequestID);
      break;
    }

    default:
      break;
  }
}
void InputWorker::sendWASMCommand(char cmd) {
  char arrayTest[256] = "9999";
  arrayTest[0] = '9';
  arrayTest[1] = '9';
  arrayTest[2] = '9';
  arrayTest[3] = cmd;

  puts(arrayTest);

  //  SimConnect_TransmitClientEvent(
  //      connect, object, 2, index, SIMCONNECT_GROUP_PRIORITY_HIGHEST,
  //      SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);

  SimConnect_SetClientData(hInputSimConnect, 1, 12,
                           SIMCONNECT_CLIENT_DATA_SET_FLAG_DEFAULT, 0, 256,
                           &arrayTest);
}
void InputWorker::inputEvents() {
  HRESULT hr;
  abortInput = false;
  for (int i = 0; i < curveStrings.size(); i++) {
    auto rudderCurveList = new QList<coordinates>();
    if (!settingsHandler
             .retrieveSubSetting(curveStrings[i] + "Series", "axis",
                                 QString::number(0))
             ->isNull()) {
      for (int j = 0; j < 7; j++) {
        QString value = "axis" + QString::number(i);
        auto foundAxis = settingsHandler
                             .retrieveSubSetting(curveStrings[i] + "Series",
                                                 "axis", QString::number(j))
                             ->toFloat();
        auto foundVal = settingsHandler
                            .retrieveSubSetting(curveStrings[i] + "Series",
                                                "value", QString::number(j))
                            ->toFloat();
        auto *foundCoords = new coordinates(foundAxis, foundVal);
        std::cout << foundCoords->getX() << "X" << std::endl;
        rudderCurveList->append(*foundCoords);
      }
      handler.setCurve(*rudderCurveList, i);
    }
  }
  keys = *settingsHandler.retrieveKeys("runningInputComs");
  int keySize = keys.size();
  int succesfullConnected = 0;
  for (int i = 0; i < keySize; i++) {
    const char *savedPort;
    savedPort = settingsHandler.retrieveSetting("runningInputComs", keys[i])
                    ->toString()
                    .toStdString()
                    .c_str();

    arduinoInput[i] = new SerialPort(savedPort);
    if (arduinoInput[i]->isConnected()) {
      emit(BoardConnectionMade(1, 1));
      succesfullConnected++;
    }
    std::cout << i << "Is connected: " << arduinoInput[i]->isConnected()
              << std::endl;
  }
  if (succesfullConnected == keySize) {
    emit(BoardConnectionMade(2, 1));
  }

  while (!abortInput) {
    emit(GameConnectionMade(1, 1));
    if (SUCCEEDED(SimConnect_Open(&hInputSimConnect, "incSimConnect", NULL, 0,
                                  0, 0))) {
      emit(GameConnectionMade(2, 1));
      printf("\nConnected to Flight Simulator!");

      SimConnect_MapClientDataNameToID(hInputSimConnect, "shared",
                                       ClientDataID);
      hr = SimConnect_CreateClientData(
          hInputSimConnect, ClientDataID, 4096,
          SIMCONNECT_CREATE_CLIENT_DATA_FLAG_DEFAULT);

      SimConnect_AddToClientDataDefinition(
          hInputSimConnect, 12, SIMCONNECT_CLIENTDATAOFFSET_AUTO, 256, 0);

      cout << "CLIENTDATA: " << hr << endl;

      //      hr = SimConnect_MapClientEventToSimEvent(hInputSimConnect,
      //      EVENT_WASM,
      //                                               "LVAR_ACCESS.EFIS");

      cout << "MAPCTOE: " << hr << endl;
      handler.connect = hInputSimConnect;

      handler.object = objectID;
      mapper.mapEvents(hInputSimConnect);

      connected = true;
      sendWASMCommand('8');
      while (!abortInput && connected) {
        for (int i = 0; i < keys.size(); i++) {
          const auto hasRead = arduinoInput[i]->readSerialPort(
              handler.receivedString[i], DATA_LENGTH);

          if (hasRead) {
            if (connected) {
              handler.switchHandling(i);
            }
            // lastVal = handler.receivedString[i];
          }

          // emit updateLastValUI(QString::fromStdString(lastVal));
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
      }

      if (connected) {
        connected = false;
      }
    }
  }

  SimConnect_Close(hInputSimConnect);
  for (int i = 0; i < keys.size(); i++) {
    if (arduinoInput[i]->isConnected()) {
      arduinoInput[i]->closeSerial();
    }
  }

  quit();
}

InputWorker::~InputWorker() {
  abortInput = true;
  connected = false;

  mutex.lock();

  condition.wakeOne();
  mutex.unlock();
}
