#ifndef OUTPUTBUNDLE_H
#define OUTPUTBUNDLE_H

#include <QMap>
#include "serial/SerialPort.hpp"

#include "Set.h"

class outputBundle {
 public:
  outputBundle();
  void setSet(Set set) { this->set = set; };
  void setSerialPort(SerialPort serialPort) { this->arduino = &serialPort; };
  void setOutputsInSet(QMap<int, Output *> outputs) {
    this->outputsInSet = outputs;
  };
  void setSerialPortString(const char *portString) {
    this->portString = portString;
  };
  const char *getSerialPortString() { return portString; }
  SerialPort *getSerialPort() { return this->arduino; };
  bool isOutputInBundle(int id);

 private:
  Set set;
  SerialPort *arduino;
  const char *portString;
  QMap<int, Output *> outputsInSet;
};

#endif  // OUTPUTBUNDLE_H
