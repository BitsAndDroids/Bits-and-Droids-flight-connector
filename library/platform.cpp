#include "platform.h"

#include <QString>

Platform::Platform(int baudRate, QString name, bool skipHardwareSerial) {
  this->baudRate = baudRate;
  this->name = name;
  this->skipHardwareSerial = skipHardwareSerial;
}
