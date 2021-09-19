
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
double dataF = 1.;

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

void InputWorker::inputEvents() {
  HRESULT hr;
  abortInput = false;
  keys = *settingsHandler.retrieveKeys("inputComs");
  for (int i = 0; i < keys.size(); i++) {
    const char *savedPort;
    savedPort = settingsHandler.retrieveSetting("inputComs", keys[i])
                    ->toString()
                    .toStdString()
                    .c_str();

    arduinoInput[i] = new SerialPort(savedPort);
    std::cout << i << "Is connected: " << arduinoInput[i]->isConnected()
              << std::endl;
  }

  while (!abortInput) {
    if (SUCCEEDED(
            SimConnect_Open(&hInputSimConnect, "incSimConnect", NULL, 0, 0, 0))) {
      printf("\nConnected to Flight Simulator!");
      SimConnect_MapClientDataNameToID(hInputSimConnect, "shared",
                                       ClientDataID);
      cout << "CLIENTNAME: " << hr << endl;
      hr = SimConnect_CreateClientData(
          hInputSimConnect, ClientDataID, sizeof(dataF),
          SIMCONNECT_CLIENT_DATA_REQUEST_FLAG_CHANGED);
      cout << "CLIENTDATA: " << hr << endl;

      hr = SimConnect_MapClientEventToSimEvent(hInputSimConnect, EVENT_WASM,
                                               "LVAR_ACCESS.EFIS");
      hr = SimConnect_SetClientData(hInputSimConnect, ClientDataID, DEFINITION_1, SIMCONNECT_CLIENT_DATA_SET_FLAG_DEFAULT, 0, sizeof(dataF), &dataF);
      cout << "MAPCTOE: " << hr << endl;
      handler.connect = hInputSimConnect;

      handler.object = objectID;
      mapper.mapEvents(hInputSimConnect);

      connected = true;

      while (!abortInput && connected) {
        for (int i = 0; i < keys.size(); i++) {
          const auto hasRead = arduinoInput[i]->readSerialPort(
              handler.receivedString[i], DATA_LENGTH);

          if (hasRead) {

            if (connected) {
              emit updateLastStatusUI("Connected to game");
              handler.switchHandling(i);
            }
            //lastVal = handler.receivedString[i];
          }

          //emit updateLastValUI(QString::fromStdString(lastVal));
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
      }

      emit updateLastStatusUI("Connection closed");
      if (connected) {
        connected = false;
      } else {
        emit updateLastStatusUI("No connection to the game");
      }
    }
  }

  SimConnect_Close(hInputSimConnect);
  for (int i = 0; i < keys.size(); i++) {
    if (arduinoInput[i]->isConnected()) {
      arduinoInput[i]->closeSerial();
    }
  }
  cout << "we are here" << endl;
  quit();
}

InputWorker::~InputWorker() {
  abortInput = true;
  connected = false;
  //  for (int i = 0; i < keys.size(); i++) {
  //    if (arduinoInput[i]->isConnected()) {
  //      arduinoInput[i]->closeSerial();
  //    }
  //  }

  mutex.lock();

  condition.wakeOne();
  mutex.unlock();
}
