#ifndef INPUTSWITCHHANDLER_H
#define INPUTSWITCHHANDLER_H
#include <qmutex.h>
#include <qsettings.h>
#include <qstandardpaths.h>
#include <qwaitcondition.h>
#include <tchar.h>
#include <windows.h>

#include <QObject>
#include <QThread>
#include <string>

#include "SimConnect.h"
#include "stdio.h"
using namespace std;
class InputSwitchHandler {
 public:
  InputSwitchHandler();
  void switchHandling(int index);
  char receivedString[10][255];
  HANDLE connect;
  SIMCONNECT_OBJECT_ID object;

 private slots:
  void set_throttle_values(int index);
  void setMixtureValues(int index);
  void set_prop_values(int index);
  int setComs(int index, int comNo);

 private:
  std::string prefix;
  void setElevatorTrim(int index);
  void sendBasicCommand(SIMCONNECT_CLIENT_EVENT_ID eventID);
};

#endif  // INPUTSWITCHHANDLER_H
