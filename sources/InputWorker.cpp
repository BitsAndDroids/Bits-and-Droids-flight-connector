// BitsAndDroidsInputHandler.cpp : This file contains the 'main' function.
// Program execution begins and ends there.
//
#include <Windows.h>
#include <headers/InputMapper.h>
#include <headers/InputSwitchHandler.h>
#include <headers/InputWorker.h>
#include <headers/SerialReader.h>
#include <qsettings.h>
#include <qstandardpaths.h>
#include <tchar.h>

#include <cstdio>
#include <future>
#include <iostream>
#include <string>

#include "headers/SerialPort.hpp"
#include "headers/SimConnect.h"

#define Bcd2Dec(BcdNum) HornerScheme(BcdNum, 0x10, 10)
#define Dec2Bcd(DecNum) HornerScheme(DecNum, 10, 0x10)

#define DATA_LENGTH 255

// Declare a global object
SerialPort *arduinoInput[10];

HANDLE hInputSimConnect = NULL;

using namespace std;
InputEnum defs = InputEnum();
InputWorker::InputWorker() {}
InputMapper mapper = InputMapper();
InputSwitchHandler handler = InputSwitchHandler();

enum GROUP_ID {
  GROUP0,
};

int x = 0;
enum DATA_REQUEST_ID {
  REQUEST_THROTTLE,
  REQUEST_MIXTURE,
  REQUEST_PROPS,
};

void InputWorker::MyDispatchProcInput(SIMCONNECT_RECV *pData, DWORD cbData,
                                      void *pContext) {
  switch (pData->dwID) {
    case SIMCONNECT_RECV_ID_QUIT: {
      // quit = 1;
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
  settings->beginGroup("inputCom");
  string val = settings->value("inputComActiveBase").toString().toStdString();
  const char *valPort = val.c_str();
  std::cout << valPort << std::endl;
  settings->endGroup();
  settings->sync();
  HRESULT hr;
  settings->beginGroup("inputCom");
  int inputs = settings->value("inputCounter").toInt();
  if (inputs == NULL) {
    inputs = 1;
  }
  int extraRows = inputs - 1;
  const char *savedPort;
  arduinoInput[0] = new SerialPort(valPort);
  for (int i = 1; i < inputs; i++) {
    QString comName = "inputCom" + QString::number(i);
    savedPort = settings->value(comName).toString().toStdString().c_str();
    std::cout << savedPort << std::endl;
    arduinoInput[i] = new SerialPort(savedPort);
    std::cout << i << "Is connected: " << arduinoInput[i]->isConnected()
              << std::endl;
  }
  settings->endGroup();

  bool connected = false;

  while (abortInput != true) {
    if (!connected) {
      hr = SimConnect_Open(&hInputSimConnect, "Input Event", NULL, 0, 0, 0);
      cout << hr << endl;
      if (hr == S_OK) {
        connected = true;
        handler.connect = hInputSimConnect;
        handler.object = SIMCONNECT_OBJECT_ID_USER;
        SIMCONNECT_OBJECT_ID objectID = SIMCONNECT_OBJECT_ID_USER;
        SimConnect_SetNotificationGroupPriority(
            hInputSimConnect, GROUP0, SIMCONNECT_GROUP_PRIORITY_HIGHEST);
        SimConnect_SetInputGroupState(hInputSimConnect, INPUT0,
                                      SIMCONNECT_STATE_ON);
        mapper.mapEvents(hInputSimConnect);
      }
    }

    Sleep(5);
    for (int i = 0; i <= extraRows; i++) {
      const auto hasRead = arduinoInput[i]->readSerialPort(
          handler.receivedString[i], DATA_LENGTH);
      if (hasRead) {
        if (connected) {
          emit updateLastStatusUI("Connected to game");
          handler.switchHandling(i);
        }
        lastVal = handler.receivedString[i];
        emit updateLastValUI(QString::fromStdString(lastVal));
      }
    }
  }

  Sleep(1);
  emit updateLastStatusUI("Connection closed");
  if (connected) {
    arduinoInput[0]->closeSerial();
    for (int i = 1; i <= extraRows; i++) {
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
