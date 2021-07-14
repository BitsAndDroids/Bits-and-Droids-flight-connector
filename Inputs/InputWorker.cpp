// BitsAndDroidsInputHandler.cpp : This file contains the 'main' function.
// Program execution begins and ends there.
//
#include "InputWorker.h"

#include <Windows.h>
#include <headers/SerialReader.h>
#include <headers/constants.h>
#include <qsettings.h>
#include <qstandardpaths.h>
#include <tchar.h>

#include <cstdio>
#include <future>
#include <iostream>
#include <string>

#include "InputMapper.h"
#include "InputSwitchHandler.h"
#include "headers/SerialPort.hpp"
#include "headers/SimConnect.h"

#define Bcd2Dec(BcdNum) HornerScheme(BcdNum, 0x10, 10)
#define Dec2Bcd(DecNum) HornerScheme(DecNum, 10, 0x10)

#define DATA_LENGTH 255

// Declare a global object
SerialPort *arduinoInput[10];

HANDLE hInputSimConnect = NULL;

// Definition of the client data area format
double data = 1.;
using namespace std;
InputEnum defs = InputEnum();
InputWorker::InputWorker() {}
InputMapper mapper = InputMapper();
InputSwitchHandler handler = InputSwitchHandler();

enum GROUP_ID {
  GROUP0,
  GROUP_A = 1,
};

int x = 0;
enum DATA_REQUEST_ID {

  REQUEST_THROTTLE,
  REQUEST_MIXTURE,
  REQUEST_PROPS,
  REQUEST_1 = 10,
};

void InputWorker::MyDispatchProcInput(SIMCONNECT_RECV *pData, DWORD cbData,
                                      void *pContext) {
  switch (pData->dwID) {
    case SIMCONNECT_RECV_ID_QUIT: {
      // quit = 1;
      break;
    }
    case SIMCONNECT_RECV_ID_CLIENT_DATA: {
      // Cast incoming data into interpretable format for this event.
      SIMCONNECT_RECV_CLIENT_DATA *pObjData =
          (SIMCONNECT_RECV_CLIENT_DATA *)pData;

      printf("Request ID = %d.\n", pObjData->dwRequestID);
      break;
    }

    default:
      break;
  }
}

enum INPUT_ID {
  INPUT0,
};

void InputWorker::inputEvents() {
  HRESULT hr;
  cout << "why" << endl;
  for (int i = 0; i < keys.size(); i++) {
    const char *savedPort;
    cout << "why" << endl;
    savedPort = settingsHandler.retrieveSetting("inputCom", keys[i])
                    ->toString()
                    .toStdString()
                    .c_str();
    std::cout << savedPort << std::endl;
    arduinoInput[i] = new SerialPort(savedPort);
    std::cout << i << "Is connected: " << arduinoInput[i]->isConnected()
              << std::endl;
  }
  bool connected = false;

  while (abortInput != true) {
    if (!connected) {
      hr = SimConnect_Open(&hInputSimConnect, "Inputs", NULL, 0, 0, 0);
      cout << hr << endl;
      if (hr == S_OK) {
        connected = true;
        handler.connect = hInputSimConnect;

        handler.object = SIMCONNECT_OBJECT_ID_USER;
        mapper.mapEvents(hInputSimConnect);

        if (!settingsHandler.retrieveSetting("ranges", "flapsmin")->isNull()) {
          for (int i = 0; i < constants::supportedEngines; i++) {
            QString minStr = "Engine " + QString::number(i + 1) + "Reverse";

            int minRange =
                settingsHandler.retrieveSetting("ranges", minStr)->toInt();
            cout << minRange << endl;

            QString idleStr =
                "Engine " + QString::number(i + 1) + "Idle cutoff";
            int idleCutoff =
                settingsHandler.retrieveSetting("ranges", idleStr)->toInt();

            QString maxStr = "Engine " + QString::number(i + 1) + "Max";
            int maxRange =
                settingsHandler.retrieveSetting("ranges", maxStr)->toInt();

            handler.enginelist[i] =
                Engine(minRange, idleCutoff, maxRange, i + 1);
          }

          if (!settingsHandler.retrieveSetting("ranges", "maxReverseRange")
                   ->isNull()) {
            handler.reverseAxis =
                settingsHandler.retrieveSetting("ranges", "maxReverseRange")
                    ->toFloat();
          }

          for (int i = 0; i < constants::supportedMixtureLevers; i++) {
            QString minStr = "Mixture " + QString::number(i + 1) + "Min";
            int minRange =
                settingsHandler.retrieveSetting("ranges", minStr)->toInt();
            cout << minRange << endl;

            QString idleStr = "Mixture " + QString::number(i + 1) + "Max";
            int maxRange =
                settingsHandler.retrieveSetting("ranges", idleStr)->toInt();

            handler.mixtureRanges[i] = Range(minRange, maxRange);
          }
          for (int i = 0; i < constants::supportedPropellerLevers; i++) {
            QString minStr = "Propeller " + QString::number(i + 1) + "Min";
            int minRange =
                settingsHandler.retrieveSetting("ranges", minStr)->toInt();

            QString idleStr = "Propeller " + QString::number(i + 1) + "Max";
            int maxRange =
                settingsHandler.retrieveSetting("ranges", idleStr)->toInt();

            handler.propellerRanges[i] = Range(minRange, maxRange);
          }
          int minFlaps =
              settingsHandler.retrieveSetting("ranges", "FlapsMin")->toInt();
          int maxFlaps =
              settingsHandler.retrieveSetting("ranges", "FlapsMax")->toInt();
          handler.flapsRange = Range(minFlaps, maxFlaps);

        } else if (settingsHandler.retrieveSetting("ranges", "FlapsMin")
                       ->isNull()) {
          for (int i = 0; i < constants::supportedEngines; i++) {
            handler.enginelist[i] = Engine(0, 0, 1023, i);
          }
          for (int i = 0; i < constants::supportedMixtureLevers; i++) {
            handler.mixtureRanges[i] = Range(0, 1023);
          }
          for (int i = 0; i < constants::supportedPropellerLevers; i++) {
            handler.propellerRanges[i] = Range(0, 1023);
          }

          handler.flapsRange = Range(0, 1023);
        }
      }
    }

    Sleep(5);
    for (int i = 0; i < keys.size(); i++) {
      cout << arduinoInput[i]->isConnected() << endl;
      const auto hasRead = arduinoInput[i]->readSerialPort(
          handler.receivedString[i], DATA_LENGTH);

      if (hasRead) {
        if (connected) {
          emit updateLastStatusUI("Connected to game");
          handler.switchHandling(i);
        }
        lastVal = handler.receivedString[i];
      }

      emit updateLastValUI(QString::fromStdString(lastVal));
    }
  }

  Sleep(1);
  // emit updateLastStatusUI("Connection closed");
  if (connected) {
    for (int i = 0; i < keys.size(); i++) {
      if (arduinoInput[i]->isConnected()) {
        arduinoInput[i]->closeSerial();
      }
    }
    quit();
    SimConnect_Close(hInputSimConnect);
  } else {
    emit updateLastStatusUI("No connection to the game");
    sleep(100);
  }
}

InputWorker::~InputWorker() {
  mutex.lock();
  abortInput = true;

  condition.wakeOne();
  mutex.unlock();
}
