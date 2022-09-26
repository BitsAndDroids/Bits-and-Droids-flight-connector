/*
 * Author: Manash Kumar Mandal
 * Modified Library introduced in Arduino Playground which does not work
 * This works perfectly
 * LICENSE: MIT
 */

#pragma once

#define MAX_DATA_LENGTH 25

#include "settings/settingshandler.h"
#include <windows.h>

#include <iostream>

class SerialPort {
 private:
  HANDLE handler;
  bool connected;
  COMSTAT status;
  DWORD errors;
  SettingsHandler settingsHandler;
  int arduinoWaitTime = 15;
  std::string portName;

 public:
  explicit SerialPort(const char *portName);
  ~SerialPort();

  int readSerialPort(const char *buffer, unsigned int buf_size);
  bool writeSerialPort(const char *buffer, unsigned int buf_size);
  bool isConnected();
  void closeSerial();
  std::string getPortName();
};
