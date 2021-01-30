// BitsAndDroidsInputHandler.cpp : This file contains the 'main' function.
// Program execution begins and ends there.
//

#include <Windows.h>
#include <headers/InputWorker.h>
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

// com1
int firstFreq1 = 24;
int lastFreq1 = 85;
// com2
int firstFreq2 = 24;
int lastFreq2 = 85;
// nav1

int firstFreqNav1 = 11;
int lastFreqNav1 = 8;
// nav2
int firstFreqNav2 = 11;
int lastFreqNav2 = 8;

char *token, *next_token;
bool inputs[5];

int counter = 0;
int engines = 4;
// Declare a global object
SerialPort *arduinoInput;

double oldValThrottle[4];
double oldValMixture = 0;

double oldValProps[2] = {-10, -10};

char receivedString[DATA_LENGTH];
HANDLE hInputSimConnect = NULL;

using namespace std;

InputWorker::InputWorker() {}

enum GROUP_ID {
  GROUP0,
};

UINT32 InputWorker::HornerScheme(UINT32 Num, UINT32 Divider, UINT32 Factor) {
  UINT32 Remainder = 0, Quotient = 0, Result = 0;
  Remainder = Num % Divider;
  Quotient = Num / Divider;
  if (!(Quotient == 0 && Remainder == 0))
    Result += HornerScheme(Quotient, Divider, Factor) * Factor + Remainder;
  return Result;
}
enum EVENT_ID {
  EVENT_ENGINE_AUTO_SHUTDOWN,
};

enum DATA_DEFINE_ID_INPUT {
  // Engine
  DEFINITION_THROTTLE_1,
  DEFINITION_THROTTLE_2,
  DEFINITION_THROTTLE_3,
  DEFINITION_THROTTLE_4,
  DEFINITION_MIXTURE_1,
  DEFINITION_PROPELLER_LEVER_POSITION_1,
  DEFINITION_PROPELLER_LEVER_POSITION_2,

  // Coms
  DEFINITION_COM_1_SET,
  DEFINITION_COM_2_SET,
  DEFINITION_NAV_1,
  DEFINITION_NAV_2,
  DEFINITION_COM_STANDBY_SWAP,
  DEFINITION_COM_2_STANDBY_SWAP,
  DEFINITION_NAV_1_STANDBY_SWAP,
  DEFINITION_NAV_2_STANDBY_SWAP,
  DEFINITION_COM_RADIO_FRACT_DEC,
  DEFINITION_COM_RADIO_FRACT_INC,
  DEFINITION_COM2_RADIO_FRACT_DEC,
  DEFINITION_COM2_RADIO_FRACT_INC,
  DEFINITION_COM_RADIO_FRACT_DEC_CARRY,
  DEFiNITION_COM_RADIO_FRACT_INC_CARRY,
  DEFINITION_COM2_RADIO_FRACT_DEC_CARRY,
  DEFINITION_COM2_RADIO_FRACT_INC_CARRY,
  DEFINITION_COM_RADIO_FRACT_INC_CARRY,

  // Nav
  DEFINITION_NAV_1_RADIO_FRACT_INC,
  DEFINITION_NAV_1_RADIO_FRACT_DEC,
  DEFINITION_NAV_2_RADIO_FRACT_INC,
  DEFINITION_NAV_2_RADIO_FRACT_DEC,
  DEFINITION_NAV_1_RADIO_WHOLE_INC,
  DEFINITION_NAV_1_RADIO_WHOLE_DEC,
  DEFINITION_NAV_2_RADIO_WHOLE_INC,
  DEFINITION_NAV_2_RADIO_WHOLE_DEC,

  // DME
  DEFINITION_RADIO_SELECTED_DME1_IDENT_ENABLE,
  DEFINITION_RADIO_SELECTED_DME1_IDENT_DISABLE,
  DEFINITION_RADIO_SELECTED_DME1_IDENT_SET,
  DEFINITION_RADIO_SELECTED_DM1E_IDENT_TOGGLE,
  DEFINITION_RADIO_SELECTED_DME2_IDENT_ENABLE,
  DEFINITION_RADIO_SELECTED_DME2_IDENT_DISABLE,
  DEFINITION_RADIO_SELECTED_DME2_IDENT_SET,
  DEFINITION_RADIO_SELECTED_DME2_IDENT_TOGGLE,
  DEFINITION_TOGGLE_DME,

  // VOR
  DEFINITION_RADIO_SELECTED_VOR1_IDENT_ENABLE,
  DEFINITION_RADIO_SELECTED_VOR1_IDENT_DISABLE,
  DEFINITION_RADIO_SELECTED_VOR1_IDENT_SET,
  DEFINITION_RADIO_SELECTED_VORE_IDENT_TOGGLE,
  DEFINITION_RADIO_SELECTED_VOR2_IDENT_ENABLE,
  DEFINITION_RADIO_SELECTED_VOR2_IDENT_DISABLE,
  DEFINITION_RADIO_SELECTED_VOR2_IDENT_SET,
  DEFINITION_RADIO_SELECTED_VOR2_IDENT_TOGGLE,
  DEFINITION_TOGGLE_VOR,
  DEFINITION_VOR1_OBI_DEC,
  DEFINITION_VOR1_OBI_INC,
  DEFINITION_VOR2_OBI_DEC,
  DEFINITION_VOR2_OBI_INC,

  // ADF
  DEFINITION_RADIO_SELECTED_ADF_IDENT_ENABLE,
  DEFINITION_RADIO_SELECTED_ADF_IDENT_DISABLE,
  DEFINITION_RADIO_SELECTED_ADF_IDENT_SET,
  DEFINITION_RADIO_SELECTED_ADF_IDENT_TOGGLE,
  DEFINITION_ADF_CARD_INC,
  DEFINITION_ADF_CARD_DEC,
  DEFINITION_ADF_CARD_SET,
  DEFINITION_ADF_100_INC,
  DEFINITION_ADF_10_INC,
  DEFINITION_ADF_1_INC,
  DEFINITION_ADF_100_DEC,
  DEFINITION_ADF_10_DEC,
  DEFINITION_ADF_1_DEC,
  DEFINITION_ADF_COMPLETE_SET,
  DEFINITION_ADF1_WHOLE_INC,
  DEFINITION_ADF1_WHOLE_DEC,
  DEFINITION_ADF2_100_INC,
  DEFINITION_ADF2_10_INC,
  DEFINITION_ADF2_1_INC,
  DEFINITION_ADF2_RADIO_TENTHS_INC,
  DEFINITION_ADF2_100_DEC,
  DEFINITION_ADF2_10_DEC,
  DEFINITION_ADF2_1_DEC,
  DEFINITION_ADF2_RADIO_TENTHS_DEC,
  DEFINITION_ADF2_WHOLE_INC,
  DEFINITION_ADF2_WHOLE_DEC,
  DEFINITION_ADF2_FRACT_DEC_CARRY,
  DEFINITION_ADF2_FRACT_INC_CARRY,
  DEFINITION_ADF2_COMPLETE_SET,
  DEFINITION_RADIO_ADF2_IDENT_DISABLE,
  DEFINITION_RADIO_ADF2_IDENT_ENABLE,
  DEFINITION_RADIO_ADF2_IDENT_TOGGLE,
  DEFINITION_RADIO_ADF2_IDENT_SET,

  // XPNDR
  DEFINITION_XPNDR_1000_INC,
  DEFINITION_XPNDR_100_INC,
  DEFINITION_XPNDR_10_INC,
  DEFINITION_XPNDR_1_INC,
  DEFINITION_XPNDR_1000_DEC,
  DEFINITION_XPNDR_100_DEC,
  DEFINITION_XPNDR_10_DEC,
  DEFINITION_XPNDR_1_DEC,
  DEFINITION_XPNDR_SET,

  // Avionics
  DEFINITION_TOGGLE_AVIONICS1_MASTER_ON,
  DEFINITION_TOGGLE_AVIONICS1_MASTER_OFF,
  DEFINITION_TOGGLE_AVIONICS2_MASTER_ON,
  DEFINITION_TOGGLE_AVIONICS2_MASTER_OFF,
  DEFINITION_BAT_1_ON,
  DEFINITION_BAT_2_ON,
  DEFINITION_BAT_1_OFF,
  DEFINITION_BAT_2_OFF,
  DEFINITION_PARKING_BRAKE,

  // Battery
  DEFINITION_TOGGLE_MASTER_1_BATTERY,
  DEFINITION_TOGGLE_MASTER_2_BATTERY,
  DEFINITION_TOGGLE_EXTERNAL_POWER,
  DEFINITION_TOGGLE_MASTER_BATTERY,
  DEFINITION_TOGGLE_MASTER_ALTERNATOR,

  // G1000
  DEFINITION_G1000_MFD_ZOOMIN_BUTTON,
  DEFINITION_G1000_MFD_ZOOMOUT_BUTTON,

  // AP
  DEFINITION_AP_MASTER,
  DEFINITION_AP_PANEL_HEADING_HOLD,
  DEFINITION_AP_PANEL_ALTITUDE_HOLD,
  DEFINITION_AP_ATT_HOLD_ON,
  DEFINITION_AP_LOC_HOLD_ON,
  DEFINITION_AP_APR_HOLD_ON,
  DEFINITION_AP_HDG_HOLD_ON,
  DEFINITION_AP_ALT_HOLD_ON,
  DEFINITION_AP_WING_LEVELER_ON,
  DEFINITION_AP_BC_HOLD_ON,
  DEFINITION_AP_NAV1_HOLD_ON,
  DEFINITION_AP_ATT_HOLD_OFF,
  DEFINITION_AP_LOC_HOLD_OFF,
  DEFINITION_AP_APR_HOLD_OFF,
  DEFINITION_AP_HDG_HOLD_OFF,
  DEFINITION_AP_ALT_HOLD_OFF,
  DEFINITION_AP_WING_LEVELER_OFF,
  DEFINITION_AP_BC_HOLD_OFF,
  DEFINITION_AP_NAV1_HOLD_OFF,
  DEFINITION_AP_AIRSPEED_HOLD,
  DEFINITION_AP_PANEL_SPEED_HOLD,
  DEFINITION_AP_ALT_VAR_INC,
  DEFINITION_AP_ALT_VAR_DEC,
  DEFINITION_AP_VS_VAR_INC,
  DEFINITION_AP_VS_VAR_DEC,
  DEFINITION_AP_SPD_VAR_INC,
  DEFINITION_AP_SPD_VAR_DEC,
  DEFINITION_AP_PANEL_MACH_HOLD,
  DEFINITION_AP_MACH_VAR_INC,
  DEFINITION_AP_MACH_VAR_DEC,
  DEFINITION_AP_MACH_HOLD,
  DEFINITION_AP_ALT_VAR_SET_METRIC,
  DEFINITION_AP_VS_VAR_SET_ENGLISH,
  DEFINITION_AP_SPD_VAR_SET,
  DEFINITION_AP_MACH_VAR_SET,
  DEFINITION_AP_AIRSPEED_ON,
  DEFINITION_AP_AIRSPEED_OFF,
  DEFINITION_AP_AIRSPEED_SET,
  DEFINITION_AP_MACH_ON,
  DEFINITION_AP_MACH_OFF,
  DEFINITION_AP_MACH_SET,
  DEFINITION_AP_PANEL_ALTITUDE_ON,
  DEFINITION_AP_PANEL_ALTITUDE_OFF,
  DEFINITION_AP_PANEL_ALTITUDE_SET,
  DEFINITION_AP_PANEL_HEADING_ON,
  DEFINITION_AP_PANEL_HEADING_OFF,
  DEFINITION_AP_PANEL_HEADING_SET,
  DEFINITION_AP_PANEL_MACH_ON,
  DEFINITION_AP_PANEL_MACH_OFF,
  DEFINITION_AP_PANEL_MACH_SET,
  DEFINITION_AP_PANEL_SPEED_ON,
  DEFINITION_AP_PANEL_SPEED_OFF,
  DEFINITION_AP_PANEL_SPEED_SET,
  DEFINITION_AP_ALT_VAR_SET_ENGLISH,
  DEFINITION_AP_VS_VAR_SET_METRIC,
  DEFINITION_AP_PANEL_SPEED_HOLD_TOGGLE,
  DEFINITION_AP_PANEL_MACH_HOLD_TOGGLE,
  DEFINITION_AP_NAV_SELECT_SET,
  DEFINITION_AP_PITCH_REF_INC_UP,
  DEFINITION_AP_PITCH_REF_INC_DN,
  DEFINITION_AP_PITCH_REF_SELECT,
  DEFINITION_AP_ATT_HOLD,
  DEFINITION_AP_LOC_HOLD,
  DEFINITION_AP_APR_HOLD,
  DEFINITION_AP_HDG_HOLD,
  DEFINITION_AP_ALT_HOLD,
  DEFINITION_AP_WING_LEVELER,
  DEFINITION_AP_BC_HOLD,
  DEFINITION_AP_NAV1_HOLD,
  DEFINITION_HEADING_BUG_INC,
  DEFINITION_HEADING_BUG_DEC,
  DEFINITION_TOGGLE_FLIGHT_DIRECTOR,
  DEFINITION_AP_VS_HOLD,
  DEFINITION_KOHLSMAN_INC,
  DEFINITION_KOHLSMAN_DEC,
  DEFINITION_AUTO_THROTTLE_ARM,
  DEFINITION_AUTO_THROTTLE_TO_GA,

  // Flaps
  DEFINITION_FLAPS_UP,
  DEFINITION_FLAPS_1,
  DEFINITION_FLAPS_2,
  DEFINITION_FLAPS_3,
  DEFINITION_FLAPS_DOWN,
  DEFINITION_FLAPS_INCR,
  DEFINITION_FLAPS_DECR,

};
int x = 0;
enum DATA_REQUEST_ID {
  REQUEST_THROTTLE,
  REQUEST_MIXTURE,
  REQUEST_PROPS,
};
struct structBattery1 {
  bool battery1On = true;
};
struct structBattery2 {
  bool battery2On = true;
};

struct structPropControl {
  double prop_percent[2] = {0, 0};
};

struct structThrottleControl {
  double throttle_percent[4] = {0, 0, 0, 0};
};

struct structMixtureControl {
  double mixture_percent;
};

structMixtureControl mc;
structThrottleControl tc;
structPropControl pc;
structBattery1 bc1;
structBattery2 bc2;

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

void set_throttle_values() {
  // Throttle control
  try {
    token = strtok_s(receivedString, " ", &next_token);

    counter = 0;

    while (token != nullptr && counter < 6) {
      cout << receivedString << ":Received" << endl;
      cout << token << ":Counter" << counter << endl;

      if (token != nullptr) {
        const auto incVal = strtod(token, nullptr);

        if (counter == 5) {
          std::cout << incVal << std::endl;
          if (incVal < 10) {
            if (oldValMixture < 20.0) {
              oldValMixture = incVal;
            }
          } else {
            oldValMixture = incVal;
          }
          mc.mixture_percent = oldValMixture;
          cout << "counter: " << counter << " val: " << incVal << endl;
        }

        if (counter != 0 && counter != 5) {
          std::cout << incVal << std::endl;
          if (incVal < 10.0) {
            if (oldValThrottle[counter - 1] < 20.0) {
              oldValThrottle[counter - 1] = incVal;
            }
          } else {
            oldValThrottle[counter - 1] = incVal;
          }
          tc.throttle_percent[counter - 1] = oldValThrottle[counter - 1];
          cout << "counter: " << counter << " val: " << incVal << endl;
        }

        token = strtok_s(nullptr, " ", &next_token);

        counter++;
      }
    }
  } catch (const std::exception &e) {
    cout << "error in throttle" << endl;
  }
}

void set_prop_values() {
  // Throttle control
  try {
    token = strtok_s(receivedString, " ", &next_token);

    counter = 0;

    while (token != nullptr && counter < 3) {
      cout << receivedString << ":Received" << endl;
      cout << token << ":Counter" << counter << endl;

      if (token != nullptr) {
        auto incVal = strtod(token, nullptr);

        if (counter != 0) {
          std::cout << incVal << std::endl;
          if (incVal < 10.0) {
            if (oldValProps[counter - 1] < 20.0) {
              oldValProps[counter - 1] = incVal;
            }
          } else {
            if (incVal == 98) {
              incVal = 100;
            }
            oldValProps[counter - 1] = incVal;
          }
          pc.prop_percent[counter - 1] = oldValProps[counter - 1];
          cout << "counter: " << counter << " val: " << incVal << endl;
        }

        token = strtok_s(nullptr, " ", &next_token);

        counter++;
      }
    }
  } catch (const std::exception &e) {
    cout << "error in throttle" << endl;
  }
}
/*!
 * \fn void InputWorker::inputEvents()
 */
void InputWorker::inputEvents() {
  QString path =
      QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
  QSettings settings(path + "/" + "settings.ini", QSettings::IniFormat);
  settings.beginGroup("Coms");
  string val = settings.value("comActiveInput1").toString().toStdString();
  const char *valPort = val.c_str();
  settings.endGroup();
  settings.sync();

  HRESULT hr;

  if (SUCCEEDED(
          SimConnect_Open(&hInputSimConnect, "Input Event", NULL, 0, 0, 0))) {
    SIMCONNECT_OBJECT_ID objectID = SIMCONNECT_OBJECT_ID_USER;
    hr = SimConnect_SetNotificationGroupPriority(
        hInputSimConnect, GROUP0, SIMCONNECT_GROUP_PRIORITY_HIGHEST);
    hr = SimConnect_SetInputGroupState(hInputSimConnect, INPUT0,
                                       SIMCONNECT_STATE_ON);

    if (advanced) {
      hr = SimConnect_AddToDataDefinition(
          hInputSimConnect, DEFINITION_MIXTURE_1,
          "GENERAL ENG MIXTURE LEVER POSITION:1", "percent");
    }

    if (props) {
      hr = SimConnect_AddToDataDefinition(
          hInputSimConnect, DEFINITION_PROPELLER_LEVER_POSITION_1,
          "GENERAL ENG PROPELLER LEVER POSITION:1", "percent");
      hr = SimConnect_AddToDataDefinition(
          hInputSimConnect, DEFINITION_PROPELLER_LEVER_POSITION_2,
          "GENERAL ENG PROPELLER LEVER POSITION:2", "percent");
    }

    // Avionics
    hr = SimConnect_MapClientEventToSimEvent(
        hInputSimConnect, DEFINITION_TOGGLE_AVIONICS1_MASTER_ON,
        "AVIONICS_MASTER_1_SET");
    hr = SimConnect_MapClientEventToSimEvent(
        hInputSimConnect, DEFINITION_TOGGLE_AVIONICS1_MASTER_OFF,
        "AVIONICS_MASTER_1_SET");
    hr = SimConnect_MapClientEventToSimEvent(
        hInputSimConnect, DEFINITION_TOGGLE_AVIONICS2_MASTER_ON,
        "AVIONICS_MASTER_2_SET");
    hr = SimConnect_MapClientEventToSimEvent(
        hInputSimConnect, DEFINITION_TOGGLE_AVIONICS2_MASTER_OFF,
        "AVIONICS_MASTER_2_SET");
    hr = SimConnect_MapClientEventToSimEvent(
        hInputSimConnect, DEFINITION_TOGGLE_MASTER_ALTERNATOR,
        "TOGGLE_MASTER_ALTERNATOR");
    hr = SimConnect_MapClientEventToSimEvent(
        hInputSimConnect, DEFINITION_PARKING_BRAKE, "PARKING BRAKE");

    // Battery
    hr = SimConnect_AddToDataDefinition(hInputSimConnect, DEFINITION_BAT_1_ON,
                                        "ELECTRICAL MASTER BATTERY:1", "Bool",
                                        SIMCONNECT_DATATYPE_INT32);
    hr = SimConnect_AddToDataDefinition(hInputSimConnect, DEFINITION_BAT_2_ON,
                                        "ELECTRICAL MASTER BATTERY:2", "Bool");
    hr = SimConnect_MapClientEventToSimEvent(hInputSimConnect,
                                             DEFINITION_TOGGLE_MASTER_BATTERY,
                                             "TOGGLE_MASTER_BATTERY");
    hr = SimConnect_MapClientEventToSimEvent(
        hInputSimConnect, DEFINITION_TOGGLE_MASTER_ALTERNATOR,
        "TOGGLE_MASTER_BATTERY_ALTERNATOR");
    hr = SimConnect_MapClientEventToSimEvent(hInputSimConnect,
                                             DEFINITION_TOGGLE_EXTERNAL_POWER,
                                             "TOGGLE_EXTERNAL_POWER");
    // Engines
    hr = SimConnect_AddToDataDefinition(hInputSimConnect, DEFINITION_THROTTLE_1,
                                        "GENERAL ENG THROTTLE LEVER POSITION:1",
                                        "percent");
    hr = SimConnect_AddToDataDefinition(hInputSimConnect, DEFINITION_THROTTLE_2,
                                        "GENERAL ENG THROTTLE LEVER POSITION:2",
                                        "percent");
    hr = SimConnect_AddToDataDefinition(hInputSimConnect, DEFINITION_THROTTLE_3,
                                        "GENERAL ENG THROTTLE LEVER POSITION:3",
                                        "percent");
    hr = SimConnect_AddToDataDefinition(hInputSimConnect, DEFINITION_THROTTLE_4,
                                        "GENERAL ENG THROTTLE LEVER POSITION:4",
                                        "percent");
    // AP
    hr = SimConnect_MapClientEventToSimEvent(hInputSimConnect,
                                             DEFINITION_AP_MASTER, "AP_MASTER");
    hr = SimConnect_MapClientEventToSimEvent(hInputSimConnect,
                                             DEFINITION_AP_PANEL_HEADING_HOLD,
                                             "AP_PANEL_HEADING_HOLD");
    hr = SimConnect_MapClientEventToSimEvent(hInputSimConnect,
                                             DEFINITION_AP_PANEL_ALTITUDE_HOLD,
                                             "AP_PANEL_ALTITUDE_HOLD");
    hr = SimConnect_MapClientEventToSimEvent(
        hInputSimConnect, DEFINITION_AP_ATT_HOLD_ON, "AP_ATT_HOLD_ON");
    hr = SimConnect_MapClientEventToSimEvent(
        hInputSimConnect, DEFINITION_AP_LOC_HOLD_ON, "AP_LOC_HOLD_ON");

    hr = SimConnect_MapClientEventToSimEvent(
        hInputSimConnect, DEFINITION_AP_APR_HOLD_ON, "AP_APR_HOLD_ON");
    hr = SimConnect_MapClientEventToSimEvent(
        hInputSimConnect, DEFINITION_AP_HDG_HOLD_ON, "AP_HDG_HOLD_ON");
    hr = SimConnect_MapClientEventToSimEvent(
        hInputSimConnect, DEFINITION_AP_ALT_HOLD_ON, "AP_ALT_HOLD_ON");
    hr = SimConnect_MapClientEventToSimEvent(
        hInputSimConnect, DEFINITION_AP_WING_LEVELER_ON, "AP_WING_LEVELER_ON");
    hr = SimConnect_MapClientEventToSimEvent(
        hInputSimConnect, DEFINITION_AP_BC_HOLD_ON, "AP_BC_HOLD_ON");
    hr = SimConnect_MapClientEventToSimEvent(
        hInputSimConnect, DEFINITION_AP_NAV1_HOLD_ON, "AP_NAV1_HOLD_ON");
    hr = SimConnect_MapClientEventToSimEvent(
        hInputSimConnect, DEFINITION_AP_ATT_HOLD_OFF, "AP_ATT_HOLD_OFF");
    hr = SimConnect_MapClientEventToSimEvent(
        hInputSimConnect, DEFINITION_AP_LOC_HOLD_OFF, "AP_LOC_HOLD_OFF");
    hr = SimConnect_MapClientEventToSimEvent(
        hInputSimConnect, DEFINITION_AP_APR_HOLD_OFF, "AP_APR_HOLD_OFF");
    hr = SimConnect_MapClientEventToSimEvent(
        hInputSimConnect, DEFINITION_AP_HDG_HOLD_OFF, "AP_HDG_HOLD_OFF");
    hr = SimConnect_MapClientEventToSimEvent(
        hInputSimConnect, DEFINITION_AP_ALT_HOLD_OFF, "AP_ALT_HOLD_OFF");
    hr = SimConnect_MapClientEventToSimEvent(hInputSimConnect,
                                             DEFINITION_AP_WING_LEVELER_OFF,
                                             "AP_WING_LEVELER_OFF");
    hr = SimConnect_MapClientEventToSimEvent(
        hInputSimConnect, DEFINITION_AP_BC_HOLD_OFF, "AP_BC_HOLD_OFF");
    hr = SimConnect_MapClientEventToSimEvent(
        hInputSimConnect, DEFINITION_AP_NAV1_HOLD_OFF, "AP_NAV1_HOLD_OFF");
    hr = SimConnect_MapClientEventToSimEvent(
        hInputSimConnect, DEFINITION_AP_AIRSPEED_HOLD, "AP_AIRSPEED_HOLD");
    hr = SimConnect_MapClientEventToSimEvent(hInputSimConnect,
                                             DEFINITION_AP_PANEL_SPEED_HOLD,
                                             "AP_PANEL_SPEED_HOLD");
    hr = SimConnect_MapClientEventToSimEvent(
        hInputSimConnect, DEFINITION_AP_ALT_VAR_INC, "AP_ALT_VAR_INC");
    hr = SimConnect_MapClientEventToSimEvent(
        hInputSimConnect, DEFINITION_AP_ALT_VAR_DEC, "AP_ALT_VAR_DEC");
    hr = SimConnect_MapClientEventToSimEvent(
        hInputSimConnect, DEFINITION_AP_VS_VAR_INC, "AP_VS_VAR_INC");
    hr = SimConnect_MapClientEventToSimEvent(
        hInputSimConnect, DEFINITION_AP_VS_VAR_DEC, "AP_VS_VAR_DEC");
    hr = SimConnect_MapClientEventToSimEvent(
        hInputSimConnect, DEFINITION_AP_SPD_VAR_INC, "AP_SPD_VAR_INC");
    hr = SimConnect_MapClientEventToSimEvent(
        hInputSimConnect, DEFINITION_AP_SPD_VAR_DEC, "AP_SPD_VAR_DEC");
    hr = SimConnect_MapClientEventToSimEvent(
        hInputSimConnect, DEFINITION_AP_PANEL_MACH_HOLD, "AP_PANEL_MACH_HOLD");
    hr = SimConnect_MapClientEventToSimEvent(
        hInputSimConnect, DEFINITION_AP_MACH_VAR_INC, "AP_MACH_VAR_INC");
    hr = SimConnect_MapClientEventToSimEvent(
        hInputSimConnect, DEFINITION_AP_MACH_VAR_DEC, "AP_MACH_VAR_DEC");
    hr = SimConnect_MapClientEventToSimEvent(
        hInputSimConnect, DEFINITION_AP_MACH_HOLD, "AP_MACH_HOLD");
    hr = SimConnect_MapClientEventToSimEvent(hInputSimConnect,
                                             DEFINITION_AP_ALT_VAR_SET_METRIC,
                                             "AP_ALT_VAR_SET_METRIC");
    hr = SimConnect_MapClientEventToSimEvent(hInputSimConnect,
                                             DEFINITION_AP_VS_VAR_SET_ENGLISH,
                                             "AP_VS_VAR_SET_ENGLISH");
    hr = SimConnect_MapClientEventToSimEvent(
        hInputSimConnect, DEFINITION_AP_SPD_VAR_SET, "AP_SPD_VAR_SET");
    hr = SimConnect_MapClientEventToSimEvent(
        hInputSimConnect, DEFINITION_AP_MACH_VAR_SET, "AP_MACH_VAR_SET");
    hr = SimConnect_MapClientEventToSimEvent(
        hInputSimConnect, DEFINITION_AP_AIRSPEED_ON, "AP_AIRSPEED_ON");
    hr = SimConnect_MapClientEventToSimEvent(
        hInputSimConnect, DEFINITION_AP_AIRSPEED_OFF, "AP_AIRSPEED_OFF");
    hr = SimConnect_MapClientEventToSimEvent(
        hInputSimConnect, DEFINITION_AP_AIRSPEED_SET, "AP_AIRSPEED_SET");
    hr = SimConnect_MapClientEventToSimEvent(
        hInputSimConnect, DEFINITION_AP_MACH_ON, "AP_MACH_ON");
    hr = SimConnect_MapClientEventToSimEvent(
        hInputSimConnect, DEFINITION_AP_MACH_OFF, "AP_MACH_OFF");
    hr = SimConnect_MapClientEventToSimEvent(
        hInputSimConnect, DEFINITION_AP_MACH_SET, "AP_MACH_SET");
    hr = SimConnect_MapClientEventToSimEvent(hInputSimConnect,
                                             DEFINITION_AP_PANEL_ALTITUDE_ON,
                                             "AP_PANEL_ALTITUDE_ON");
    hr = SimConnect_MapClientEventToSimEvent(hInputSimConnect,
                                             DEFINITION_AP_PANEL_ALTITUDE_OFF,
                                             "AP_PANEL_ALTITUDE_OFF");
    hr = SimConnect_MapClientEventToSimEvent(
        hInputSimConnect, DEFINITION_AP_PANEL_ALTITUDE_SET, "");
    hr = SimConnect_MapClientEventToSimEvent(hInputSimConnect,
                                             DEFINITION_AP_PANEL_HEADING_ON,
                                             "AP_PANEL_HEADING_ON");
    hr = SimConnect_MapClientEventToSimEvent(hInputSimConnect,
                                             DEFINITION_AP_PANEL_HEADING_OFF,
                                             "AP_PANEL_HEADING_OFF");
    hr = SimConnect_MapClientEventToSimEvent(hInputSimConnect,
                                             DEFINITION_AP_PANEL_HEADING_SET,
                                             "AP_PANEL_HEADING_SET");
    hr = SimConnect_MapClientEventToSimEvent(
        hInputSimConnect, DEFINITION_AP_PANEL_MACH_ON, "AP_PANEL_MACH_ON");
    hr = SimConnect_MapClientEventToSimEvent(
        hInputSimConnect, DEFINITION_AP_PANEL_MACH_OFF, "AP_PANEL_MACH_OFF");
    hr = SimConnect_MapClientEventToSimEvent(
        hInputSimConnect, DEFINITION_AP_PANEL_MACH_SET, "AP_PANEL_MACH_SET");
    hr = SimConnect_MapClientEventToSimEvent(
        hInputSimConnect, DEFINITION_AP_PANEL_SPEED_ON, "AP_PANEL_SPEED_ON");
    hr = SimConnect_MapClientEventToSimEvent(
        hInputSimConnect, DEFINITION_AP_PANEL_SPEED_OFF, "AP_PANEL_SPEED_OFF");
    hr = SimConnect_MapClientEventToSimEvent(
        hInputSimConnect, DEFINITION_AP_PANEL_SPEED_SET, "AP_PANEL_SPEED_SET");
    hr = SimConnect_MapClientEventToSimEvent(hInputSimConnect,
                                             DEFINITION_AP_ALT_VAR_SET_ENGLISH,
                                             "AP_ALT_VAR_SET_ENGLISH");
    hr = SimConnect_MapClientEventToSimEvent(hInputSimConnect,
                                             DEFINITION_AP_VS_VAR_SET_METRIC,
                                             "AP_VS_VAR_SET_METRIC");
    hr = SimConnect_MapClientEventToSimEvent(
        hInputSimConnect, DEFINITION_AP_PANEL_SPEED_HOLD_TOGGLE,
        "AP_PANEL_SPEED_HOLD_TOGGLE");
    hr = SimConnect_MapClientEventToSimEvent(
        hInputSimConnect, DEFINITION_AP_PANEL_MACH_HOLD_TOGGLE,
        "AP_PANEL_MACH_HOLD_TOGGLE");
    hr = SimConnect_MapClientEventToSimEvent(
        hInputSimConnect, DEFINITION_AP_NAV_SELECT_SET, "AP_NAV_SELECT_SET");
    hr = SimConnect_MapClientEventToSimEvent(hInputSimConnect,
                                             DEFINITION_AP_PITCH_REF_INC_UP,
                                             "AP_PITCH_REF_INC_UP");
    hr = SimConnect_MapClientEventToSimEvent(hInputSimConnect,
                                             DEFINITION_AP_PITCH_REF_INC_DN,
                                             "AP_PITCH_REF_INC_DN");
    hr = SimConnect_MapClientEventToSimEvent(hInputSimConnect,
                                             DEFINITION_AP_PITCH_REF_SELECT,
                                             "AP_PITCH_REF_SELECT");
    hr = SimConnect_MapClientEventToSimEvent(
        hInputSimConnect, DEFINITION_AP_ATT_HOLD, "AP_ATT_HOLD");
    hr = SimConnect_MapClientEventToSimEvent(
        hInputSimConnect, DEFINITION_AP_LOC_HOLD, "AP_LOC_HOLD");
    hr = SimConnect_MapClientEventToSimEvent(
        hInputSimConnect, DEFINITION_AP_APR_HOLD, "AP_APR_HOLD");
    hr = SimConnect_MapClientEventToSimEvent(
        hInputSimConnect, DEFINITION_AP_HDG_HOLD, "AP_HDG_HOLD");
    hr = SimConnect_MapClientEventToSimEvent(
        hInputSimConnect, DEFINITION_AP_ALT_HOLD, "AP_ALT_HOLD");
    hr = SimConnect_MapClientEventToSimEvent(
        hInputSimConnect, DEFINITION_AP_WING_LEVELER, "AP_WING_LEVELER");
    hr = SimConnect_MapClientEventToSimEvent(
        hInputSimConnect, DEFINITION_AP_BC_HOLD, "AP_BC_HOLD");
    hr = SimConnect_MapClientEventToSimEvent(
        hInputSimConnect, DEFINITION_AP_NAV1_HOLD, "AP_NAV1_HOLD");
    hr = SimConnect_MapClientEventToSimEvent(
        hInputSimConnect, DEFINITION_HEADING_BUG_INC, "HEADING_BUG_INC");
    hr = SimConnect_MapClientEventToSimEvent(
        hInputSimConnect, DEFINITION_HEADING_BUG_DEC, "HEADING_BUG_DEC");
    hr = SimConnect_MapClientEventToSimEvent(hInputSimConnect,
                                             DEFINITION_TOGGLE_FLIGHT_DIRECTOR,
                                             "TOGGLE_FLIGHT_DIRECTOR");
    hr = SimConnect_MapClientEventToSimEvent(
        hInputSimConnect, DEFINITION_AP_VS_HOLD, "AP_VS_HOLD");
    hr = SimConnect_MapClientEventToSimEvent(
        hInputSimConnect, DEFINITION_KOHLSMAN_INC, "KOHLSMAN_INC");
    hr = SimConnect_MapClientEventToSimEvent(
        hInputSimConnect, DEFINITION_KOHLSMAN_DEC, "KOHLSMAN_DEC");
    hr = SimConnect_MapClientEventToSimEvent(
        hInputSimConnect, DEFINITION_AUTO_THROTTLE_ARM, "AUTO_THROTTLE_ARM");
    hr = SimConnect_MapClientEventToSimEvent(hInputSimConnect,
                                             DEFINITION_AUTO_THROTTLE_TO_GA,
                                             "AUTO_THROTTLE_TO_GA");

    // Coms
    hr = SimConnect_MapClientEventToSimEvent(
        hInputSimConnect, DEFINITION_COM_1_SET, "COM_STBY_RADIO_SET");
    hr = SimConnect_MapClientEventToSimEvent(
        hInputSimConnect, DEFINITION_COM_2_SET, "COM_STBY_RADIO_SET_2");

    hr = SimConnect_MapClientEventToSimEvent(hInputSimConnect,
                                             DEFINITION_COM_RADIO_FRACT_DEC,
                                             "COM_RADIO_FRACT_DEC");
    hr = SimConnect_MapClientEventToSimEvent(hInputSimConnect,
                                             DEFINITION_COM_RADIO_FRACT_INC,
                                             "COM_RADIO_FRACT_INC");

    hr = SimConnect_MapClientEventToSimEvent(hInputSimConnect,
                                             DEFINITION_COM2_RADIO_FRACT_DEC,
                                             "COM_RADIO2_FRACT_DEC");
    hr = SimConnect_MapClientEventToSimEvent(hInputSimConnect,
                                             DEFINITION_COM2_RADIO_FRACT_INC,
                                             "COM_RADIO2_FRACT_INC");

    hr = SimConnect_MapClientEventToSimEvent(
        hInputSimConnect, DEFINITION_COM_RADIO_FRACT_DEC_CARRY,
        "COM_RADIO_FRACT_DEC_CARRY");
    hr = SimConnect_MapClientEventToSimEvent(
        hInputSimConnect, DEFINITION_COM_RADIO_FRACT_INC_CARRY,
        "COM_RADIO_FRACT_INC_CARRY");

    hr = SimConnect_MapClientEventToSimEvent(
        hInputSimConnect, DEFINITION_COM2_RADIO_FRACT_DEC_CARRY,
        "COM_RADIO2_FRACT_DEC_CARRY");
    hr = SimConnect_MapClientEventToSimEvent(
        hInputSimConnect, DEFINITION_COM2_RADIO_FRACT_INC_CARRY,
        "COM_RADIO2_FRACT_INC_CARRY");

    hr = SimConnect_MapClientEventToSimEvent(hInputSimConnect, DEFINITION_NAV_1,
                                             "NAV1_RADIO_SET");
    hr = SimConnect_MapClientEventToSimEvent(hInputSimConnect, DEFINITION_NAV_2,
                                             "NAV2_RADIO_SET");
    hr = SimConnect_MapClientEventToSimEvent(hInputSimConnect,
                                             DEFINITION_NAV_1_RADIO_FRACT_INC,
                                             "NAV1_RADIO_FRACT_INC");
    hr = SimConnect_MapClientEventToSimEvent(hInputSimConnect,
                                             DEFINITION_NAV_1_RADIO_FRACT_DEC,
                                             "NAV1_RADIO_FRACT_DEC");
    hr = SimConnect_MapClientEventToSimEvent(hInputSimConnect,
                                             DEFINITION_NAV_2_RADIO_FRACT_INC,
                                             "NAV2_RADIO_FRACT_INC");
    hr = SimConnect_MapClientEventToSimEvent(hInputSimConnect,
                                             DEFINITION_NAV_2_RADIO_FRACT_DEC,
                                             "NAV2_RADIO_FRACT_DEC");
    hr = SimConnect_MapClientEventToSimEvent(hInputSimConnect,
                                             DEFINITION_NAV_1_RADIO_WHOLE_INC,
                                             "NAV1_RADIO_WHOLE_INC");
    hr = SimConnect_MapClientEventToSimEvent(hInputSimConnect,
                                             DEFINITION_NAV_1_RADIO_WHOLE_DEC,
                                             "NAV1_RADIO_WHOLE_DEC");
    hr = SimConnect_MapClientEventToSimEvent(hInputSimConnect,
                                             DEFINITION_NAV_2_RADIO_WHOLE_INC,
                                             "NAV2_RADIO_WHOLE_INC");
    hr = SimConnect_MapClientEventToSimEvent(hInputSimConnect,
                                             DEFINITION_NAV_2_RADIO_WHOLE_DEC,
                                             "NAV2_RADIO_WHOLE_DEC");

    hr = SimConnect_MapClientEventToSimEvent(
        hInputSimConnect, DEFINITION_COM_STANDBY_SWAP, "COM1_RADIO_SWAP");
    hr = SimConnect_MapClientEventToSimEvent(
        hInputSimConnect, DEFINITION_COM_2_STANDBY_SWAP, "COM2_RADIO_SWAP");
    hr = SimConnect_MapClientEventToSimEvent(
        hInputSimConnect, DEFINITION_NAV_1_STANDBY_SWAP, "NAV1_RADIO_SWAP");
    hr = SimConnect_MapClientEventToSimEvent(
        hInputSimConnect, DEFINITION_NAV_2_STANDBY_SWAP, "NAV2_RADIO_SWAP");
    // G1000
    hr = SimConnect_MapClientEventToSimEvent(hInputSimConnect,
                                             DEFINITION_G1000_MFD_ZOOMIN_BUTTON,
                                             "TOGGLE_NAV_LIGHTS");
    hr = SimConnect_MapClientEventToSimEvent(
        hInputSimConnect, DEFINITION_G1000_MFD_ZOOMOUT_BUTTON,
        "GPS_ZOOMOUT_BUTTON");

    // DME
    hr = SimConnect_MapClientEventToSimEvent(
        hInputSimConnect, DEFINITION_RADIO_SELECTED_DME1_IDENT_ENABLE,
        "RADIO_SELECTED_DME1_IDENT_ENABLE");
    hr = SimConnect_MapClientEventToSimEvent(
        hInputSimConnect, DEFINITION_RADIO_SELECTED_DME1_IDENT_DISABLE,
        "RADIO_SELECTED_DME1_IDENT_DISABLE");
    hr = SimConnect_MapClientEventToSimEvent(
        hInputSimConnect, DEFINITION_RADIO_SELECTED_DME1_IDENT_SET,
        "RADIO_SELECTED_DME1_IDENT_SET");
    hr = SimConnect_MapClientEventToSimEvent(
        hInputSimConnect, DEFINITION_RADIO_SELECTED_DM1E_IDENT_TOGGLE,
        "RADIO_SELECTED_DM1E_IDENT_TOGGLE");
    hr = SimConnect_MapClientEventToSimEvent(
        hInputSimConnect, DEFINITION_RADIO_SELECTED_DME2_IDENT_ENABLE,
        "RADIO_SELECTED_DME2_IDENT_ENABLE");
    hr = SimConnect_MapClientEventToSimEvent(
        hInputSimConnect, DEFINITION_RADIO_SELECTED_DME2_IDENT_DISABLE,
        "RADIO_SELECTED_DME2_IDENT_DISABLE");
    hr = SimConnect_MapClientEventToSimEvent(
        hInputSimConnect, DEFINITION_RADIO_SELECTED_DME2_IDENT_SET,
        "RADIO_SELECTED_DME2_IDENT_SET");
    hr = SimConnect_MapClientEventToSimEvent(
        hInputSimConnect, DEFINITION_RADIO_SELECTED_DME2_IDENT_TOGGLE,
        "RADIO_SELECTED_DME2_IDENT_TOGGLE");
    hr = SimConnect_MapClientEventToSimEvent(
        hInputSimConnect, DEFINITION_TOGGLE_DME, "TOGGLE_DME");

    // VOR
    hr = SimConnect_MapClientEventToSimEvent(
        hInputSimConnect, DEFINITION_RADIO_SELECTED_VOR1_IDENT_ENABLE,
        "RADIO_SELECTED_VOR1_IDENT_ENABLE");
    hr = SimConnect_MapClientEventToSimEvent(
        hInputSimConnect, DEFINITION_RADIO_SELECTED_VOR1_IDENT_DISABLE,
        "RADIO_SELECTED_VOR1_IDENT_DISABLE");
    hr = SimConnect_MapClientEventToSimEvent(
        hInputSimConnect, DEFINITION_RADIO_SELECTED_VOR1_IDENT_SET,
        "RADIO_SELECTED_VOR1_IDENT_SET");
    hr = SimConnect_MapClientEventToSimEvent(
        hInputSimConnect, DEFINITION_RADIO_SELECTED_VORE_IDENT_TOGGLE,
        "RADIO_SELECTED_VORE_IDENT_TOGGLE");
    hr = SimConnect_MapClientEventToSimEvent(
        hInputSimConnect, DEFINITION_RADIO_SELECTED_VOR2_IDENT_ENABLE,
        "RADIO_SELECTED_VOR2_IDENT_ENABLE");
    hr = SimConnect_MapClientEventToSimEvent(
        hInputSimConnect, DEFINITION_RADIO_SELECTED_VOR2_IDENT_DISABLE,
        "RADIO_SELECTED_VOR2_IDENT_DISABLE");
    hr = SimConnect_MapClientEventToSimEvent(
        hInputSimConnect, DEFINITION_RADIO_SELECTED_VOR2_IDENT_SET,
        "RADIO_SELECTED_VOR2_IDENT_SET");
    hr = SimConnect_MapClientEventToSimEvent(
        hInputSimConnect, DEFINITION_RADIO_SELECTED_VOR2_IDENT_TOGGLE,
        "RADIO_SELECTED_VOR2_IDENT_TOGGLE");
    hr = SimConnect_MapClientEventToSimEvent(
        hInputSimConnect, DEFINITION_TOGGLE_VOR, "TOGGLE_VOR");
    hr = SimConnect_MapClientEventToSimEvent(
        hInputSimConnect, DEFINITION_VOR1_OBI_DEC, "VOR1_OBI_DEC");
    hr = SimConnect_MapClientEventToSimEvent(
        hInputSimConnect, DEFINITION_VOR1_OBI_INC, "VOR1_OBI_INC");
    hr = SimConnect_MapClientEventToSimEvent(
        hInputSimConnect, DEFINITION_VOR2_OBI_DEC, "VOR2_OBI_DEC");
    hr = SimConnect_MapClientEventToSimEvent(
        hInputSimConnect, DEFINITION_VOR2_OBI_INC, "VOR2_OBI_INC");

    // ADF
    hr = SimConnect_MapClientEventToSimEvent(
        hInputSimConnect, DEFINITION_RADIO_SELECTED_ADF_IDENT_ENABLE,
        "RADIO_SELECTED_ADF_IDENT_ENABLE");
    hr = SimConnect_MapClientEventToSimEvent(
        hInputSimConnect, DEFINITION_RADIO_SELECTED_ADF_IDENT_DISABLE,
        "RADIO_SELECTED_ADF_IDENT_DISABLE");
    hr = SimConnect_MapClientEventToSimEvent(
        hInputSimConnect, DEFINITION_RADIO_SELECTED_ADF_IDENT_SET,
        "RADIO_SELECTED_ADF_IDENT_SET");
    hr = SimConnect_MapClientEventToSimEvent(
        hInputSimConnect, DEFINITION_RADIO_SELECTED_ADF_IDENT_TOGGLE,
        "RADIO_SELECTED_ADF_IDENT_TOGGLE");
    hr = SimConnect_MapClientEventToSimEvent(
        hInputSimConnect, DEFINITION_ADF_CARD_INC, "ADF_CARD_INC");
    hr = SimConnect_MapClientEventToSimEvent(
        hInputSimConnect, DEFINITION_ADF_CARD_DEC, "ADF_CARD_DEC");
    hr = SimConnect_MapClientEventToSimEvent(
        hInputSimConnect, DEFINITION_ADF_CARD_SET, "ADF_CARD_SET");
    hr = SimConnect_MapClientEventToSimEvent(
        hInputSimConnect, DEFINITION_ADF_100_INC, "ADF_100_INC");
    hr = SimConnect_MapClientEventToSimEvent(
        hInputSimConnect, DEFINITION_ADF_10_INC, "ADF_10_INC");
    hr = SimConnect_MapClientEventToSimEvent(hInputSimConnect,
                                             DEFINITION_ADF_1_INC, "ADF_1_INC");
    hr = SimConnect_MapClientEventToSimEvent(
        hInputSimConnect, DEFINITION_ADF_100_DEC, "ADF_100_DEC");
    hr = SimConnect_MapClientEventToSimEvent(
        hInputSimConnect, DEFINITION_ADF_10_DEC, "ADF_10_DEC");
    hr = SimConnect_MapClientEventToSimEvent(hInputSimConnect,
                                             DEFINITION_ADF_1_DEC, "ADF_1_DEC");
    hr = SimConnect_MapClientEventToSimEvent(
        hInputSimConnect, DEFINITION_ADF_COMPLETE_SET, "ADF_COMPLETE_SET ");
    hr = SimConnect_MapClientEventToSimEvent(
        hInputSimConnect, DEFINITION_ADF1_WHOLE_INC, "ADF1_WHOLE_INC ");
    hr = SimConnect_MapClientEventToSimEvent(
        hInputSimConnect, DEFINITION_ADF1_WHOLE_DEC, "ADF1_WHOLE_DEC ");
    hr = SimConnect_MapClientEventToSimEvent(
        hInputSimConnect, DEFINITION_ADF2_100_INC, "ADF2_100_INC ");
    hr = SimConnect_MapClientEventToSimEvent(
        hInputSimConnect, DEFINITION_ADF2_10_INC, "ADF2_10_INC ");
    hr = SimConnect_MapClientEventToSimEvent(
        hInputSimConnect, DEFINITION_ADF2_1_INC, "ADF2_1_INC ");
    hr = SimConnect_MapClientEventToSimEvent(hInputSimConnect,
                                             DEFINITION_ADF2_RADIO_TENTHS_INC,
                                             "ADF2_RADIO_TENTHS_INC ");
    hr = SimConnect_MapClientEventToSimEvent(
        hInputSimConnect, DEFINITION_ADF2_100_DEC, "ADF2_100_DEC ");
    hr = SimConnect_MapClientEventToSimEvent(
        hInputSimConnect, DEFINITION_ADF2_10_DEC, "ADF2_10_DEC ");
    hr = SimConnect_MapClientEventToSimEvent(
        hInputSimConnect, DEFINITION_ADF2_1_DEC, "ADF2_1_DEC ");
    hr = SimConnect_MapClientEventToSimEvent(hInputSimConnect,
                                             DEFINITION_ADF2_RADIO_TENTHS_DEC,
                                             "ADF2_RADIO_TENTHS_DEC ");
    hr = SimConnect_MapClientEventToSimEvent(
        hInputSimConnect, DEFINITION_ADF2_WHOLE_INC, "ADF2_WHOLE_INC ");
    hr = SimConnect_MapClientEventToSimEvent(
        hInputSimConnect, DEFINITION_ADF2_WHOLE_DEC, "ADF2_WHOLE_DEC ");
    hr = SimConnect_MapClientEventToSimEvent(hInputSimConnect,
                                             DEFINITION_ADF2_FRACT_DEC_CARRY,
                                             "ADF2_FRACT_DEC_CARRY ");
    hr = SimConnect_MapClientEventToSimEvent(hInputSimConnect,
                                             DEFINITION_ADF2_FRACT_INC_CARRY,
                                             "ADF2_FRACT_INC_CARRY ");
    hr = SimConnect_MapClientEventToSimEvent(
        hInputSimConnect, DEFINITION_ADF2_COMPLETE_SET, "ADF2_COMPLETE_SET ");
    hr = SimConnect_MapClientEventToSimEvent(
        hInputSimConnect, DEFINITION_RADIO_ADF2_IDENT_DISABLE,
        "RADIO_ADF2_IDENT_DISABLE ");
    hr = SimConnect_MapClientEventToSimEvent(hInputSimConnect,
                                             DEFINITION_RADIO_ADF2_IDENT_ENABLE,
                                             "RADIO_ADF2_IDENT_ENABLE ");
    hr = SimConnect_MapClientEventToSimEvent(hInputSimConnect,
                                             DEFINITION_RADIO_ADF2_IDENT_TOGGLE,
                                             "RADIO_ADF2_IDENT_TOGGLE ");
    hr = SimConnect_MapClientEventToSimEvent(hInputSimConnect,
                                             DEFINITION_RADIO_ADF2_IDENT_SET,
                                             "RADIO_ADF2_IDENT_SET ");

    // XPNDR
    hr = SimConnect_MapClientEventToSimEvent(
        hInputSimConnect, DEFINITION_XPNDR_1000_INC, "XPNDR_1000_INC");
    hr = SimConnect_MapClientEventToSimEvent(
        hInputSimConnect, DEFINITION_XPNDR_100_INC, "XPNDR_100_INC");
    hr = SimConnect_MapClientEventToSimEvent(
        hInputSimConnect, DEFINITION_XPNDR_10_INC, "XPNDR_10_INC");
    hr = SimConnect_MapClientEventToSimEvent(
        hInputSimConnect, DEFINITION_XPNDR_1_INC, "XPNDR_1_INC");
    hr = SimConnect_MapClientEventToSimEvent(
        hInputSimConnect, DEFINITION_XPNDR_1000_DEC, "XPNDR_1000_DEC");
    hr = SimConnect_MapClientEventToSimEvent(
        hInputSimConnect, DEFINITION_XPNDR_100_DEC, "XPNDR_100_DEC");
    hr = SimConnect_MapClientEventToSimEvent(
        hInputSimConnect, DEFINITION_XPNDR_10_DEC, "XPNDR_10_DEC");
    hr = SimConnect_MapClientEventToSimEvent(
        hInputSimConnect, DEFINITION_XPNDR_1_DEC, "XPNDR_1_DEC");
    hr = SimConnect_MapClientEventToSimEvent(hInputSimConnect,
                                             DEFINITION_XPNDR_SET, "XPNDR_SET");

    // Flaps
    hr = SimConnect_MapClientEventToSimEvent(hInputSimConnect,
                                             DEFINITION_FLAPS_UP, "FLAPS_UP");
    hr = SimConnect_MapClientEventToSimEvent(hInputSimConnect,
                                             DEFINITION_FLAPS_1, "FLAPS_1");
    hr = SimConnect_MapClientEventToSimEvent(hInputSimConnect,
                                             DEFINITION_FLAPS_2, "FLAPS_2");
    hr = SimConnect_MapClientEventToSimEvent(hInputSimConnect,
                                             DEFINITION_FLAPS_3, "FLAPS_3");
    hr = SimConnect_MapClientEventToSimEvent(
        hInputSimConnect, DEFINITION_FLAPS_DOWN, "FLAPS_DOWN");
    hr = SimConnect_MapClientEventToSimEvent(
        hInputSimConnect, DEFINITION_FLAPS_INCR, "FLAPS_INCR");
    hr = SimConnect_MapClientEventToSimEvent(
        hInputSimConnect, DEFINITION_FLAPS_DECR, "FLAPS_DECR");

    arduinoInput = new SerialPort(valPort);
    std::cout << "Is connected: " << arduinoInput->isConnected() << std::endl;
    while (arduinoInput->isConnected() && abortInput != true) {
      const auto hasRead =
          arduinoInput->readSerialPort(receivedString, DATA_LENGTH);

      if (hasRead) {
        Sleep(10);
        //
        if (sizeof(receivedString) > 2) {
          prefix = std::string(&receivedString[0], &receivedString[3]);
          try {
            int prefixVal = stoi(prefix);
            cout << prefix << endl;
            switch (prefixVal) {
                // Throttle + mixture

              case 198: {
                set_prop_values();
                hr = SimConnect_SetDataOnSimObject(
                    hInputSimConnect, DEFINITION_PROPELLER_LEVER_POSITION_1,
                    SIMCONNECT_OBJECT_ID_USER, 0, 2, sizeof(pc.prop_percent[0]),
                    &pc.prop_percent[0]);

                hr = SimConnect_SetDataOnSimObject(
                    hInputSimConnect, DEFINITION_PROPELLER_LEVER_POSITION_2,
                    SIMCONNECT_OBJECT_ID_USER, 0, 2, sizeof(pc.prop_percent[1]),
                    &pc.prop_percent[1]);
                break;
              }

              case 199: {
                set_throttle_values();
                cout << tc.throttle_percent << ": Throtle percent" << endl;
                if (advanced) {
                  hr = SimConnect_SetDataOnSimObject(
                      hInputSimConnect, DEFINITION_MIXTURE_1,
                      SIMCONNECT_OBJECT_ID_USER, 0, 0,
                      sizeof(mc.mixture_percent), &mc.mixture_percent);
                }

                hr = SimConnect_SetDataOnSimObject(
                    hInputSimConnect, DEFINITION_THROTTLE_1,
                    SIMCONNECT_OBJECT_ID_USER, 0, 4,
                    sizeof(tc.throttle_percent[0]), &tc.throttle_percent[0]);

                hr = SimConnect_SetDataOnSimObject(
                    hInputSimConnect, DEFINITION_THROTTLE_2,
                    SIMCONNECT_OBJECT_ID_USER, 0, 4,
                    sizeof(tc.throttle_percent[1]), &tc.throttle_percent[1]);

                hr = SimConnect_SetDataOnSimObject(
                    hInputSimConnect, DEFINITION_THROTTLE_3,
                    SIMCONNECT_OBJECT_ID_USER, 0, 4,
                    sizeof(tc.throttle_percent[2]), &tc.throttle_percent[2]);

                hr = SimConnect_SetDataOnSimObject(
                    hInputSimConnect, DEFINITION_THROTTLE_4,
                    SIMCONNECT_OBJECT_ID_USER, 0, 4,
                    sizeof(tc.throttle_percent[3]), &tc.throttle_percent[3]);

                break;
              }

              case 100: {
                int value =
                    stoi(std::string(&receivedString[4], &receivedString[10]));
                SimConnect_TransmitClientEvent(
                    hInputSimConnect, SIMCONNECT_OBJECT_ID_USER,
                    DEFINITION_COM_1_SET, Dec2Bcd(value / 10),
                    SIMCONNECT_GROUP_PRIORITY_HIGHEST,
                    SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
                if (value % (value / 10) == 5 || value % (value / 10) == 85) {
                  SimConnect_TransmitClientEvent(
                      hInputSimConnect, SIMCONNECT_OBJECT_ID_USER,
                      DEFINITION_COM_RADIO_FRACT_INC, 0,
                      SIMCONNECT_GROUP_PRIORITY_HIGHEST,
                      SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
                }
                break;
              }
              case 102: {
                int value =
                    stoi(std::string(&receivedString[4], &receivedString[10]));
                SimConnect_TransmitClientEvent(
                    hInputSimConnect, SIMCONNECT_OBJECT_ID_USER,
                    DEFINITION_COM_2_SET, Dec2Bcd(value / 10),
                    SIMCONNECT_GROUP_PRIORITY_HIGHEST,
                    SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
                if (value % (value / 10) == 5 || value % (value / 10) == 85) {
                  SimConnect_TransmitClientEvent(
                      hInputSimConnect, SIMCONNECT_OBJECT_ID_USER,
                      DEFINITION_COM2_RADIO_FRACT_INC, 0,
                      SIMCONNECT_GROUP_PRIORITY_HIGHEST,
                      SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
                }
                break;
              }

                // Swap com1
              case 116: {
                SimConnect_TransmitClientEvent(
                    hInputSimConnect, 0, DEFINITION_COM_STANDBY_SWAP, 0,
                    SIMCONNECT_GROUP_PRIORITY_HIGHEST,
                    SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
                break;
              }
                // Swap com2
              case 117: {
                SimConnect_TransmitClientEvent(
                    hInputSimConnect, 0, DEFINITION_COM_2_STANDBY_SWAP, 0,
                    SIMCONNECT_GROUP_PRIORITY_HIGHEST,
                    SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
                break;
              }
                // Swap nav1
              case 118: {
                SimConnect_TransmitClientEvent(
                    hInputSimConnect, 0, DEFINITION_NAV_1_STANDBY_SWAP, 0,
                    SIMCONNECT_GROUP_PRIORITY_HIGHEST,
                    SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
                break;
              }
                // Swap nav1
              case 119: {
                SimConnect_TransmitClientEvent(
                    hInputSimConnect, 0, DEFINITION_NAV_2_STANDBY_SWAP, 0,
                    SIMCONNECT_GROUP_PRIORITY_HIGHEST,
                    SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
                break;
              }

              case 120: {
                SimConnect_TransmitClientEvent(
                    hInputSimConnect, 0, DEFINITION_COM2_RADIO_FRACT_DEC, 0,
                    SIMCONNECT_GROUP_PRIORITY_HIGHEST,
                    SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
                break;
              }
              case 121: {
                SimConnect_TransmitClientEvent(
                    hInputSimConnect, 0, DEFINITION_COM2_RADIO_FRACT_INC, 0,
                    SIMCONNECT_GROUP_PRIORITY_HIGHEST,
                    SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
                break;
              }
              case 122: {
                SimConnect_TransmitClientEvent(
                    hInputSimConnect, 0, DEFINITION_COM2_RADIO_FRACT_DEC_CARRY,
                    0, SIMCONNECT_GROUP_PRIORITY_HIGHEST,
                    SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
                break;
              }
              case 123: {
                SimConnect_TransmitClientEvent(
                    hInputSimConnect, 0, DEFINITION_COM2_RADIO_FRACT_INC_CARRY,
                    0, SIMCONNECT_GROUP_PRIORITY_HIGHEST,
                    SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
                break;
              }
              case 124: {
                SimConnect_TransmitClientEvent(
                    hInputSimConnect, 0, DEFINITION_COM_RADIO_FRACT_DEC, 0,
                    SIMCONNECT_GROUP_PRIORITY_HIGHEST,
                    SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
                break;
              }
              case 125: {
                SimConnect_TransmitClientEvent(
                    hInputSimConnect, 0, DEFINITION_COM_RADIO_FRACT_INC, 0,
                    SIMCONNECT_GROUP_PRIORITY_HIGHEST,
                    SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
                break;
              }
              case 126: {
                SimConnect_TransmitClientEvent(
                    hInputSimConnect, 0, DEFINITION_COM_RADIO_FRACT_DEC_CARRY,
                    0, SIMCONNECT_GROUP_PRIORITY_HIGHEST,
                    SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
                break;
              }
              case 127: {
                SimConnect_TransmitClientEvent(
                    hInputSimConnect, 0, DEFINITION_COM_RADIO_FRACT_INC_CARRY,
                    0, SIMCONNECT_GROUP_PRIORITY_HIGHEST,
                    SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
                break;
              }

              // NAV
              case 128: {
                SimConnect_TransmitClientEvent(
                    hInputSimConnect, 0, DEFINITION_NAV_1_RADIO_FRACT_INC, 0,
                    SIMCONNECT_GROUP_PRIORITY_HIGHEST,
                    SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
                break;
              }
              case 129: {
                SimConnect_TransmitClientEvent(
                    hInputSimConnect, 0, DEFINITION_NAV_1_RADIO_FRACT_DEC, 0,
                    SIMCONNECT_GROUP_PRIORITY_HIGHEST,
                    SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
                break;
              }
              case 130: {
                SimConnect_TransmitClientEvent(
                    hInputSimConnect, 0, DEFINITION_NAV_2_RADIO_FRACT_INC, 0,
                    SIMCONNECT_GROUP_PRIORITY_HIGHEST,
                    SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
                break;
              }
              case 131: {
                SimConnect_TransmitClientEvent(
                    hInputSimConnect, 0, DEFINITION_NAV_2_RADIO_FRACT_DEC, 0,
                    SIMCONNECT_GROUP_PRIORITY_HIGHEST,
                    SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
                break;
              }
              case 132: {
                SimConnect_TransmitClientEvent(
                    hInputSimConnect, 0, DEFINITION_NAV_1_RADIO_WHOLE_INC, 0,
                    SIMCONNECT_GROUP_PRIORITY_HIGHEST,
                    SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
                break;
              }
              case 133: {
                SimConnect_TransmitClientEvent(
                    hInputSimConnect, 0, DEFINITION_NAV_1_RADIO_WHOLE_DEC, 0,
                    SIMCONNECT_GROUP_PRIORITY_HIGHEST,
                    SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
                break;
              }
              case 134: {
                SimConnect_TransmitClientEvent(
                    hInputSimConnect, 0, DEFINITION_NAV_2_RADIO_WHOLE_INC, 0,
                    SIMCONNECT_GROUP_PRIORITY_HIGHEST,
                    SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
                break;
              }
              case 135: {
                SimConnect_TransmitClientEvent(
                    hInputSimConnect, 0, DEFINITION_NAV_2_RADIO_WHOLE_DEC, 0,
                    SIMCONNECT_GROUP_PRIORITY_HIGHEST,
                    SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
                break;
              }
              // DME
              case 136: {
                SimConnect_TransmitClientEvent(
                    hInputSimConnect, 0,
                    DEFINITION_RADIO_SELECTED_DME1_IDENT_ENABLE, 0,
                    SIMCONNECT_GROUP_PRIORITY_HIGHEST,
                    SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
                break;
              }
              case 137: {
                SimConnect_TransmitClientEvent(
                    hInputSimConnect, 0,
                    DEFINITION_RADIO_SELECTED_DME1_IDENT_DISABLE, 0,
                    SIMCONNECT_GROUP_PRIORITY_HIGHEST,
                    SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
                break;
              }
              case 138: {
                SimConnect_TransmitClientEvent(
                    hInputSimConnect, 0,
                    DEFINITION_RADIO_SELECTED_DME1_IDENT_SET, 0,
                    SIMCONNECT_GROUP_PRIORITY_HIGHEST,
                    SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
                break;
              }
              case 139: {
                SimConnect_TransmitClientEvent(
                    hInputSimConnect, 0,
                    DEFINITION_RADIO_SELECTED_DM1E_IDENT_TOGGLE, 0,
                    SIMCONNECT_GROUP_PRIORITY_HIGHEST,
                    SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
                break;
              }
              case 140: {
                SimConnect_TransmitClientEvent(
                    hInputSimConnect, 0,
                    DEFINITION_RADIO_SELECTED_DME2_IDENT_ENABLE, 0,
                    SIMCONNECT_GROUP_PRIORITY_HIGHEST,
                    SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
                break;
              }
              case 141: {
                SimConnect_TransmitClientEvent(
                    hInputSimConnect, 0,
                    DEFINITION_RADIO_SELECTED_DME2_IDENT_DISABLE, 0,
                    SIMCONNECT_GROUP_PRIORITY_HIGHEST,
                    SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
                break;
              }
              case 142: {
                SimConnect_TransmitClientEvent(
                    hInputSimConnect, 0,
                    DEFINITION_RADIO_SELECTED_DME2_IDENT_SET, 0,
                    SIMCONNECT_GROUP_PRIORITY_HIGHEST,
                    SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
                break;
              }
              case 143: {
                SimConnect_TransmitClientEvent(
                    hInputSimConnect, 0,
                    DEFINITION_RADIO_SELECTED_DME2_IDENT_TOGGLE, 0,
                    SIMCONNECT_GROUP_PRIORITY_HIGHEST,
                    SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
                break;
              }
              case 144: {
                SimConnect_TransmitClientEvent(
                    hInputSimConnect, 0, DEFINITION_TOGGLE_DME, 0,
                    SIMCONNECT_GROUP_PRIORITY_HIGHEST,
                    SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
                break;
              }

              // VOR1
              case 145: {
                SimConnect_TransmitClientEvent(
                    hInputSimConnect, 0,
                    DEFINITION_RADIO_SELECTED_VOR1_IDENT_ENABLE, 0,
                    SIMCONNECT_GROUP_PRIORITY_HIGHEST,
                    SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
                break;
              }
              case 146: {
                SimConnect_TransmitClientEvent(
                    hInputSimConnect, 0,
                    DEFINITION_RADIO_SELECTED_VOR1_IDENT_DISABLE, 0,
                    SIMCONNECT_GROUP_PRIORITY_HIGHEST,
                    SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
                break;
              }
              //!
              case 147: {
                SimConnect_TransmitClientEvent(
                    hInputSimConnect, 0,
                    DEFINITION_RADIO_SELECTED_VOR1_IDENT_SET, 0,
                    SIMCONNECT_GROUP_PRIORITY_HIGHEST,
                    SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
                break;
              }
              case 148: {
                SimConnect_TransmitClientEvent(
                    hInputSimConnect, 0,
                    DEFINITION_RADIO_SELECTED_VORE_IDENT_TOGGLE, 0,
                    SIMCONNECT_GROUP_PRIORITY_HIGHEST,
                    SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
                break;
              }
              case 149: {
                SimConnect_TransmitClientEvent(
                    hInputSimConnect, 0,
                    DEFINITION_RADIO_SELECTED_VOR2_IDENT_ENABLE, 0,
                    SIMCONNECT_GROUP_PRIORITY_HIGHEST,
                    SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
                break;
              }
              case 150: {
                SimConnect_TransmitClientEvent(
                    hInputSimConnect, 0,
                    DEFINITION_RADIO_SELECTED_VOR2_IDENT_DISABLE, 0,
                    SIMCONNECT_GROUP_PRIORITY_HIGHEST,
                    SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
                break;
              }
              case 151: {
                SimConnect_TransmitClientEvent(
                    hInputSimConnect, 0,
                    DEFINITION_RADIO_SELECTED_VOR2_IDENT_SET, 0,
                    SIMCONNECT_GROUP_PRIORITY_HIGHEST,
                    SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
                break;
              }
              case 152: {
                SimConnect_TransmitClientEvent(
                    hInputSimConnect, 0,
                    DEFINITION_RADIO_SELECTED_VOR2_IDENT_TOGGLE, 0,
                    SIMCONNECT_GROUP_PRIORITY_HIGHEST,
                    SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
                break;
              }
              case 153: {
                SimConnect_TransmitClientEvent(
                    hInputSimConnect, 0, DEFINITION_TOGGLE_VOR, 0,
                    SIMCONNECT_GROUP_PRIORITY_HIGHEST,
                    SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
                break;
              }
              case 154: {
                SimConnect_TransmitClientEvent(
                    hInputSimConnect, 0, DEFINITION_VOR1_OBI_DEC, 0,
                    SIMCONNECT_GROUP_PRIORITY_HIGHEST,
                    SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
                break;
              }
              case 155: {
                SimConnect_TransmitClientEvent(
                    hInputSimConnect, 0, DEFINITION_VOR1_OBI_INC, 0,
                    SIMCONNECT_GROUP_PRIORITY_HIGHEST,
                    SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
                break;
              }
              case 156: {
                SimConnect_TransmitClientEvent(
                    hInputSimConnect, 0, DEFINITION_VOR2_OBI_DEC, 0,
                    SIMCONNECT_GROUP_PRIORITY_HIGHEST,
                    SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
                break;
              }
              case 157: {
                SimConnect_TransmitClientEvent(
                    hInputSimConnect, 0, DEFINITION_VOR2_OBI_INC, 0,
                    SIMCONNECT_GROUP_PRIORITY_HIGHEST,
                    SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
                break;
              }

              // ADF
              case 158: {
                SimConnect_TransmitClientEvent(
                    hInputSimConnect, 0,
                    DEFINITION_RADIO_SELECTED_ADF_IDENT_ENABLE, 0,
                    SIMCONNECT_GROUP_PRIORITY_HIGHEST,
                    SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
                break;
              }
              case 159: {
                SimConnect_TransmitClientEvent(
                    hInputSimConnect, 0,
                    DEFINITION_RADIO_SELECTED_ADF_IDENT_DISABLE, 0,
                    SIMCONNECT_GROUP_PRIORITY_HIGHEST,
                    SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
                break;
              }
              case 160: {
                SimConnect_TransmitClientEvent(
                    hInputSimConnect, 0,
                    DEFINITION_RADIO_SELECTED_ADF_IDENT_SET, 0,
                    SIMCONNECT_GROUP_PRIORITY_HIGHEST,
                    SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
                break;
              }
              case 161: {
                SimConnect_TransmitClientEvent(
                    hInputSimConnect, 0,
                    DEFINITION_RADIO_SELECTED_ADF_IDENT_TOGGLE, 0,
                    SIMCONNECT_GROUP_PRIORITY_HIGHEST,
                    SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
                break;
              }
              case 162: {
                SimConnect_TransmitClientEvent(
                    hInputSimConnect, 0, DEFINITION_ADF_CARD_INC, 0,
                    SIMCONNECT_GROUP_PRIORITY_HIGHEST,
                    SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
                break;
              }
              case 163: {
                SimConnect_TransmitClientEvent(
                    hInputSimConnect, 0, DEFINITION_ADF_CARD_DEC, 0,
                    SIMCONNECT_GROUP_PRIORITY_HIGHEST,
                    SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
                break;
              }
              case 164: {
                SimConnect_TransmitClientEvent(
                    hInputSimConnect, 0, DEFINITION_ADF_CARD_SET, 0,
                    SIMCONNECT_GROUP_PRIORITY_HIGHEST,
                    SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
                break;
              }
              case 165: {
                SimConnect_TransmitClientEvent(
                    hInputSimConnect, 0, DEFINITION_ADF_100_INC, 0,
                    SIMCONNECT_GROUP_PRIORITY_HIGHEST,
                    SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
                break;
              }
              case 166: {
                SimConnect_TransmitClientEvent(
                    hInputSimConnect, 0, DEFINITION_ADF_10_INC, 0,
                    SIMCONNECT_GROUP_PRIORITY_HIGHEST,
                    SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
                break;
              }
              case 167: {
                SimConnect_TransmitClientEvent(
                    hInputSimConnect, 0, DEFINITION_ADF_1_INC, 0,
                    SIMCONNECT_GROUP_PRIORITY_HIGHEST,
                    SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
                break;
              }
              case 168: {
                SimConnect_TransmitClientEvent(
                    hInputSimConnect, 0, DEFINITION_ADF_100_DEC, 0,
                    SIMCONNECT_GROUP_PRIORITY_HIGHEST,
                    SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
                break;
              }
              case 169: {
                SimConnect_TransmitClientEvent(
                    hInputSimConnect, 0, DEFINITION_ADF_10_DEC, 0,
                    SIMCONNECT_GROUP_PRIORITY_HIGHEST,
                    SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
                break;
              }
              case 170: {
                SimConnect_TransmitClientEvent(
                    hInputSimConnect, 0, DEFINITION_ADF_1_DEC, 0,
                    SIMCONNECT_GROUP_PRIORITY_HIGHEST,
                    SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
                break;
              }

              // XPNDR
              case 171: {
                SimConnect_TransmitClientEvent(
                    hInputSimConnect, 0, DEFINITION_XPNDR_1000_INC, 0,
                    SIMCONNECT_GROUP_PRIORITY_HIGHEST,
                    SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
                break;
              }
              case 172: {
                SimConnect_TransmitClientEvent(
                    hInputSimConnect, 0, DEFINITION_XPNDR_100_INC, 0,
                    SIMCONNECT_GROUP_PRIORITY_HIGHEST,
                    SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
                break;
              }
              case 173: {
                SimConnect_TransmitClientEvent(
                    hInputSimConnect, 0, DEFINITION_XPNDR_10_INC, 0,
                    SIMCONNECT_GROUP_PRIORITY_HIGHEST,
                    SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
                break;
              }
              case 174: {
                SimConnect_TransmitClientEvent(
                    hInputSimConnect, 0, DEFINITION_XPNDR_1_INC, 0,
                    SIMCONNECT_GROUP_PRIORITY_HIGHEST,
                    SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
                break;
              }
              case 175: {
                SimConnect_TransmitClientEvent(
                    hInputSimConnect, 0, DEFINITION_XPNDR_1000_DEC, 0,
                    SIMCONNECT_GROUP_PRIORITY_HIGHEST,
                    SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
                break;
              }
              case 176: {
                SimConnect_TransmitClientEvent(
                    hInputSimConnect, 0, DEFINITION_XPNDR_100_DEC, 0,
                    SIMCONNECT_GROUP_PRIORITY_HIGHEST,
                    SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
                break;
              }
              case 177: {
                SimConnect_TransmitClientEvent(
                    hInputSimConnect, 0, DEFINITION_XPNDR_10_DEC, 0,
                    SIMCONNECT_GROUP_PRIORITY_HIGHEST,
                    SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
                break;
              }
              case 178: {
                SimConnect_TransmitClientEvent(
                    hInputSimConnect, 0, DEFINITION_XPNDR_1_DEC, 0,
                    SIMCONNECT_GROUP_PRIORITY_HIGHEST,
                    SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
                break;
              }
              case 202: {
                std::string value =
                    std::string(&receivedString[4], &receivedString[8]);
                SimConnect_TransmitClientEvent(
                    hInputSimConnect, 0, DEFINITION_XPNDR_SET,
                    Dec2Bcd(stoi(value)), SIMCONNECT_GROUP_PRIORITY_HIGHEST,
                    SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
                break;
              }
              case 179: {
                SimConnect_TransmitClientEvent(
                    hInputSimConnect, 0, DEFINITION_ADF_COMPLETE_SET, 0,
                    SIMCONNECT_GROUP_PRIORITY_HIGHEST,
                    SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
                break;
              }
              case 180: {
                SimConnect_TransmitClientEvent(
                    hInputSimConnect, 0, DEFINITION_ADF1_WHOLE_INC, 0,
                    SIMCONNECT_GROUP_PRIORITY_HIGHEST,
                    SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
                break;
              }
              case 181: {
                SimConnect_TransmitClientEvent(
                    hInputSimConnect, 0, DEFINITION_ADF1_WHOLE_DEC, 0,
                    SIMCONNECT_GROUP_PRIORITY_HIGHEST,
                    SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
                break;
              }
              case 182: {
                SimConnect_TransmitClientEvent(
                    hInputSimConnect, 0, DEFINITION_ADF2_100_INC, 0,
                    SIMCONNECT_GROUP_PRIORITY_HIGHEST,
                    SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
                break;
              }
              case 183: {
                SimConnect_TransmitClientEvent(
                    hInputSimConnect, 0, DEFINITION_ADF2_10_INC, 0,
                    SIMCONNECT_GROUP_PRIORITY_HIGHEST,
                    SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
                break;
              }
              case 184: {
                SimConnect_TransmitClientEvent(
                    hInputSimConnect, 0, DEFINITION_ADF2_1_INC, 0,
                    SIMCONNECT_GROUP_PRIORITY_HIGHEST,
                    SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
                break;
              }
              case 185: {
                SimConnect_TransmitClientEvent(
                    hInputSimConnect, 0, DEFINITION_ADF2_RADIO_TENTHS_INC, 0,
                    SIMCONNECT_GROUP_PRIORITY_HIGHEST,
                    SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
                break;
              }
              case 186: {
                SimConnect_TransmitClientEvent(
                    hInputSimConnect, 0, DEFINITION_ADF2_100_DEC, 0,
                    SIMCONNECT_GROUP_PRIORITY_HIGHEST,
                    SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
                break;
              }
              case 187: {
                SimConnect_TransmitClientEvent(
                    hInputSimConnect, 0, DEFINITION_ADF2_10_DEC, 0,
                    SIMCONNECT_GROUP_PRIORITY_HIGHEST,
                    SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
                break;
              }
              case 188: {
                SimConnect_TransmitClientEvent(
                    hInputSimConnect, 0, DEFINITION_ADF2_1_DEC, 0,
                    SIMCONNECT_GROUP_PRIORITY_HIGHEST,
                    SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
                break;
              }
              case 189: {
                SimConnect_TransmitClientEvent(
                    hInputSimConnect, 0, DEFINITION_ADF2_RADIO_TENTHS_DEC, 0,
                    SIMCONNECT_GROUP_PRIORITY_HIGHEST,
                    SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
                break;
              }
              case 190: {
                SimConnect_TransmitClientEvent(
                    hInputSimConnect, 0, DEFINITION_ADF2_WHOLE_INC, 0,
                    SIMCONNECT_GROUP_PRIORITY_HIGHEST,
                    SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
                break;
              }
              case 191: {
                SimConnect_TransmitClientEvent(
                    hInputSimConnect, 0, DEFINITION_ADF2_WHOLE_DEC, 0,
                    SIMCONNECT_GROUP_PRIORITY_HIGHEST,
                    SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
                break;
              }
              case 192: {
                SimConnect_TransmitClientEvent(
                    hInputSimConnect, 0, DEFINITION_ADF2_FRACT_DEC_CARRY, 0,
                    SIMCONNECT_GROUP_PRIORITY_HIGHEST,
                    SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
                break;
              }
              case 193: {
                SimConnect_TransmitClientEvent(
                    hInputSimConnect, 0, DEFINITION_ADF2_FRACT_INC_CARRY, 0,
                    SIMCONNECT_GROUP_PRIORITY_HIGHEST,
                    SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
                break;
              }
              case 194: {
                SimConnect_TransmitClientEvent(
                    hInputSimConnect, 0, DEFINITION_ADF2_COMPLETE_SET, 0,
                    SIMCONNECT_GROUP_PRIORITY_HIGHEST,
                    SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
                break;
              }
              case 195: {
                SimConnect_TransmitClientEvent(
                    hInputSimConnect, 0, DEFINITION_RADIO_ADF2_IDENT_DISABLE, 0,
                    SIMCONNECT_GROUP_PRIORITY_HIGHEST,
                    SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
                break;
              }
              case 196: {
                SimConnect_TransmitClientEvent(
                    hInputSimConnect, 0, DEFINITION_RADIO_ADF2_IDENT_ENABLE, 0,
                    SIMCONNECT_GROUP_PRIORITY_HIGHEST,
                    SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
                break;
              }
              case 197: {
                SimConnect_TransmitClientEvent(
                    hInputSimConnect, 0, DEFINITION_RADIO_ADF2_IDENT_TOGGLE, 0,
                    SIMCONNECT_GROUP_PRIORITY_HIGHEST,
                    SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
                break;
              }
              case 200: {
                SimConnect_TransmitClientEvent(
                    hInputSimConnect, 0, DEFINITION_RADIO_ADF2_IDENT_SET, 0,
                    SIMCONNECT_GROUP_PRIORITY_HIGHEST,
                    SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
                break;
              }
              // G1000---------
              // zoomout
              case 250: {
                SimConnect_TransmitClientEvent(
                    hInputSimConnect, 0, DEFINITION_G1000_MFD_ZOOMOUT_BUTTON, 1,
                    SIMCONNECT_GROUP_PRIORITY_HIGHEST,
                    SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
                break;
              }
                // zoomin
              case 251: {
                SimConnect_TransmitClientEvent(
                    hInputSimConnect, 0, DEFINITION_G1000_MFD_ZOOMIN_BUTTON, 1,
                    SIMCONNECT_GROUP_PRIORITY_HIGHEST,
                    SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
                break;
              }

              // AP
              case 301: {
                SimConnect_TransmitClientEvent(
                    hInputSimConnect, 0, DEFINITION_AP_MASTER, 0,
                    SIMCONNECT_GROUP_PRIORITY_HIGHEST,
                    SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
                break;
              }
              case 302: {
                SimConnect_TransmitClientEvent(
                    hInputSimConnect, 0, DEFINITION_AP_PANEL_HEADING_HOLD, 0,
                    SIMCONNECT_GROUP_PRIORITY_HIGHEST,
                    SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
                break;
              }
              case 303: {
                SimConnect_TransmitClientEvent(
                    hInputSimConnect, 0, DEFINITION_AP_PANEL_ALTITUDE_HOLD, 0,
                    SIMCONNECT_GROUP_PRIORITY_HIGHEST,
                    SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
                break;
              }
              case 304: {
                SimConnect_TransmitClientEvent(
                    hInputSimConnect, 0, DEFINITION_AP_ATT_HOLD_ON, 0,
                    SIMCONNECT_GROUP_PRIORITY_HIGHEST,
                    SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
                break;
              }
              case 305: {
                SimConnect_TransmitClientEvent(
                    hInputSimConnect, 0, DEFINITION_AP_LOC_HOLD_ON, 0,
                    SIMCONNECT_GROUP_PRIORITY_HIGHEST,
                    SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
                break;
              }
              case 306: {
                SimConnect_TransmitClientEvent(
                    hInputSimConnect, 0, DEFINITION_AP_APR_HOLD_ON, 0,
                    SIMCONNECT_GROUP_PRIORITY_HIGHEST,
                    SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
                break;
              }
              case 307: {
                SimConnect_TransmitClientEvent(
                    hInputSimConnect, 0, DEFINITION_AP_HDG_HOLD_ON, 0,
                    SIMCONNECT_GROUP_PRIORITY_HIGHEST,
                    SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
                break;
              }
              case 308: {
                SimConnect_TransmitClientEvent(
                    hInputSimConnect, 0, DEFINITION_AP_ALT_HOLD_ON, 0,
                    SIMCONNECT_GROUP_PRIORITY_HIGHEST,
                    SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
                break;
              }
              case 309: {
                SimConnect_TransmitClientEvent(
                    hInputSimConnect, 0, DEFINITION_AP_WING_LEVELER_ON, 0,
                    SIMCONNECT_GROUP_PRIORITY_HIGHEST,
                    SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
                break;
              }
              case 310: {
                SimConnect_TransmitClientEvent(
                    hInputSimConnect, 0, DEFINITION_AP_BC_HOLD_ON, 0,
                    SIMCONNECT_GROUP_PRIORITY_HIGHEST,
                    SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
                break;
              }
              case 311: {
                SimConnect_TransmitClientEvent(
                    hInputSimConnect, 0, DEFINITION_AP_NAV1_HOLD_ON, 0,
                    SIMCONNECT_GROUP_PRIORITY_HIGHEST,
                    SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
                break;
              }
              case 312: {
                SimConnect_TransmitClientEvent(
                    hInputSimConnect, 0, DEFINITION_AP_ATT_HOLD_OFF, 0,
                    SIMCONNECT_GROUP_PRIORITY_HIGHEST,
                    SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
                break;
              }
              case 313: {
                SimConnect_TransmitClientEvent(
                    hInputSimConnect, 0, DEFINITION_AP_LOC_HOLD_OFF, 0,
                    SIMCONNECT_GROUP_PRIORITY_HIGHEST,
                    SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
                break;
              }
              case 314: {
                SimConnect_TransmitClientEvent(
                    hInputSimConnect, 0, DEFINITION_AP_APR_HOLD_OFF, 0,
                    SIMCONNECT_GROUP_PRIORITY_HIGHEST,
                    SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
                break;
              }
              case 315: {
                SimConnect_TransmitClientEvent(
                    hInputSimConnect, 0, DEFINITION_AP_HDG_HOLD_OFF, 0,
                    SIMCONNECT_GROUP_PRIORITY_HIGHEST,
                    SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
                break;
              }
              case 316: {
                SimConnect_TransmitClientEvent(
                    hInputSimConnect, 0, DEFINITION_AP_ALT_HOLD_OFF, 0,
                    SIMCONNECT_GROUP_PRIORITY_HIGHEST,
                    SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
                break;
              }
              case 317: {
                SimConnect_TransmitClientEvent(
                    hInputSimConnect, 0, DEFINITION_AP_WING_LEVELER_OFF, 0,
                    SIMCONNECT_GROUP_PRIORITY_HIGHEST,
                    SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
                break;
              }
              case 318: {
                SimConnect_TransmitClientEvent(
                    hInputSimConnect, 0, DEFINITION_AP_BC_HOLD_OFF, 0,
                    SIMCONNECT_GROUP_PRIORITY_HIGHEST,
                    SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
                break;
              }
              case 319: {
                SimConnect_TransmitClientEvent(
                    hInputSimConnect, 0, DEFINITION_AP_NAV1_HOLD_OFF, 0,
                    SIMCONNECT_GROUP_PRIORITY_HIGHEST,
                    SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
                break;
              }
              case 320: {
                SimConnect_TransmitClientEvent(
                    hInputSimConnect, 0, DEFINITION_AP_AIRSPEED_HOLD, 0,
                    SIMCONNECT_GROUP_PRIORITY_HIGHEST,
                    SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
                break;
              }
              case 321: {
                SimConnect_TransmitClientEvent(
                    hInputSimConnect, 0, DEFINITION_AP_PANEL_SPEED_HOLD, 0,
                    SIMCONNECT_GROUP_PRIORITY_HIGHEST,
                    SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
                break;
              }
              case 322: {
                SimConnect_TransmitClientEvent(
                    hInputSimConnect, 0, DEFINITION_AP_ALT_VAR_INC, 0,
                    SIMCONNECT_GROUP_PRIORITY_HIGHEST,
                    SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
                break;
              }
              case 323: {
                SimConnect_TransmitClientEvent(
                    hInputSimConnect, 0, DEFINITION_AP_ALT_VAR_DEC, 0,
                    SIMCONNECT_GROUP_PRIORITY_HIGHEST,
                    SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
                break;
              }
              case 324: {
                SimConnect_TransmitClientEvent(
                    hInputSimConnect, 0, DEFINITION_AP_VS_VAR_INC, 0,
                    SIMCONNECT_GROUP_PRIORITY_HIGHEST,
                    SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
                break;
              }
              case 325: {
                SimConnect_TransmitClientEvent(
                    hInputSimConnect, 0, DEFINITION_AP_VS_VAR_DEC, 0,
                    SIMCONNECT_GROUP_PRIORITY_HIGHEST,
                    SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
                break;
              }
              case 326: {
                SimConnect_TransmitClientEvent(
                    hInputSimConnect, 0, DEFINITION_AP_SPD_VAR_INC, 0,
                    SIMCONNECT_GROUP_PRIORITY_HIGHEST,
                    SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
                break;
              }
              case 327: {
                SimConnect_TransmitClientEvent(
                    hInputSimConnect, 0, DEFINITION_AP_SPD_VAR_DEC, 0,
                    SIMCONNECT_GROUP_PRIORITY_HIGHEST,
                    SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
                break;
              }
              case 328: {
                SimConnect_TransmitClientEvent(
                    hInputSimConnect, 0, DEFINITION_AP_PANEL_MACH_HOLD, 0,
                    SIMCONNECT_GROUP_PRIORITY_HIGHEST,
                    SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
                break;
              }
              case 329: {
                SimConnect_TransmitClientEvent(
                    hInputSimConnect, 0, DEFINITION_AP_MACH_VAR_INC, 0,
                    SIMCONNECT_GROUP_PRIORITY_HIGHEST,
                    SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
                break;
              }
              case 330: {
                SimConnect_TransmitClientEvent(
                    hInputSimConnect, 0, DEFINITION_AP_MACH_VAR_DEC, 0,
                    SIMCONNECT_GROUP_PRIORITY_HIGHEST,
                    SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
                break;
              }
              case 331: {
                SimConnect_TransmitClientEvent(
                    hInputSimConnect, 0, DEFINITION_AP_MACH_HOLD, 0,
                    SIMCONNECT_GROUP_PRIORITY_HIGHEST,
                    SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
                break;
              }
              case 332: {
                SimConnect_TransmitClientEvent(
                    hInputSimConnect, 0, DEFINITION_AP_ALT_VAR_SET_METRIC, 0,
                    SIMCONNECT_GROUP_PRIORITY_HIGHEST,
                    SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
                break;
              }
              case 333: {
                SimConnect_TransmitClientEvent(
                    hInputSimConnect, 0, DEFINITION_AP_VS_VAR_SET_ENGLISH, 0,
                    SIMCONNECT_GROUP_PRIORITY_HIGHEST,
                    SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
                break;
              }
              case 334: {
                SimConnect_TransmitClientEvent(
                    hInputSimConnect, 0, DEFINITION_AP_SPD_VAR_SET, 0,
                    SIMCONNECT_GROUP_PRIORITY_HIGHEST,
                    SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
                break;
              }
              case 335: {
                SimConnect_TransmitClientEvent(
                    hInputSimConnect, 0, DEFINITION_AP_MACH_VAR_SET, 0,
                    SIMCONNECT_GROUP_PRIORITY_HIGHEST,
                    SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
                break;
              }
              case 336: {
                SimConnect_TransmitClientEvent(
                    hInputSimConnect, 0, DEFINITION_AP_AIRSPEED_ON, 0,
                    SIMCONNECT_GROUP_PRIORITY_HIGHEST,
                    SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
                break;
              }
              case 337: {
                SimConnect_TransmitClientEvent(
                    hInputSimConnect, 0, DEFINITION_AP_AIRSPEED_OFF, 0,
                    SIMCONNECT_GROUP_PRIORITY_HIGHEST,
                    SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
                break;
              }
              case 338: {
                SimConnect_TransmitClientEvent(
                    hInputSimConnect, 0, DEFINITION_AP_AIRSPEED_SET, 0,
                    SIMCONNECT_GROUP_PRIORITY_HIGHEST,
                    SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
                break;
              }
              case 339: {
                SimConnect_TransmitClientEvent(
                    hInputSimConnect, 0, DEFINITION_AP_MACH_ON, 0,
                    SIMCONNECT_GROUP_PRIORITY_HIGHEST,
                    SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
                break;
              }
              case 340: {
                SimConnect_TransmitClientEvent(
                    hInputSimConnect, 0, DEFINITION_AP_MACH_OFF, 0,
                    SIMCONNECT_GROUP_PRIORITY_HIGHEST,
                    SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
                break;
              }
              case 341: {
                SimConnect_TransmitClientEvent(
                    hInputSimConnect, 0, DEFINITION_AP_MACH_SET, 0,
                    SIMCONNECT_GROUP_PRIORITY_HIGHEST,
                    SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
                break;
              }
              case 342: {
                SimConnect_TransmitClientEvent(
                    hInputSimConnect, 0, DEFINITION_AP_PANEL_ALTITUDE_ON, 0,
                    SIMCONNECT_GROUP_PRIORITY_HIGHEST,
                    SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
                break;
              }
              case 343: {
                SimConnect_TransmitClientEvent(
                    hInputSimConnect, 0, DEFINITION_AP_PANEL_ALTITUDE_OFF, 0,
                    SIMCONNECT_GROUP_PRIORITY_HIGHEST,
                    SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
                break;
              }
              case 345: {
                SimConnect_TransmitClientEvent(
                    hInputSimConnect, 0, DEFINITION_AP_PANEL_ALTITUDE_SET, 0,
                    SIMCONNECT_GROUP_PRIORITY_HIGHEST,
                    SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
                break;
              }
              case 346: {
                SimConnect_TransmitClientEvent(
                    hInputSimConnect, 0, DEFINITION_AP_PANEL_HEADING_ON, 0,
                    SIMCONNECT_GROUP_PRIORITY_HIGHEST,
                    SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
                break;
              }
              case 347: {
                SimConnect_TransmitClientEvent(
                    hInputSimConnect, 0, DEFINITION_AP_PANEL_HEADING_OFF, 0,
                    SIMCONNECT_GROUP_PRIORITY_HIGHEST,
                    SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
                break;
              }
              case 348: {
                SimConnect_TransmitClientEvent(
                    hInputSimConnect, 0, DEFINITION_AP_PANEL_HEADING_SET, 0,
                    SIMCONNECT_GROUP_PRIORITY_HIGHEST,
                    SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
                break;
              }
              case 349: {
                SimConnect_TransmitClientEvent(
                    hInputSimConnect, 0, DEFINITION_AP_PANEL_MACH_ON, 0,
                    SIMCONNECT_GROUP_PRIORITY_HIGHEST,
                    SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
                break;
              }
              case 350: {
                SimConnect_TransmitClientEvent(
                    hInputSimConnect, 0, DEFINITION_AP_PANEL_MACH_OFF, 0,
                    SIMCONNECT_GROUP_PRIORITY_HIGHEST,
                    SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
                break;
              }
              case 351: {
                SimConnect_TransmitClientEvent(
                    hInputSimConnect, 0, DEFINITION_AP_PANEL_MACH_SET, 0,
                    SIMCONNECT_GROUP_PRIORITY_HIGHEST,
                    SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
                break;
              }
              case 352: {
                SimConnect_TransmitClientEvent(
                    hInputSimConnect, 0, DEFINITION_AP_PANEL_SPEED_ON, 0,
                    SIMCONNECT_GROUP_PRIORITY_HIGHEST,
                    SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
                break;
              }
              case 353: {
                SimConnect_TransmitClientEvent(
                    hInputSimConnect, 0, DEFINITION_AP_PANEL_SPEED_OFF, 0,
                    SIMCONNECT_GROUP_PRIORITY_HIGHEST,
                    SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
                break;
              }
              case 354: {
                SimConnect_TransmitClientEvent(
                    hInputSimConnect, 0, DEFINITION_AP_PANEL_SPEED_SET, 0,
                    SIMCONNECT_GROUP_PRIORITY_HIGHEST,
                    SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
                break;
              }
              case 355: {
                SimConnect_TransmitClientEvent(
                    hInputSimConnect, 0, DEFINITION_AP_ALT_VAR_SET_ENGLISH, 0,
                    SIMCONNECT_GROUP_PRIORITY_HIGHEST,
                    SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
                break;
              }
              case 356: {
                SimConnect_TransmitClientEvent(
                    hInputSimConnect, 0, DEFINITION_AP_VS_VAR_SET_METRIC, 0,
                    SIMCONNECT_GROUP_PRIORITY_HIGHEST,
                    SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
                break;
              }
              case 357: {
                SimConnect_TransmitClientEvent(
                    hInputSimConnect, 0, DEFINITION_AP_PANEL_SPEED_HOLD_TOGGLE,
                    0, SIMCONNECT_GROUP_PRIORITY_HIGHEST,
                    SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
                break;
              }
              case 358: {
                SimConnect_TransmitClientEvent(
                    hInputSimConnect, 0, DEFINITION_AP_PANEL_MACH_HOLD_TOGGLE,
                    0, SIMCONNECT_GROUP_PRIORITY_HIGHEST,
                    SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
                break;
              }
              case 359: {
                SimConnect_TransmitClientEvent(
                    hInputSimConnect, 0, DEFINITION_AP_NAV_SELECT_SET, 0,
                    SIMCONNECT_GROUP_PRIORITY_HIGHEST,
                    SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
                break;
              }
              case 360: {
                SimConnect_TransmitClientEvent(
                    hInputSimConnect, 0, DEFINITION_AP_PITCH_REF_INC_UP, 0,
                    SIMCONNECT_GROUP_PRIORITY_HIGHEST,
                    SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
                break;
              }
              case 361: {
                SimConnect_TransmitClientEvent(
                    hInputSimConnect, 0, DEFINITION_AP_PITCH_REF_INC_DN, 0,
                    SIMCONNECT_GROUP_PRIORITY_HIGHEST,
                    SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
                break;
              }
              case 362: {
                SimConnect_TransmitClientEvent(
                    hInputSimConnect, 0, DEFINITION_AP_PITCH_REF_SELECT, 0,
                    SIMCONNECT_GROUP_PRIORITY_HIGHEST,
                    SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
                break;
              }
              case 363: {
                SimConnect_TransmitClientEvent(
                    hInputSimConnect, 0, DEFINITION_AP_ATT_HOLD, 0,
                    SIMCONNECT_GROUP_PRIORITY_HIGHEST,
                    SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
                break;
              }
              case 364: {
                SimConnect_TransmitClientEvent(
                    hInputSimConnect, 0, DEFINITION_AP_LOC_HOLD, 0,
                    SIMCONNECT_GROUP_PRIORITY_HIGHEST,
                    SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
                break;
              }
              case 365: {
                SimConnect_TransmitClientEvent(
                    hInputSimConnect, 0, DEFINITION_AP_APR_HOLD, 0,
                    SIMCONNECT_GROUP_PRIORITY_HIGHEST,
                    SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
                break;
              }
              case 366: {
                SimConnect_TransmitClientEvent(
                    hInputSimConnect, 0, DEFINITION_AP_HDG_HOLD, 0,
                    SIMCONNECT_GROUP_PRIORITY_HIGHEST,
                    SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
                break;
              }
              case 367: {
                SimConnect_TransmitClientEvent(
                    hInputSimConnect, 0, DEFINITION_AP_ALT_HOLD, 0,
                    SIMCONNECT_GROUP_PRIORITY_HIGHEST,
                    SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
                break;
              }
              case 368: {
                SimConnect_TransmitClientEvent(
                    hInputSimConnect, 0, DEFINITION_AP_WING_LEVELER, 0,
                    SIMCONNECT_GROUP_PRIORITY_HIGHEST,
                    SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
                break;
              }
              case 369: {
                SimConnect_TransmitClientEvent(
                    hInputSimConnect, 0, DEFINITION_AP_BC_HOLD, 0,
                    SIMCONNECT_GROUP_PRIORITY_HIGHEST,
                    SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
                break;
              }
              case 370: {
                SimConnect_TransmitClientEvent(
                    hInputSimConnect, 0, DEFINITION_AP_NAV1_HOLD, 0,
                    SIMCONNECT_GROUP_PRIORITY_HIGHEST,
                    SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
                break;
              }
              case 371: {
                SimConnect_TransmitClientEvent(
                    hInputSimConnect, 0, DEFINITION_HEADING_BUG_INC, 0,
                    SIMCONNECT_GROUP_PRIORITY_HIGHEST,
                    SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
                break;
              }
              case 372: {
                SimConnect_TransmitClientEvent(
                    hInputSimConnect, 0, DEFINITION_HEADING_BUG_DEC, 0,
                    SIMCONNECT_GROUP_PRIORITY_HIGHEST,
                    SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
                break;
              }
              case 373: {
                SimConnect_TransmitClientEvent(
                    hInputSimConnect, 0, DEFINITION_TOGGLE_FLIGHT_DIRECTOR, 0,
                    SIMCONNECT_GROUP_PRIORITY_HIGHEST,
                    SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
                break;
              }
              case 374: {
                SimConnect_TransmitClientEvent(
                    hInputSimConnect, 0, DEFINITION_AP_VS_HOLD, 0,
                    SIMCONNECT_GROUP_PRIORITY_HIGHEST,
                    SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
                break;
              }
              case 375: {
                SimConnect_TransmitClientEvent(
                    hInputSimConnect, 0, DEFINITION_KOHLSMAN_INC, 0,
                    SIMCONNECT_GROUP_PRIORITY_HIGHEST,
                    SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
                break;
              }
              case 376: {
                SimConnect_TransmitClientEvent(
                    hInputSimConnect, 0, DEFINITION_KOHLSMAN_DEC, 0,
                    SIMCONNECT_GROUP_PRIORITY_HIGHEST,
                    SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
                break;
              }
              case 378: {
                SimConnect_TransmitClientEvent(
                    hInputSimConnect, 0, DEFINITION_AUTO_THROTTLE_ARM, 0,
                    SIMCONNECT_GROUP_PRIORITY_HIGHEST,
                    SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
                break;
              }
              case 379: {
                SimConnect_TransmitClientEvent(
                    hInputSimConnect, 0, DEFINITION_AUTO_THROTTLE_TO_GA, 0,
                    SIMCONNECT_GROUP_PRIORITY_HIGHEST,
                    SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
                break;
              }
              // Avionics
              case 401: {
                SimConnect_TransmitClientEvent(
                    hInputSimConnect, 0, DEFINITION_TOGGLE_AVIONICS1_MASTER_OFF,
                    0, SIMCONNECT_GROUP_PRIORITY_HIGHEST,
                    SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
                break;
              }
              case 402: {
                SimConnect_TransmitClientEvent(
                    hInputSimConnect, 0, DEFINITION_TOGGLE_AVIONICS1_MASTER_ON,
                    1, SIMCONNECT_GROUP_PRIORITY_HIGHEST,
                    SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
                break;
              }
              case 403: {
                SimConnect_TransmitClientEvent(
                    hInputSimConnect, 0, DEFINITION_TOGGLE_AVIONICS2_MASTER_OFF,
                    0, SIMCONNECT_GROUP_PRIORITY_HIGHEST,
                    SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
                break;
              }
              case 404: {
                SimConnect_TransmitClientEvent(
                    hInputSimConnect, 0, DEFINITION_TOGGLE_AVIONICS2_MASTER_ON,
                    1, SIMCONNECT_GROUP_PRIORITY_HIGHEST,
                    SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
                break;
              }
              // Battery
              case 405: {
                SimConnect_TransmitClientEvent(
                    hInputSimConnect, 0, DEFINITION_TOGGLE_MASTER_BATTERY, 1,
                    SIMCONNECT_GROUP_PRIORITY_HIGHEST,
                    SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
                break;
              }
              case 406: {
                SimConnect_TransmitClientEvent(
                    hInputSimConnect, 0, DEFINITION_TOGGLE_MASTER_BATTERY, 2,
                    SIMCONNECT_GROUP_PRIORITY_HIGHEST,
                    SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
                break;
              }
              case 407: {
                SimConnect_TransmitClientEvent(
                    hInputSimConnect, 0, DEFINITION_TOGGLE_MASTER_ALTERNATOR, 0,
                    SIMCONNECT_GROUP_PRIORITY_HIGHEST,
                    SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
                break;
              }
              case 408: {
                SimConnect_TransmitClientEvent(
                    hInputSimConnect, 0, DEFINITION_TOGGLE_EXTERNAL_POWER, 0,
                    SIMCONNECT_GROUP_PRIORITY_HIGHEST,
                    SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
                break;
              }
              case 420: {
                SimConnect_TransmitClientEvent(
                    hInputSimConnect, 0, DEFINITION_PARKING_BRAKE, 0,
                    SIMCONNECT_GROUP_PRIORITY_HIGHEST,
                    SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
                break;
              }
              case 501: {
                SimConnect_TransmitClientEvent(
                    hInputSimConnect, 0, DEFINITION_FLAPS_UP, 0,
                    SIMCONNECT_GROUP_PRIORITY_HIGHEST,
                    SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
                break;
              }
              case 502: {
                SimConnect_TransmitClientEvent(
                    hInputSimConnect, 0, DEFINITION_FLAPS_1, 0,
                    SIMCONNECT_GROUP_PRIORITY_HIGHEST,
                    SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
                break;
              }
              case 503: {
                SimConnect_TransmitClientEvent(
                    hInputSimConnect, 0, DEFINITION_FLAPS_2, 0,
                    SIMCONNECT_GROUP_PRIORITY_HIGHEST,
                    SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
                break;
              }
              case 504: {
                SimConnect_TransmitClientEvent(
                    hInputSimConnect, 0, DEFINITION_FLAPS_3, 0,
                    SIMCONNECT_GROUP_PRIORITY_HIGHEST,
                    SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
                break;
              }
              case 505: {
                SimConnect_TransmitClientEvent(
                    hInputSimConnect, 0, DEFINITION_FLAPS_DOWN, 0,
                    SIMCONNECT_GROUP_PRIORITY_HIGHEST,
                    SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
                break;
              }
              case 506: {
                SimConnect_TransmitClientEvent(
                    hInputSimConnect, 0, DEFINITION_FLAPS_INCR, 0,
                    SIMCONNECT_GROUP_PRIORITY_HIGHEST,
                    SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
                break;
              }
              case 507: {
                SimConnect_TransmitClientEvent(
                    hInputSimConnect, 0, DEFINITION_FLAPS_DECR, 0,
                    SIMCONNECT_GROUP_PRIORITY_HIGHEST,
                    SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
                break;
              }
              default: {
                break;
              }
            }
          } catch (const std::exception &e) {
            cout << "error" << endl;
          }
        }
      }
    }

    SimConnect_CallDispatch(hInputSimConnect, MyDispatchProcInput, NULL);
    Sleep(1);
    hr = SimConnect_Close(hInputSimConnect);
    arduinoInput->closeSerial();
    quit();
  }
}

InputWorker::~InputWorker() {
  mutex.lock();
  abortInput = true;
  condition.wakeOne();
  mutex.unlock();
}
