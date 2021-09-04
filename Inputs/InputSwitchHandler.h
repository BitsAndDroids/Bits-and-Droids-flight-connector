#ifndef INPUTSWITCHHANDLER_H
#define INPUTSWITCHHANDLER_H
#include <headers/Engine.h>
#include <headers/constants.h>
#include <headers/range.h>
#include <qmutex.h>
#include <qsettings.h>
#include <qstandardpaths.h>
#include <qwaitcondition.h>
#include <settings/settingshandler.h>
#include <tchar.h>
#include <windows.h>

#include <QObject>
#include <QThread>
#include <string>

#include "headers/SimConnect.h"
#include <cstdio>
using namespace std;
class InputSwitchHandler {

 public:
  InputSwitchHandler();
  void switchHandling(int index);
  char receivedString[10][255];
  HANDLE connect;
  SIMCONNECT_OBJECT_ID object;
  std::array<Engine, constants::supportedEngines> enginelist;
  Range mixtureRanges[constants::supportedMixtureLevers];
  Range propellerRanges[constants::supportedPropellerLevers];
  Range flapsRange;

  float reverseAxis = -23000.0;
 private slots:
  SettingsHandler settingsHandler;
  void set_throttle_values(int index);
  void setMixtureValues(int index);
  void set_prop_values(int index);
  int setComs(int index, int comNo);

  void sendBasicCommandOn(SIMCONNECT_CLIENT_EVENT_ID eventID);
  void sendBasicCommandOff(SIMCONNECT_CLIENT_EVENT_ID eventID);

 private:
  std::string prefix;
  void setElevatorTrim(int index);
  void setFlaps(int index);
  void setRudder(int index);
  void setBrakeAxis(int index);
  void sendBasicCommandValue(SIMCONNECT_CLIENT_EVENT_ID eventID, int value);
  void controlYoke(int index);
  void sendBasicCommand(SIMCONNECT_CLIENT_EVENT_ID eventID, int index);
  int mapThrottleValueToAxis(int value, float reverse, float max,
                             int idleCutoff);
};

#endif  // INPUTSWITCHHANDLER_H
