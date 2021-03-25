
#include <headers/InputSwitchHandler.h>
#include <headers/SimConnect.h>
#include <qsettings.h>
#include <qstandardpaths.h>
#include <tchar.h>
#include <windows.h>

#include <cstdio>
#include <future>
#include <iostream>
#include <string>

#include "headers/inputenum.h"
#include "stdio.h"
#include "strsafe.h"

#define Bcd2Dec(BcdNum) HornerScheme(BcdNum, 0x10, 10)
#define Dec2Bcd(DecNum) HornerScheme(DecNum, 10, 0x10)

using namespace std;

char *token, *next_token;
bool inputs[5];

int counter = 0;
int engines = 4;

double oldValThrottle[4];
double oldValMixture[2];

double oldValProps[2] = {-10, -10};

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
  double mixture_percent[2] = {0, 0};
};

structMixtureControl mc;
structThrottleControl tc;
structPropControl pc;
structBattery1 bc1;
structBattery2 bc2;

int trim;
int oldTrim;

int rudderAxis;
int oldRudderAxis;

int leftBrake;
int oldLeftBrake;

int rightBrake;
int oldRightBrake;

InputEnum inputDefinitions = InputEnum();

UINT32 HornerScheme(UINT32 Num, UINT32 Divider, UINT32 Factor) {
  UINT32 Remainder = 0, Quotient = 0, Result = 0;
  Remainder = Num % Divider;
  Quotient = Num / Divider;
  if (!(Quotient == 0 && Remainder == 0))
    Result += HornerScheme(Quotient, Divider, Factor) * Factor + Remainder;
  return Result;
}

void InputSwitchHandler::set_throttle_values(int index) {
  // Throttle control
  try {
    token = strtok_s(receivedString[index], " ", &next_token);
    cout << receivedString[index] << endl;
    counter = 0;

    while (token != nullptr && counter < 5) {
      cout << receivedString << ":Received" << endl;
      cout << token << ":Counter" << counter << endl;
      int engineBuffer[4];
      if (token != nullptr) {
        const auto incVal = strtod(token, nullptr);

        if (counter != 0 && counter != 5) {
          std::cout << incVal << std::endl;
          if (incVal < 10.0) {
            if (oldValThrottle[counter - 1] < 20.0) {
              oldValThrottle[counter - 1] = incVal;
            }
          } else {
            oldValThrottle[counter - 1] = incVal;
          }
          engineBuffer[counter - 1] = oldValThrottle[counter - 1];
          cout << "counter: " << counter << " val: " << incVal << endl;
        }

        token = strtok_s(nullptr, " ", &next_token);

        counter++;
      }
      if (counter == 4) {
        for (int i = 0; i < 4; i++) {
          tc.throttle_percent[i] = engineBuffer[i];
        }
      }
    }
  } catch (const std::exception &e) {
    cout << "error in throttle" << endl;
  }
}

void InputSwitchHandler::setMixtureValues(int index) {
  try {
    token = strtok_s(receivedString[index], " ", &next_token);

    counter = 0;

    while (token != nullptr && counter < 3) {
      cout << receivedString << ":Received" << endl;
      cout << token << ":Counter" << counter << endl;

      if (token != nullptr) {
        auto incVal = strtod(token, nullptr);

        if (counter != 0) {
          cout << incVal << std::endl;
          if (incVal < 10.0) {
            if (oldValMixture[counter - 1] < 20.0) {
              oldValMixture[counter - 1] = incVal;
            }
          } else {
            if (incVal == 98) {
              incVal = 100;
            }
            oldValMixture[counter - 1] = incVal;
          }
          mc.mixture_percent[counter - 1] = oldValMixture[counter - 1];
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

void InputSwitchHandler::set_prop_values(int index) {
  // Throttle control
  try {
    token = strtok_s(receivedString[index], " ", &next_token);

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

void InputSwitchHandler::setElevatorTrim(int index) {
  try {
    token = strtok_s(receivedString[index], " ", &next_token);

    counter = 0;
    while (token != nullptr && counter < 2) {
      if (counter == 1) {
        trim = stoi(token);
      }
      token = strtok_s(nullptr, " ", &next_token);
      counter++;
    }
    int diff = std::abs(trim - oldTrim);
    cout << diff << endl;
    if (diff < 3000 || oldTrim == NULL) {
      SimConnect_TransmitClientEvent(
          connect, 0, inputDefinitions.DEFINITION_ELEVATOR_TRIM_SET, trim,
          SIMCONNECT_GROUP_PRIORITY_HIGHEST,
          SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
      oldTrim = trim;
    }
  } catch (const std::exception &e) {
    cout << "error in trim" << endl;
  }
}

void InputSwitchHandler::setRudder(int index) {
  try {
    token = strtok_s(receivedString[index], " ", &next_token);

    counter = 0;
    while (token != nullptr && counter < 2) {
      if (counter == 1) {
        rudderAxis = stoi(token);
      }
      token = strtok_s(nullptr, " ", &next_token);
      counter++;
    }
    int diff = std::abs(rudderAxis - oldRudderAxis);
    cout << diff << endl;
    if (diff < 3000 || oldRudderAxis == NULL) {
      SimConnect_TransmitClientEvent(
          connect, 0, inputDefinitions.DEFINITION_AXIS_RUDDER_SET, rudderAxis,
          SIMCONNECT_GROUP_PRIORITY_HIGHEST,
          SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
      oldRudderAxis = rudderAxis;
    }
  } catch (const std::exception &e) {
    cout << "error in rudder" << endl;
  }
}

void InputSwitchHandler::setBrakeAxis(int index) {
  try {
    token = strtok_s(receivedString[index], " ", &next_token);

    counter = 0;
    while (token != nullptr && counter < 3) {
      if (counter == 1) {
        leftBrake = stoi(token);
      }
      if (counter == 2) {
        rightBrake = stoi(token);
      }
      token = strtok_s(nullptr, " ", &next_token);
      counter++;
    }
    int diff = std::abs(leftBrake - oldLeftBrake);
    cout << diff << endl;
    if (diff < 3000 || oldRightBrake == NULL) {
      SimConnect_TransmitClientEvent(
          connect, 0, inputDefinitions.DEFINITION_AXIS_RIGHT_BRAKE_SET,
          rightBrake, SIMCONNECT_GROUP_PRIORITY_HIGHEST,
          SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
      oldLeftBrake = leftBrake;
    }
    diff = std::abs(rightBrake - oldRightBrake);
    if (diff < 3000 || oldLeftBrake == NULL) {
      SimConnect_TransmitClientEvent(
          connect, 0, inputDefinitions.DEFINITION_AXIS_LEFT_BRAKE_SET,
          leftBrake, SIMCONNECT_GROUP_PRIORITY_HIGHEST,
          SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
      oldRightBrake = rightBrake;
    }
  } catch (const std::exception &e) {
    cout << "error in brakes" << endl;
  }
}

int InputSwitchHandler::setComs(int index, int comNo) {
  int value =
      stoi(std::string(&receivedString[index][4], &receivedString[index][10]));
  if (value % (value / 10) == 5 || value % (value / 10) == 85) {
    switch (comNo) {
      case 1: {
        SimConnect_TransmitClientEvent(
            connect, SIMCONNECT_OBJECT_ID_USER,
            inputDefinitions.DEFINITION_COM_RADIO_FRACT_INC, 0,
            SIMCONNECT_GROUP_PRIORITY_HIGHEST,
            SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
      }
      case 2: {
        SimConnect_TransmitClientEvent(
            connect, SIMCONNECT_OBJECT_ID_USER,
            inputDefinitions.DEFINITION_COM2_RADIO_FRACT_INC, 0,
            SIMCONNECT_GROUP_PRIORITY_HIGHEST,
            SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
      }
      default: {
      }
    }
  }
  return Dec2Bcd(value / 10);
}

void InputSwitchHandler::sendBasicCommand(SIMCONNECT_CLIENT_EVENT_ID eventID) {
  SimConnect_TransmitClientEvent(connect, 0, eventID, 0,
                                 SIMCONNECT_GROUP_PRIORITY_HIGHEST,
                                 SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
};
void InputSwitchHandler::sendBasicCommandOn(
    SIMCONNECT_CLIENT_EVENT_ID eventID) {
  SimConnect_TransmitClientEvent(connect, 0, eventID, 1,
                                 SIMCONNECT_GROUP_PRIORITY_HIGHEST,
                                 SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
};

void InputSwitchHandler::sendBasicCommandOff(
    SIMCONNECT_CLIENT_EVENT_ID eventID) {
  SimConnect_TransmitClientEvent(connect, 0, eventID, 0,
                                 SIMCONNECT_GROUP_PRIORITY_HIGHEST,
                                 SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
};

InputSwitchHandler::InputSwitchHandler() {}

void InputSwitchHandler::switchHandling(int index) {
  Sleep(10);
  //
  if (sizeof(receivedString) > 2) {
    prefix = std::string(&receivedString[index][0], &receivedString[index][3]);
    try {
      int prefixVal = stoi(prefix);
      switch (prefixVal) {
        case 198: {
          set_prop_values(index);
          SimConnect_SetDataOnSimObject(
              connect, inputDefinitions.DEFINITION_PROPELLER_LEVER_POSITION_1,
              SIMCONNECT_OBJECT_ID_USER, 0, 2, sizeof(pc.prop_percent[0]),
              &pc.prop_percent[0]);

          SimConnect_SetDataOnSimObject(
              connect, inputDefinitions.DEFINITION_PROPELLER_LEVER_POSITION_2,
              SIMCONNECT_OBJECT_ID_USER, 0, 2, sizeof(pc.prop_percent[1]),
              &pc.prop_percent[1]);
          break;
        }

        case 199: {
          set_throttle_values(index);

          SimConnect_SetDataOnSimObject(
              connect, inputDefinitions.DEFINITION_THROTTLE_1,
              SIMCONNECT_OBJECT_ID_USER, 0, 4, sizeof(tc.throttle_percent[0]),
              &tc.throttle_percent[0]);

          SimConnect_SetDataOnSimObject(
              connect, inputDefinitions.DEFINITION_THROTTLE_2,
              SIMCONNECT_OBJECT_ID_USER, 0, 4, sizeof(tc.throttle_percent[1]),
              &tc.throttle_percent[1]);

          SimConnect_SetDataOnSimObject(
              connect, inputDefinitions.DEFINITION_THROTTLE_3,
              SIMCONNECT_OBJECT_ID_USER, 0, 4, sizeof(tc.throttle_percent[2]),
              &tc.throttle_percent[2]);

          SimConnect_SetDataOnSimObject(
              connect, inputDefinitions.DEFINITION_THROTTLE_4,
              SIMCONNECT_OBJECT_ID_USER, 0, 4, sizeof(tc.throttle_percent[3]),
              &tc.throttle_percent[3]);

          break;
        }
        case 115: {
          setMixtureValues(index);
          SimConnect_SetDataOnSimObject(
              connect, inputDefinitions.DEFINITION_MIXTURE_1,
              SIMCONNECT_OBJECT_ID_USER, 0, 2, sizeof(mc.mixture_percent[0]),
              &mc.mixture_percent[0]);

          SimConnect_SetDataOnSimObject(
              connect, inputDefinitions.DEFINITION_MIXTURE_2,
              SIMCONNECT_OBJECT_ID_USER, 0, 2, sizeof(mc.mixture_percent[1]),
              &mc.mixture_percent);
          break;
        }

        case 100: {
          SimConnect_TransmitClientEvent(
              connect, SIMCONNECT_OBJECT_ID_USER,
              inputDefinitions.DEFINITION_COM_1_SET, setComs(index, 1),
              SIMCONNECT_GROUP_PRIORITY_HIGHEST,
              SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);

          break;
        }
        case 102: {
          int value = stoi(std::string(&receivedString[index][4],
                                       &receivedString[index][10]));
          SimConnect_TransmitClientEvent(
              connect, SIMCONNECT_OBJECT_ID_USER,
              inputDefinitions.DEFINITION_COM_2_SET, Dec2Bcd(value / 10),
              SIMCONNECT_GROUP_PRIORITY_HIGHEST,
              SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
          if (value % (value / 10) == 5 || value % (value / 10) == 85) {
            SimConnect_TransmitClientEvent(
                connect, SIMCONNECT_OBJECT_ID_USER,
                inputDefinitions.DEFINITION_COM2_RADIO_FRACT_INC, 0,
                SIMCONNECT_GROUP_PRIORITY_HIGHEST,
                SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
          }
          break;
        }
        case 111: {
          SimConnect_TransmitClientEvent(
              connect, 0, inputDefinitions.DEFINITION_COM1_RADIO_WHOLE_DEC, 0,
              SIMCONNECT_GROUP_PRIORITY_HIGHEST,
              SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
          break;
        }
        case 112: {
          SimConnect_TransmitClientEvent(
              connect, 0, inputDefinitions.DEFINITION_COM1_RADIO_WHOLE_INC, 0,
              SIMCONNECT_GROUP_PRIORITY_HIGHEST,
              SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);

          break;
        }
        case 113: {
          SimConnect_TransmitClientEvent(
              connect, 0, inputDefinitions.DEFINITION_COM2_RADIO_WHOLE_DEC, 0,
              SIMCONNECT_GROUP_PRIORITY_HIGHEST,
              SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
          break;
        }
        case 114: {
          sendBasicCommand(inputDefinitions.DEFINITION_COM2_RADIO_WHOLE_INC);
          break;
        }

          // Swap com1
        case 116: {
          sendBasicCommand(inputDefinitions.DEFINITION_COM_STANDBY_SWAP);
          break;
        }
          // Swap com2
        case 117: {
          sendBasicCommand(inputDefinitions.DEFINITION_COM_2_STANDBY_SWAP);
          break;
        }
          // Swap nav1
        case 118: {
          sendBasicCommand(inputDefinitions.DEFINITION_NAV_1_STANDBY_SWAP);
          break;
        }
          // Swap nav1
        case 119: {
          sendBasicCommand(inputDefinitions.DEFINITION_NAV_2_STANDBY_SWAP);
          break;
        }

        case 120: {
          sendBasicCommand(inputDefinitions.DEFINITION_COM2_RADIO_FRACT_DEC);
          break;
        }
        case 121: {
          sendBasicCommand(inputDefinitions.DEFINITION_COM2_RADIO_FRACT_INC);
          break;
        }
        case 122: {
          sendBasicCommand(
              inputDefinitions.DEFINITION_COM2_RADIO_FRACT_DEC_CARRY);
          break;
        }
        case 123: {
          sendBasicCommand(
              inputDefinitions.DEFINITION_COM2_RADIO_FRACT_INC_CARRY);
          break;
        }
        case 124: {
          sendBasicCommand(inputDefinitions.DEFINITION_COM_RADIO_FRACT_DEC);
          break;
        }
        case 125: {
          sendBasicCommand(inputDefinitions.DEFINITION_COM_RADIO_FRACT_INC);
          break;
        }
        case 126: {
          sendBasicCommand(
              inputDefinitions.DEFINITION_COM_RADIO_FRACT_DEC_CARRY);
          break;
        }
        case 127: {
          sendBasicCommand(
              inputDefinitions.DEFINITION_COM_RADIO_FRACT_INC_CARRY);
          break;
        }

        // NAV
        case 128: {
          sendBasicCommand(inputDefinitions.DEFINITION_NAV_1_RADIO_FRACT_INC);
          break;
        }
        case 129: {
          sendBasicCommand(inputDefinitions.DEFINITION_NAV_1_RADIO_FRACT_DEC);
          break;
        }
        case 130: {
          sendBasicCommand(inputDefinitions.DEFINITION_NAV_2_RADIO_FRACT_INC);
          break;
        }
        case 131: {
          sendBasicCommand(inputDefinitions.DEFINITION_NAV_2_RADIO_FRACT_DEC);
          break;
        }
        case 132: {
          sendBasicCommand(inputDefinitions.DEFINITION_NAV_1_RADIO_WHOLE_INC);
          break;
        }
        case 133: {
          sendBasicCommand(inputDefinitions.DEFINITION_NAV_1_RADIO_WHOLE_DEC);
          break;
        }
        case 134: {
          sendBasicCommand(inputDefinitions.DEFINITION_NAV_2_RADIO_WHOLE_INC);
          break;
        }
        case 135: {
          sendBasicCommand(inputDefinitions.DEFINITION_NAV_2_RADIO_WHOLE_DEC);
          break;
        }
        // DME
        case 136: {
          sendBasicCommand(
              inputDefinitions.DEFINITION_RADIO_SELECTED_DME1_IDENT_ENABLE);
          break;
        }
        case 137: {
          sendBasicCommand(
              inputDefinitions.DEFINITION_RADIO_SELECTED_DME1_IDENT_DISABLE);
          break;
        }
        case 138: {
          sendBasicCommand(
              inputDefinitions.DEFINITION_RADIO_SELECTED_DME1_IDENT_SET);
          break;
        }
        case 139: {
          sendBasicCommand(
              inputDefinitions.DEFINITION_RADIO_SELECTED_DM1E_IDENT_TOGGLE);
          break;
        }
        case 140: {
          sendBasicCommand(
              inputDefinitions.DEFINITION_RADIO_SELECTED_DME2_IDENT_ENABLE);
          break;
        }
        case 141: {
          sendBasicCommand(
              inputDefinitions.DEFINITION_RADIO_SELECTED_DME2_IDENT_DISABLE);

          break;
        }
        case 142: {
          sendBasicCommand(
              inputDefinitions.DEFINITION_RADIO_SELECTED_DME2_IDENT_SET);

          break;
        }
        case 143: {
          sendBasicCommand(
              inputDefinitions.DEFINITION_RADIO_SELECTED_DME2_IDENT_TOGGLE);

          break;
        }
        case 144: {
          sendBasicCommand(inputDefinitions.DEFINITION_TOGGLE_DME);
          break;
        }

        // VOR1
        case 145: {
          sendBasicCommand(
              inputDefinitions.DEFINITION_RADIO_SELECTED_VOR1_IDENT_ENABLE);
          break;
        }
        case 146: {
          sendBasicCommand(
              inputDefinitions.DEFINITION_RADIO_SELECTED_VOR1_IDENT_DISABLE);
          break;
        }
        //!
        case 147: {
          sendBasicCommand(
              inputDefinitions.DEFINITION_RADIO_SELECTED_VOR1_IDENT_SET);
          break;
        }
        case 148: {
          sendBasicCommand(
              inputDefinitions.DEFINITION_RADIO_SELECTED_VORE_IDENT_TOGGLE);
          break;
        }
        case 149: {
          sendBasicCommand(
              inputDefinitions.DEFINITION_RADIO_SELECTED_VOR2_IDENT_ENABLE);
          break;
        }
        case 150: {
          sendBasicCommand(
              inputDefinitions.DEFINITION_RADIO_SELECTED_VOR2_IDENT_DISABLE);
          break;
        }
        case 151: {
          // TO DO SET FUNCTION
          sendBasicCommand(
              inputDefinitions.DEFINITION_RADIO_SELECTED_VOR2_IDENT_SET);
          break;
        }
        case 152: {
          sendBasicCommand(
              inputDefinitions.DEFINITION_RADIO_SELECTED_VOR2_IDENT_TOGGLE);
          break;
        }
        case 153: {
          sendBasicCommand(inputDefinitions.DEFINITION_TOGGLE_VOR);
          break;
        }
        case 154: {
          sendBasicCommand(inputDefinitions.DEFINITION_VOR1_OBI_DEC);
          break;
        }
        case 155: {
          sendBasicCommand(inputDefinitions.DEFINITION_VOR1_OBI_INC);
          break;
        }
        case 156: {
          sendBasicCommand(inputDefinitions.DEFINITION_VOR2_OBI_DEC);
          break;
        }
        case 157: {
          sendBasicCommand(inputDefinitions.DEFINITION_VOR2_OBI_INC);
          break;
        }

        // ADF
        case 158: {
          sendBasicCommand(
              inputDefinitions.DEFINITION_RADIO_SELECTED_ADF_IDENT_ENABLE);
          break;
        }
        case 159: {
          sendBasicCommand(
              inputDefinitions.DEFINITION_RADIO_SELECTED_ADF_IDENT_DISABLE);
          break;
        }
        case 160: {
          // TO DO SET FUNCTION
          sendBasicCommand(
              inputDefinitions.DEFINITION_RADIO_SELECTED_ADF_IDENT_SET);
          break;
        }
        case 161: {
          sendBasicCommand(
              inputDefinitions.DEFINITION_RADIO_SELECTED_ADF_IDENT_TOGGLE);
          break;
        }
        case 162: {
          sendBasicCommand(inputDefinitions.DEFINITION_ADF_CARD_INC);
          break;
        }
        case 163: {
          sendBasicCommand(inputDefinitions.DEFINITION_ADF_CARD_DEC);
          break;
        }
        case 164: {
          // TO DO SET FUNCTION
          sendBasicCommand(inputDefinitions.DEFINITION_ADF_CARD_SET);
          break;
        }
        case 165: {
          sendBasicCommand(inputDefinitions.DEFINITION_ADF_100_INC);
          break;
        }
        case 166: {
          sendBasicCommand(inputDefinitions.DEFINITION_ADF_10_INC);
          break;
        }
        case 167: {
          sendBasicCommand(inputDefinitions.DEFINITION_ADF_1_INC);
          break;
        }
        case 168: {
          sendBasicCommand(inputDefinitions.DEFINITION_ADF_100_DEC);
          break;
        }
        case 169: {
          sendBasicCommand(inputDefinitions.DEFINITION_ADF_10_DEC);
          break;
        }
        case 170: {
          sendBasicCommand(inputDefinitions.DEFINITION_ADF_1_DEC);
          break;
        }

        // XPNDR
        case 171: {
          sendBasicCommand(inputDefinitions.DEFINITION_XPNDR_1000_INC);
          break;
        }
        case 172: {
          sendBasicCommand(inputDefinitions.DEFINITION_XPNDR_100_INC);
          break;
        }
        case 173: {
          sendBasicCommand(inputDefinitions.DEFINITION_XPNDR_10_INC);
          break;
        }
        case 174: {
          sendBasicCommand(inputDefinitions.DEFINITION_XPNDR_1_INC);
          break;
        }
        case 175: {
          sendBasicCommand(inputDefinitions.DEFINITION_XPNDR_1000_DEC);
          break;
        }
        case 176: {
          sendBasicCommand(inputDefinitions.DEFINITION_XPNDR_100_DEC);
          break;
        }
        case 177: {
          sendBasicCommand(inputDefinitions.DEFINITION_XPNDR_10_DEC);
          break;
        }
        case 178: {
          sendBasicCommand(inputDefinitions.DEFINITION_XPNDR_1_DEC);
          break;
        }
        case 202: {
          std::string value =
              std::string(&receivedString[index][4], &receivedString[index][8]);
          SimConnect_TransmitClientEvent(
              connect, 0, inputDefinitions.DEFINITION_XPNDR_SET,
              Dec2Bcd(stoi(value)), SIMCONNECT_GROUP_PRIORITY_HIGHEST,
              SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
          break;
        }
        case 179: {
          // TO DO SET FUNCTION
          sendBasicCommand(inputDefinitions.DEFINITION_ADF_COMPLETE_SET);
          break;
        }
        case 180: {
          sendBasicCommand(inputDefinitions.DEFINITION_ADF1_WHOLE_INC);
          break;
        }
        case 181: {
          sendBasicCommand(inputDefinitions.DEFINITION_ADF1_WHOLE_DEC);
          break;
        }
        case 182: {
          sendBasicCommand(inputDefinitions.DEFINITION_ADF2_100_INC);
          break;
        }
        case 183: {
          sendBasicCommand(inputDefinitions.DEFINITION_ADF2_10_INC);
          break;
        }
        case 184: {
          sendBasicCommand(inputDefinitions.DEFINITION_ADF2_1_INC);
          break;
        }
        case 185: {
          sendBasicCommand(inputDefinitions.DEFINITION_ADF2_RADIO_TENTHS_INC);
          break;
        }
        case 186: {
          sendBasicCommand(inputDefinitions.DEFINITION_ADF2_100_DEC);
          break;
        }
        case 187: {
          sendBasicCommand(inputDefinitions.DEFINITION_ADF2_10_DEC);
          break;
        }
        case 188: {
          sendBasicCommand(inputDefinitions.DEFINITION_ADF2_1_DEC);
          break;
        }
        case 189: {
          sendBasicCommand(inputDefinitions.DEFINITION_ADF2_RADIO_TENTHS_DEC);
          break;
        }
        case 190: {
          sendBasicCommand(inputDefinitions.DEFINITION_ADF2_WHOLE_INC);
          break;
        }
        case 191: {
          sendBasicCommand(inputDefinitions.DEFINITION_ADF2_WHOLE_DEC);
          break;
        }
        case 192: {
          sendBasicCommand(inputDefinitions.DEFINITION_ADF2_FRACT_DEC_CARRY);
          break;
        }
        case 193: {
          sendBasicCommand(inputDefinitions.DEFINITION_ADF2_FRACT_INC_CARRY);
          break;
        }
        case 194: {
          // TO DO SET FUNCTION
          sendBasicCommand(inputDefinitions.DEFINITION_ADF2_COMPLETE_SET);
          break;
        }
        case 195: {
          sendBasicCommand(
              inputDefinitions.DEFINITION_RADIO_ADF2_IDENT_DISABLE);
          break;
        }
        case 196: {
          sendBasicCommand(inputDefinitions.DEFINITION_RADIO_ADF2_IDENT_ENABLE);
          break;
        }
        case 197: {
          sendBasicCommand(inputDefinitions.DEFINITION_RADIO_ADF2_IDENT_TOGGLE);
          break;
        }
        case 200: {
          // TO DO SET FUNCTION
          sendBasicCommand(inputDefinitions.DEFINITION_RADIO_ADF2_IDENT_SET);
          break;
        }

        case 250: {
          sendBasicCommand(
              inputDefinitions.DEFINITION_G1000_PFD_ZOOMOUT_BUTTON);
          break;
        }

        case 251: {
          sendBasicCommand(inputDefinitions.DEFINITION_G1000_PFD_ZOOMIN_BUTTON);
          break;
        }

        // AP
        case 301: {
          sendBasicCommand(inputDefinitions.DEFINITION_AP_MASTER);
          break;
        }
        case 302: {
          sendBasicCommand(inputDefinitions.DEFINITION_AP_PANEL_HEADING_HOLD);
          break;
        }
        case 303: {
          sendBasicCommand(inputDefinitions.DEFINITION_AP_PANEL_ALTITUDE_HOLD);
          break;
        }
        case 304: {
          sendBasicCommand(inputDefinitions.DEFINITION_AP_ATT_HOLD_ON);
          break;
        }
        case 305: {
          sendBasicCommand(inputDefinitions.DEFINITION_AP_LOC_HOLD_ON);
          break;
        }
        case 306: {
          sendBasicCommand(inputDefinitions.DEFINITION_AP_APR_HOLD_ON);
          break;
        }
        case 307: {
          sendBasicCommand(inputDefinitions.DEFINITION_AP_HDG_HOLD_ON);
          break;
        }
        case 308: {
          sendBasicCommand(inputDefinitions.DEFINITION_AP_ALT_HOLD_ON);
          break;
        }
        case 309: {
          sendBasicCommand(inputDefinitions.DEFINITION_AP_WING_LEVELER_ON);
          break;
        }
        case 310: {
          sendBasicCommand(inputDefinitions.DEFINITION_AP_BC_HOLD_ON);
          break;
        }
        case 311: {
          sendBasicCommand(inputDefinitions.DEFINITION_AP_NAV1_HOLD_ON);
          break;
        }
        case 312: {
          sendBasicCommand(inputDefinitions.DEFINITION_AP_ATT_HOLD_OFF);
          break;
        }
        case 313: {
          sendBasicCommand(inputDefinitions.DEFINITION_AP_LOC_HOLD_OFF);
          break;
        }
        case 314: {
          sendBasicCommand(inputDefinitions.DEFINITION_AP_APR_HOLD_OFF);
          break;
        }
        case 315: {
          sendBasicCommand(inputDefinitions.DEFINITION_AP_HDG_HOLD_OFF);
          break;
        }
        case 316: {
          sendBasicCommand(inputDefinitions.DEFINITION_AP_ALT_HOLD_OFF);
          break;
        }
        case 317: {
          sendBasicCommand(inputDefinitions.DEFINITION_AP_WING_LEVELER_OFF);
          break;
        }
        case 318: {
          sendBasicCommand(inputDefinitions.DEFINITION_AP_BC_HOLD_OFF);
          break;
        }
        case 319: {
          sendBasicCommand(inputDefinitions.DEFINITION_AP_NAV1_HOLD_OFF);
          break;
        }
        case 320: {
          sendBasicCommand(inputDefinitions.DEFINITION_AP_AIRSPEED_HOLD);
          break;
        }
        case 321: {
          sendBasicCommand(inputDefinitions.DEFINITION_AP_PANEL_SPEED_HOLD);
          break;
        }
        case 322: {
          sendBasicCommand(inputDefinitions.DEFINITION_AP_ALT_VAR_INC);
          break;
        }
        case 323: {
          sendBasicCommand(inputDefinitions.DEFINITION_AP_ALT_VAR_DEC);
          break;
        }
        case 324: {
          sendBasicCommand(inputDefinitions.DEFINITION_AP_VS_VAR_INC);
          break;
        }
        case 325: {
          sendBasicCommand(inputDefinitions.DEFINITION_AP_VS_VAR_DEC);
          break;
        }
        case 326: {
          sendBasicCommand(inputDefinitions.DEFINITION_AP_SPD_VAR_INC);
          break;
        }
        case 327: {
          sendBasicCommand(inputDefinitions.DEFINITION_AP_SPD_VAR_DEC);
          break;
        }
        case 328: {
          sendBasicCommand(inputDefinitions.DEFINITION_AP_PANEL_MACH_HOLD);
          break;
        }
        case 329: {
          sendBasicCommand(inputDefinitions.DEFINITION_AP_MACH_VAR_INC);
          break;
        }
        case 330: {
          sendBasicCommand(inputDefinitions.DEFINITION_AP_MACH_VAR_DEC);
          break;
        }
        case 331: {
          sendBasicCommand(inputDefinitions.DEFINITION_AP_MACH_HOLD);
          break;
        }
        case 332: {
          // TO DO SET FUNCTION
          sendBasicCommand(inputDefinitions.DEFINITION_AP_ALT_VAR_SET_METRIC);
          break;
        }
          // TO DO SET FUNCTION
        case 333: {
          sendBasicCommand(inputDefinitions.DEFINITION_AP_VS_VAR_SET_ENGLISH);
          break;
        }
        case 334: {
          // TO DO SET FUNCTION
          sendBasicCommand(inputDefinitions.DEFINITION_AP_SPD_VAR_SET);
          break;
        }
        case 335: {
          // TO DO SET FUNCTION
          sendBasicCommand(inputDefinitions.DEFINITION_AP_MACH_VAR_SET);
          break;
        }
        case 336: {
          sendBasicCommand(inputDefinitions.DEFINITION_AP_AIRSPEED_ON);
          break;
        }
        case 337: {
          sendBasicCommand(inputDefinitions.DEFINITION_AP_AIRSPEED_OFF);
          break;
        }
        case 338: {
          // TO DO SET FUNCTION
          sendBasicCommand(inputDefinitions.DEFINITION_AP_AIRSPEED_SET);
          break;
        }
        case 339: {
          sendBasicCommand(inputDefinitions.DEFINITION_AP_MACH_ON);
          break;
        }
        case 340: {
          sendBasicCommand(inputDefinitions.DEFINITION_AP_MACH_OFF);
          break;
        }
        case 341: {
          // TO DO SET FUNCTION
          sendBasicCommand(inputDefinitions.DEFINITION_AP_MACH_SET);
          break;
        }
        case 342: {
          sendBasicCommand(inputDefinitions.DEFINITION_AP_PANEL_ALTITUDE_ON);
          break;
        }
        case 343: {
          sendBasicCommand(inputDefinitions.DEFINITION_AP_PANEL_ALTITUDE_OFF);
          break;
        }
        case 345: {
          // TO DO SET FUNCTION
          sendBasicCommand(inputDefinitions.DEFINITION_AP_PANEL_ALTITUDE_SET);
          break;
        }
        case 346: {
          sendBasicCommand(inputDefinitions.DEFINITION_AP_PANEL_HEADING_ON);
          break;
        }
        case 347: {
          sendBasicCommand(inputDefinitions.DEFINITION_AP_PANEL_HEADING_OFF);
          break;
        }
        case 348: {
          // TO DO SET FUNCTION
          sendBasicCommand(inputDefinitions.DEFINITION_AP_PANEL_HEADING_SET);
          break;
        }
        case 349: {
          sendBasicCommand(inputDefinitions.DEFINITION_AP_PANEL_MACH_ON);
          break;
        }
        case 350: {
          sendBasicCommand(inputDefinitions.DEFINITION_AP_PANEL_MACH_OFF);
          break;
        }
        case 351: {
          // TO DO SET FUNCTION
          sendBasicCommand(inputDefinitions.DEFINITION_AP_PANEL_MACH_SET);
          break;
        }
        case 352: {
          sendBasicCommand(inputDefinitions.DEFINITION_AP_PANEL_SPEED_ON);
          break;
        }
        case 353: {
          sendBasicCommand(inputDefinitions.DEFINITION_AP_PANEL_SPEED_OFF);
          break;
        }
        case 354: {
          // TO DO SET FUNCTION
          sendBasicCommand(inputDefinitions.DEFINITION_AP_PANEL_SPEED_SET);
          break;
        }
        case 355: {
          // TO DO SET FUNCTION
          sendBasicCommand(inputDefinitions.DEFINITION_AP_ALT_VAR_SET_ENGLISH);
          break;
        }
        case 356: {
          // TO DO SET FUNCTION
          sendBasicCommand(inputDefinitions.DEFINITION_AP_VS_VAR_SET_METRIC);
          break;
        }
        case 357: {
          sendBasicCommand(
              inputDefinitions.DEFINITION_AP_PANEL_SPEED_HOLD_TOGGLE);
          break;
        }
        case 358: {
          sendBasicCommand(
              inputDefinitions.DEFINITION_AP_PANEL_MACH_HOLD_TOGGLE);
          break;
        }
        case 359: {
          // TO DO SET FUNCTION
          sendBasicCommand(inputDefinitions.DEFINITION_AP_NAV_SELECT_SET);
          break;
        }
        case 360: {
          sendBasicCommand(inputDefinitions.DEFINITION_AP_PITCH_REF_INC_UP);
          break;
        }
        case 361: {
          sendBasicCommand(inputDefinitions.DEFINITION_AP_PITCH_REF_INC_DN);
          break;
        }
        case 362: {
          sendBasicCommand(inputDefinitions.DEFINITION_AP_PITCH_REF_SELECT);
          break;
        }
        case 363: {
          sendBasicCommand(inputDefinitions.DEFINITION_AP_ATT_HOLD);
          break;
        }
        case 364: {
          sendBasicCommand(inputDefinitions.DEFINITION_AP_LOC_HOLD);
          break;
        }
        case 365: {
          sendBasicCommand(inputDefinitions.DEFINITION_AP_APR_HOLD);
          break;
        }
        case 366: {
          sendBasicCommand(inputDefinitions.DEFINITION_AP_HDG_HOLD);
          break;
        }
        case 367: {
          sendBasicCommand(inputDefinitions.DEFINITION_AP_ALT_HOLD);
          break;
        }
        case 368: {
          sendBasicCommand(inputDefinitions.DEFINITION_AP_WING_LEVELER);
          break;
        }
        case 369: {
          sendBasicCommand(inputDefinitions.DEFINITION_AP_BC_HOLD);
          break;
        }
        case 370: {
          sendBasicCommand(inputDefinitions.DEFINITION_AP_NAV1_HOLD);
          break;
        }
        case 371: {
          sendBasicCommand(inputDefinitions.DEFINITION_HEADING_BUG_INC);
          break;
        }
        case 372: {
          sendBasicCommand(inputDefinitions.DEFINITION_HEADING_BUG_DEC);
          break;
        }
        case 373: {
          sendBasicCommand(inputDefinitions.DEFINITION_TOGGLE_FLIGHT_DIRECTOR);
          break;
        }
        case 374: {
          sendBasicCommand(inputDefinitions.DEFINITION_AP_VS_HOLD);
          break;
        }
        case 375: {
          sendBasicCommand(inputDefinitions.DEFINITION_KOHLSMAN_INC);
          break;
        }
        case 376: {
          sendBasicCommand(inputDefinitions.DEFINITION_KOHLSMAN_DEC);
          break;
        }
        case 378: {
          sendBasicCommand(inputDefinitions.DEFINITION_AUTO_THROTTLE_ARM);
          break;
        }
        case 379: {
          sendBasicCommand(inputDefinitions.DEFINITION_AUTO_THROTTLE_TO_GA);
          break;
        }
        // Avionics
        case 401: {
          // TO DO CHECK IF WORKS
          sendBasicCommandOff(
              inputDefinitions.DEFINITION_TOGGLE_AVIONICS1_MASTER_OFF);
          break;
        }
        case 402: {
          sendBasicCommandOn(
              inputDefinitions.DEFINITION_TOGGLE_AVIONICS1_MASTER_ON);
          break;
        }
        case 403: {
          sendBasicCommandOff(
              inputDefinitions.DEFINITION_TOGGLE_AVIONICS2_MASTER_OFF);
          break;
        }
        case 404: {
          sendBasicCommandOn(
              inputDefinitions.DEFINITION_TOGGLE_AVIONICS2_MASTER_ON);
          break;
        }
        // TO DO FUNCTION ON OFF Battery
        case 405: {
          SimConnect_TransmitClientEvent(
              connect, 0, inputDefinitions.DEFINITION_TOGGLE_MASTER_BATTERY, 1,
              SIMCONNECT_GROUP_PRIORITY_HIGHEST,
              SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
          break;
        }
        case 406: {
          SimConnect_TransmitClientEvent(
              connect, 0, inputDefinitions.DEFINITION_TOGGLE_MASTER_BATTERY, 2,
              SIMCONNECT_GROUP_PRIORITY_HIGHEST,
              SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
          break;
        }
        case 407: {
          sendBasicCommand(
              inputDefinitions.DEFINITION_TOGGLE_MASTER_ALTERNATOR);
          break;
        }
        case 408: {
          sendBasicCommand(inputDefinitions.DEFINITION_TOGGLE_EXTERNAL_POWER);
          break;
        }
        case 420: {
          sendBasicCommand(inputDefinitions.DEFINITION_PARKING_BRAKE);
          break;
        }
        case 501: {
          sendBasicCommand(inputDefinitions.DEFINITION_FLAPS_UP);
          break;
        }
        case 502: {
          sendBasicCommand(inputDefinitions.DEFINITION_FLAPS_1);
          break;
        }
        case 503: {
          sendBasicCommand(inputDefinitions.DEFINITION_FLAPS_2);
          break;
        }
        case 504: {
          sendBasicCommand(inputDefinitions.DEFINITION_FLAPS_3);
          break;
        }
        case 505: {
          sendBasicCommand(inputDefinitions.DEFINITION_FLAPS_DOWN);
          break;
        }
        case 506: {
          sendBasicCommand(inputDefinitions.DEFINITION_FLAPS_INCR);
          break;
        }
        case 507: {
          sendBasicCommand(inputDefinitions.DEFINITION_FLAPS_DECR);
          break;
        }
        case 508: {
          sendBasicCommand(inputDefinitions.DEFINITION_PITOT_HEAT_TOGGLE);
          break;
        }
        case 509: {
          sendBasicCommand(inputDefinitions.DEFINITION_PITOT_HEAT_ON);
          break;
        }
        case 510: {
          sendBasicCommand(inputDefinitions.DEFINITION_PITOT_HEAT_OFF);
          break;
        }
        case 511: {
          sendBasicCommand(inputDefinitions.DEFINITION_MAGNETO);
          break;
        }
        case 512: {
          sendBasicCommand(inputDefinitions.DEFINITION_MAGNETO_DECR);
          break;
        }
        case 513: {
          sendBasicCommand(inputDefinitions.DEFINITION_MAGNETO_INCR);
          break;
        }
        case 514: {
          sendBasicCommand(inputDefinitions.DEFINITION_MAGNETO1_OFF);
          break;
        }
        case 515: {
          sendBasicCommand(inputDefinitions.DEFINITION_MAGNETO1_RIGHT);
          break;
        }
        case 516: {
          sendBasicCommand(inputDefinitions.DEFINITION_MAGNETO1_LEFT);
          break;
        }
        case 517: {
          sendBasicCommand(inputDefinitions.DEFINITION_MAGNETO1_BOTH);
          break;
        }
        case 518: {
          sendBasicCommand(inputDefinitions.DEFINITION_MAGNETO1_START);
          break;
        }
        case 519: {
          sendBasicCommand(inputDefinitions.DEFINITION_MAGNETO2_OFF);
          break;
        }
        case 520: {
          sendBasicCommand(inputDefinitions.DEFINITION_MAGNETO2_RIGHT);
          break;
        }
        case 521: {
          sendBasicCommand(inputDefinitions.DEFINITION_MAGNETO2_LEFT);
          break;
        }
        case 522: {
          sendBasicCommand(inputDefinitions.DEFINITION_MAGNETO2_BOTH);
          break;
        }
        case 523: {
          sendBasicCommand(inputDefinitions.DEFINITION_MAGNETO2_START);
          break;
        }
        case 524: {
          sendBasicCommand(inputDefinitions.DEFINITION_MAGNETO3_OFF);
          break;
        }
        case 525: {
          sendBasicCommand(inputDefinitions.DEFINITION_MAGNETO3_RIGHT);
          break;
        }
        case 526: {
          sendBasicCommand(inputDefinitions.DEFINITION_MAGNETO3_LEFT);
          break;
        }
        case 527: {
          sendBasicCommand(inputDefinitions.DEFINITION_MAGNETO3_BOTH);
          break;
        }
        case 528: {
          sendBasicCommand(inputDefinitions.DEFINITION_MAGNETO3_START);
          break;
        }
        case 529: {
          sendBasicCommand(inputDefinitions.DEFINITION_MAGNETO4_OFF);
          break;
        }
        case 530: {
          sendBasicCommand(inputDefinitions.DEFINITION_MAGNETO4_RIGHT);
          break;
        }
        case 531: {
          sendBasicCommand(inputDefinitions.DEFINITION_MAGNETO4_LEFT);
          break;
        }
        case 532: {
          sendBasicCommand(inputDefinitions.DEFINITION_MAGNETO4_BOTH);
          break;
        }
        case 533: {
          sendBasicCommand(inputDefinitions.DEFINITION_MAGNETO4_START);
          break;
        }
        case 534: {
          sendBasicCommand(inputDefinitions.DEFINITION_MAGNETO_OFF);
          break;
        }
        case 535: {
          sendBasicCommand(inputDefinitions.DEFINITION_MAGNETO_RIGHT);
          break;
        }
        case 536: {
          sendBasicCommand(inputDefinitions.DEFINITION_MAGNETO_LEFT);
          break;
        }
        case 537: {
          sendBasicCommand(inputDefinitions.DEFINITION_MAGNETO_BOTH);
          break;
        }
        case 538: {
          sendBasicCommand(inputDefinitions.DEFINITION_MAGNETO_START);
          break;
        }
        case 539: {
          sendBasicCommand(inputDefinitions.DEFINITION_MAGNETO1_DECR);
          break;
        }
        case 540: {
          sendBasicCommand(inputDefinitions.DEFINITION_MAGNETO1_INCR);
          break;
        }
        case 541: {
          sendBasicCommand(inputDefinitions.DEFINITION_MAGNETO2_DECR);
          break;
        }
        case 542: {
          sendBasicCommand(inputDefinitions.DEFINITION_MAGNETO2_INCR);
          break;
        }
        case 543: {
          sendBasicCommand(inputDefinitions.DEFINITION_MAGNETO3_DECR);
          break;
        }
        case 544: {
          sendBasicCommand(inputDefinitions.DEFINITION_MAGNETO3_INCR);
          break;
        }
        case 545: {
          sendBasicCommand(inputDefinitions.DEFINITION_MAGNETO4_DECR);
          break;
        }
        case 546: {
          sendBasicCommand(inputDefinitions.DEFINITION_MAGNETO4_INCR);
          break;
        }
        case 547: {
          // TO DO SET FUNCTION
          sendBasicCommand(inputDefinitions.DEFINITION_MAGNETO_SET);
          break;
        }
        case 548: {
          // TO DO SET FUNCTION
          sendBasicCommand(inputDefinitions.DEFINITION_MAGNETO1_SET);
          break;
        }
        case 549: {
          // TO DO SET FUNCTION
          sendBasicCommand(inputDefinitions.DEFINITION_MAGNETO2_SET);
          break;
        }
        case 550: {
          // TO DO SET FUNCTION
          sendBasicCommand(inputDefinitions.DEFINITION_MAGNETO3_SET);
          break;
        }
        case 551: {
          // TO DO SET FUNCTION
          sendBasicCommand(inputDefinitions.DEFINITION_MAGNETO4_SET);
          break;
        }
        case 552: {
          sendBasicCommand(inputDefinitions.DEFINITION_ANTI_ICE_ON);
          break;
        }
        case 553: {
          sendBasicCommand(inputDefinitions.DEFINITION_ANTI_ICE_OFF);
          break;
        }
        case 554: {
          // TO DO SET FUNCTION
          sendBasicCommand(inputDefinitions.DEFINITION_ANTI_ICE_SET);
          break;
        }
        case 555: {
          sendBasicCommand(inputDefinitions.DEFINITION_ANTI_ICE_TOGGLE);
          break;
        }
        case 556: {
          sendBasicCommand(inputDefinitions.DEFINITION_ANTI_ICE_TOGGLE_ENG1);
          break;
        }
        case 557: {
          sendBasicCommand(inputDefinitions.DEFINITION_ANTI_ICE_TOGGLE_ENG2);
          break;
        }
        case 558: {
          sendBasicCommand(inputDefinitions.DEFINITION_ANTI_ICE_TOGGLE_ENG3);
          break;
        }
        case 559: {
          sendBasicCommand(inputDefinitions.DEFINITION_ANTI_ICE_TOGGLE_ENG4);
          break;
        }
        case 560: {
          // TO DO SET FUNCTION
          sendBasicCommand(inputDefinitions.DEFINITION_ANTI_ICE_SET_ENG1);
          break;
        }
        case 561: {
          // TO DO SET FUNCTION
          sendBasicCommand(inputDefinitions.DEFINITION_ANTI_ICE_SET_ENG2);
          break;
        }
        case 562: {
          // TO DO SET FUNCTION
          sendBasicCommand(inputDefinitions.DEFINITION_ANTI_ICE_SET_ENG3);
          break;
        }
        case 563: {
          // TO DO SET FUNCTION
          sendBasicCommand(inputDefinitions.DEFINITION_ANTI_ICE_SET_ENG4);
          break;
        }
        case 564: {
          sendBasicCommand(inputDefinitions.DEFINITION_TOGGLE_FUEL_VALVE_ALL);
          break;
        }
        case 565: {
          sendBasicCommand(inputDefinitions.DEFINITION_TOGGLE_FUEL_VALVE_ENG1);
          break;
        }
        case 566: {
          sendBasicCommand(inputDefinitions.DEFINITION_TOGGLE_FUEL_VALVE_ENG2);
          break;
        }
        case 567: {
          sendBasicCommand(inputDefinitions.DEFINITION_TOGGLE_FUEL_VALVE_ENG3);
          break;
        }
        case 568: {
          sendBasicCommand(inputDefinitions.DEFINITION_TOGGLE_FUEL_VALVE_ENG4);
          break;
        }
        case 569: {
          sendBasicCommand(inputDefinitions.DEFINITION_FUEL_SELECTOR_OFF);
          break;
        }
        case 570: {
          sendBasicCommand(inputDefinitions.DEFINITION_FUEL_SELECTOR_ALL);
          break;
        }
        case 571: {
          sendBasicCommand(inputDefinitions.DEFINITION_FUEL_SELECTOR_LEFT);
          break;
        }
        case 572: {
          sendBasicCommand(inputDefinitions.DEFINITION_FUEL_SELECTOR_RIGHT);
          break;
        }
        case 573: {
          sendBasicCommand(inputDefinitions.DEFINITION_FUEL_SELECTOR_LEFT_AUX);
          break;
        }
        case 574: {
          sendBasicCommand(inputDefinitions.DEFINITION_FUEL_SELECTOR_RIGHT_AUX);
          break;
        }
        case 575: {
          sendBasicCommand(inputDefinitions.DEFINITION_FUEL_SELECTOR_CENTER);
          break;
        }
        case 576: {
          // TO DO SET FUEL SELECTOR
          sendBasicCommand(inputDefinitions.DEFINITION_FUEL_SELECTOR_SET);
          break;
        }
        case 577: {
          sendBasicCommand(inputDefinitions.DEFINITION_FUEL_SELECTOR_2_OFF);
          break;
        }
        case 578: {
          sendBasicCommand(inputDefinitions.DEFINITION_FUEL_SELECTOR_2_ALL);
          break;
        }
        case 579: {
          sendBasicCommand(inputDefinitions.DEFINITION_FUEL_SELECTOR_2_LEFT);
          break;
        }
        case 580: {
          sendBasicCommand(inputDefinitions.DEFINITION_FUEL_SELECTOR_2_RIGHT);
          break;
        }
        case 581: {
          sendBasicCommand(
              inputDefinitions.DEFINITION_FUEL_SELECTOR_2_LEFT_AUX);
          break;
        }
        case 582: {
          sendBasicCommand(
              inputDefinitions.DEFINITION_FUEL_SELECTOR_2_RIGHT_AUX);
          break;
        }
        case 583: {
          sendBasicCommand(inputDefinitions.DEFINITION_FUEL_SELECTOR_2_CENTER);
          break;
        }
          // TO DO SET FUNCTION
        case 584: {
          sendBasicCommand(inputDefinitions.DEFINITION_FUEL_SELECTOR_2_SET);
          break;
        }
        case 585: {
          sendBasicCommand(inputDefinitions.DEFINITION_FUEL_SELECTOR_3_OFF);
          break;
        }
        case 586: {
          sendBasicCommand(inputDefinitions.DEFINITION_FUEL_SELECTOR_3_ALL);
          break;
        }
        case 587: {
          sendBasicCommand(inputDefinitions.DEFINITION_FUEL_SELECTOR_3_LEFT);
          break;
        }
        case 588: {
          sendBasicCommand(inputDefinitions.DEFINITION_FUEL_SELECTOR_3_RIGHT);
          break;
        }
        case 589: {
          sendBasicCommand(
              inputDefinitions.DEFINITION_FUEL_SELECTOR_3_LEFT_AUX);
          break;
        }
        case 590: {
          sendBasicCommand(
              inputDefinitions.DEFINITION_FUEL_SELECTOR_3_RIGHT_AUX);
          break;
        }
        case 591: {
          sendBasicCommand(inputDefinitions.DEFINITION_FUEL_SELECTOR_3_CENTER);
          break;
        }
        case 592: {
          // TO DO SET FUNCTION
          sendBasicCommand(inputDefinitions.DEFINITION_FUEL_SELECTOR_3_SET);
          break;
        }
        case 593: {
          sendBasicCommand(inputDefinitions.DEFINITION_FUEL_SELECTOR_4_OFF);
          break;
        }
        case 594: {
          sendBasicCommand(inputDefinitions.DEFINITION_FUEL_SELECTOR_4_ALL);
          break;
        }
        case 595: {
          sendBasicCommand(inputDefinitions.DEFINITION_FUEL_SELECTOR_4_LEFT);
          break;
        }
        case 596: {
          sendBasicCommand(inputDefinitions.DEFINITION_FUEL_SELECTOR_4_RIGHT);
          break;
        }
        case 597: {
          sendBasicCommand(
              inputDefinitions.DEFINITION_FUEL_SELECTOR_4_LEFT_AUX);
          break;
        }
        case 598: {
          sendBasicCommand(
              inputDefinitions.DEFINITION_FUEL_SELECTOR_4_RIGHT_AUX);
          break;
        }
        case 599: {
          sendBasicCommand(inputDefinitions.DEFINITION_FUEL_SELECTOR_4_CENTER);
          break;
        }
        case 600: {
          // TO DO SET FUNCTION
          sendBasicCommand(inputDefinitions.DEFINITION_FUEL_SELECTOR_4_SET);
          break;
        }
        case 601: {
          sendBasicCommand(inputDefinitions.DEFINITION_CROSS_FEED_OPEN);
          break;
        }
        case 602: {
          sendBasicCommand(inputDefinitions.DEFINITION_CROSS_FEED_TOGGLE);
          break;
        }
        case 603: {
          sendBasicCommand(inputDefinitions.DEFINITION_CROSS_FEED_OFF);
          break;
        }

        case 604: {
          sendBasicCommand(inputDefinitions.DEFINITION_ELEV_TRIM_DN);
          break;
        }
        case 605: {
          sendBasicCommand(inputDefinitions.DEFINITION_DOWN);
          break;
        }
        case 606: {
          sendBasicCommand(inputDefinitions.DEFINITION_ELEV_TRIM_UP);
          break;
        }
        case 607: {
          sendBasicCommand(inputDefinitions.DEFINITION_ELEV_UP);
          break;
        }
        case 608: {
          sendBasicCommand(inputDefinitions.DEFINITION_SPOILERS_ARM_TOGGLE);
          break;
        }
        case 609: {
          sendBasicCommand(inputDefinitions.DEFINITION_SPOILERS_ON);
          break;
        }
        case 610: {
          sendBasicCommand(inputDefinitions.DEFINITION_SPOILERS_OFF);
          break;
        }
        case 611: {
          sendBasicCommand(inputDefinitions.DEFINITION_SPOILERS_ARM_ON);
          break;
        }
        case 612: {
          sendBasicCommand(inputDefinitions.DEFINITION_SPOILERS_ARM_OFF);
          break;
        }
        case 613: {
          sendBasicCommand(inputDefinitions.DEFINITION_AILERON_TRIM_LEFT);
          break;
        }
        case 614: {
          sendBasicCommand(inputDefinitions.DEFINITION_AILERON_TRIM_RIGHT);
          break;
        }
        case 615: {
          sendBasicCommand(inputDefinitions.DEFINITION_RUDDER_TRIM_LEFT);
          break;
        }
        case 616: {
          sendBasicCommand(inputDefinitions.DEFINITION_RUDDER_TRIM_RIGHT);
          break;
        }
        case 617: {
          // TO DO SET FUNCTION
          sendBasicCommand(inputDefinitions.DEFINITION_COWLFLAP1_SET);
          break;
        }
        case 618: {
          // TO DO SET FUNCTION
          sendBasicCommand(inputDefinitions.DEFINITION_COWLFLAP2_SET);
          break;
        }
        case 619: {
          // TO DO SET FUNCTION
          sendBasicCommand(inputDefinitions.DEFINITION_COWLFLAP3_SET);
          break;
        }
        case 620: {
          // TO DO SET FUNCTION
          sendBasicCommand(inputDefinitions.DEFINITION_COWLFLAP4_SET);
          break;
        }
        case 621: {
          sendBasicCommand(inputDefinitions.DEFINITION_INC_COWL_FLAPS);
          break;
        }
        case 622: {
          sendBasicCommand(inputDefinitions.DEFINITION_DEC_COWL_FLAPS);
          break;
        }
        case 623: {
          sendBasicCommand(inputDefinitions.DEFINITION_INC_COWL_FLAPS1);
          break;
        }
        case 624: {
          sendBasicCommand(inputDefinitions.DEFINITION_DEC_COWL_FLAPS1);
          break;
        }
        case 625: {
          sendBasicCommand(inputDefinitions.DEFINITION_INC_COWL_FLAPS2);
          break;
        }
        case 626: {
          sendBasicCommand(inputDefinitions.DEFINITION_DEC_COWL_FLAPS2);
          break;
        }
        case 627: {
          sendBasicCommand(inputDefinitions.DEFINITION_INC_COWL_FLAPS3);
          break;
        }
        case 628: {
          sendBasicCommand(inputDefinitions.DEFINITION_DEC_COWL_FLAPS3);
          break;
        }
        case 629: {
          sendBasicCommand(inputDefinitions.DEFINITION_INC_COWL_FLAPS4);
          break;
        }
        case 630: {
          sendBasicCommand(inputDefinitions.DEFINITION_DEC_COWL_FLAPS4);
          break;
        }
        case 631: {
          sendBasicCommand(inputDefinitions.DEFINITION_FUEL_PUMP);
          break;
        }
        case 632: {
          sendBasicCommand(inputDefinitions.DEFINITION_TOGGLE_ELECT_FUEL_PUMP);
          break;
        }
        case 633: {
          sendBasicCommand(inputDefinitions.DEFINITION_TOGGLE_ELECT_FUEL_PUMP1);
          break;
        }
        case 634: {
          sendBasicCommand(inputDefinitions.DEFINITION_TOGGLE_ELECT_FUEL_PUMP2);
          break;
        }
        case 635: {
          sendBasicCommand(inputDefinitions.DEFINITION_TOGGLE_ELECT_FUEL_PUMP3);
          break;
        }
        case 636: {
          sendBasicCommand(inputDefinitions.DEFINITION_TOGGLE_ELECT_FUEL_PUMP4);
          break;
        }
        case 637: {
          sendBasicCommand(inputDefinitions.DEFINITION_ENGINE_PRIMER);
          break;
        }
        case 638: {
          sendBasicCommand(inputDefinitions.DEFINITION_TOGGLE_PRIMER);
          break;
        }
        case 639: {
          sendBasicCommand(inputDefinitions.DEFINITION_TOGGLE_PRIMER1);
          break;
        }
        case 640: {
          sendBasicCommand(inputDefinitions.DEFINITION_TOGGLE_PRIMER2);
          break;
        }
        case 641: {
          sendBasicCommand(inputDefinitions.DEFINITION_TOGGLE_PRIMER3);
          break;
        }
        case 642: {
          sendBasicCommand(inputDefinitions.DEFINITION_TOGGLE_PRIMER4);
          break;
        }
        case 643: {
          sendBasicCommand(inputDefinitions.DEFINITION_TOGGLE_FEATHER_SWITCHES);
          break;
        }
        case 644: {
          sendBasicCommand(inputDefinitions.DEFINITION_TOGGLE_FEATHER_SWITCH_1);
          break;
        }
        case 645: {
          sendBasicCommand(inputDefinitions.DEFINITION_TOGGLE_FEATHER_SWITCH_2);
          break;
        }
        case 646: {
          sendBasicCommand(inputDefinitions.DEFINITION_TOGGLE_FEATHER_SWITCH_3);
          break;
        }
        case 647: {
          sendBasicCommand(inputDefinitions.DEFINITION_TOGGLE_FEATHER_SWITCH_4);
          break;
        }
        case 648: {
          sendBasicCommand(inputDefinitions.DEFINITION_TOGGLE_PROPELLER_SYNC);
          break;
        }
        case 649: {
          sendBasicCommand(inputDefinitions.DEFINITION_TOGGLE_AUTOFEATHER_ARM);
          break;
        }
        case 650: {
          sendBasicCommand(inputDefinitions.DEFINITION_TOGGLE_AFTERBURNER);
          break;
        }
        case 651: {
          sendBasicCommand(inputDefinitions.DEFINITION_TOGGLE_AFTERBURNER1);
          break;
        }
        case 652: {
          sendBasicCommand(inputDefinitions.DEFINITION_TOGGLE_AFTERBURNER2);
          break;
        }
        case 653: {
          sendBasicCommand(inputDefinitions.DEFINITION_TOGGLE_AFTERBURNER3);
          break;
        }
        case 654: {
          sendBasicCommand(inputDefinitions.DEFINITION_TOGGLE_AFTERBURNER4);
          break;
        }
        case 655: {
          sendBasicCommand(inputDefinitions.DEFINITION_ENGINE);
          break;
        }
        case 656: {
          sendBasicCommand(inputDefinitions.DEFINITION_SPOILERS_TOGGLE);
          break;
        }
        case 657: {
          sendBasicCommand(inputDefinitions.DEFINITION_TOGGLE_STARTER1);
          break;
        }
        case 658: {
          sendBasicCommand(inputDefinitions.DEFINITION_TOGGLE_STARTER2);
          break;
        }
        case 659: {
          sendBasicCommand(inputDefinitions.DEFINITION_TOGGLE_STARTER3);
          break;
        }
        case 660: {
          sendBasicCommand(inputDefinitions.DEFINITION_TOGGLE_STARTER4);
          break;
        }
        case 661: {
          sendBasicCommand(inputDefinitions.DEFINITION_TOGGLE_ALL_STARTERS);
          break;
        }
        case 662: {
          sendBasicCommand(inputDefinitions.DEFINITION_ENGINE_AUTO_START);
          break;
        }
        case 663: {
          sendBasicCommand(inputDefinitions.DEFINITION_ENGINE_AUTO_SHUTDOWN);
          break;
        }
        case 664: {
          sendBasicCommand(inputDefinitions.DEFINITION_FUEL_SELECTOR_LEFT_MAIN);
          break;
        }
        case 665: {
          sendBasicCommand(
              inputDefinitions.DEFINITION_FUEL_SELECTOR_2_LEFT_MAIN);
          break;
        }
        case 666: {
          sendBasicCommand(
              inputDefinitions.DEFINITION_FUEL_SELECTOR_3_LEFT_MAIN);
          break;
        }
        case 667: {
          sendBasicCommand(
              inputDefinitions.DEFINITION_FUEL_SELECTOR_4_LEFT_MAIN);
          break;
        }
        case 668: {
          sendBasicCommand(
              inputDefinitions.DEFINITION_FUEL_SELECTOR_RIGHT_MAIN);
          break;
        }
        case 669: {
          sendBasicCommand(
              inputDefinitions.DEFINITION_FUEL_SELECTOR_2_RIGHT_MAIN);
          break;
        }
        case 670: {
          sendBasicCommand(
              inputDefinitions.DEFINITION_FUEL_SELECTOR_3_RIGHT_MAIN);
          break;
        }
        case 671: {
          sendBasicCommand(
              inputDefinitions.DEFINITION_FUEL_SELECTOR_4_RIGHT_MAIN);
          break;
        }
        case 672: {
          sendBasicCommand(inputDefinitions.DEFINITION_APU_STARTER);
          break;
        }
        case 673: {
          sendBasicCommand(inputDefinitions.DEFINITION_APU_OFF_SWITCH);
          break;
        }
        case 674: {
          sendBasicCommand(
              inputDefinitions.DEFINITION_APU_GENERATOR_SWITCH_TOGGLE);
          break;
        }
        case 675: {
          sendBasicCommand(
              inputDefinitions.DEFINITION_CABIN_NO_SMOKING_ALERT_SWITCH_TOGGLE);
          break;
        }
        case 676: {
          sendBasicCommand(
              inputDefinitions.DEFINITION_CABIN_SEATBELTS_ALERT_SWITCH_TOGGLE);
          break;
        }
        case 677: {
          sendBasicCommand(inputDefinitions.DEFINITION_STROBES_TOGGLE);
          break;
        }
        case 678: {
          sendBasicCommand(inputDefinitions.DEFINITION_ALL_LIGHTS_TOGGLE);
          break;
        }
        case 679: {
          sendBasicCommand(inputDefinitions.DEFINITION_PANEL_LIGHTS_TOGGLE);
          break;
        }
        case 680: {
          sendBasicCommand(inputDefinitions.DEFINITION_LANDING_LIGHTS_TOGGLE);
          break;
        }
        case 681: {
          sendBasicCommand(inputDefinitions.DEFINITION_LANDING_LIGHT_UP);
          break;
        }
        case 682: {
          sendBasicCommand(inputDefinitions.DEFINITION_LANDING_LIGHT_DOWN);
          break;
        }
        case 683: {
          sendBasicCommand(inputDefinitions.DEFINITION_LANDING_LIGHT_LEFT);
          break;
        }
        case 684: {
          sendBasicCommand(inputDefinitions.DEFINITION_LANDING_LIGHT_RIGHT);
          break;
        }
        case 685: {
          sendBasicCommand(inputDefinitions.DEFINITION_LANDING_LIGHT_HOME);
          break;
        }
        case 686: {
          sendBasicCommand(inputDefinitions.DEFINITION_STROBES_ON);
          break;
        }
        case 687: {
          sendBasicCommand(inputDefinitions.DEFINITION_STROBES_OFF);
          break;
        }
        case 688: {
          sendBasicCommand(inputDefinitions.DEFINITION_PANEL_LIGHTS_ON);
          break;
        }
        case 689: {
          sendBasicCommand(inputDefinitions.DEFINITION_PANEL_LIGHTS_OFF);
          break;
        }
        case 690: {
          sendBasicCommand(inputDefinitions.DEFINITION_LANDING_LIGHTS_ON);
          break;
        }
        case 691: {
          sendBasicCommand(inputDefinitions.DEFINITION_LANDING_LIGHTS_OFF);
          break;
        }
        case 692: {
          sendBasicCommand(inputDefinitions.DEFINITION_TOGGLE_BEACON_LIGHTS);
          break;
        }
        case 693: {
          sendBasicCommand(inputDefinitions.DEFINITION_TOGGLE_TAXI_LIGHTS);
          break;
        }
        case 694: {
          sendBasicCommand(inputDefinitions.DEFINITION_TOGGLE_LOGO_LIGHTS);
          break;
        }
        case 695: {
          sendBasicCommand(
              inputDefinitions.DEFINITION_TOGGLE_RECOGNITION_LIGHTS);
          break;
        }
        case 696: {
          sendBasicCommand(inputDefinitions.DEFINITION_TOGGLE_WING_LIGHTS);
          break;
        }
        case 697: {
          sendBasicCommand(inputDefinitions.DEFINITION_TOGGLE_NAV_LIGHTS);
          break;
        }
        case 698: {
          sendBasicCommand(inputDefinitions.DEFINITION_TOGGLE_CABIN_LIGHTS);
          break;
        }
        case 699: {
          sendBasicCommand(inputDefinitions.DEFINITION_GEAR_UP);
          break;
        }
        case 700: {
          sendBasicCommand(inputDefinitions.DEFINITION_GEAR_DOWN);
          break;
        }
        case 701: {
          sendBasicCommand(inputDefinitions.DEFINITION_TOGGLE_STRUCTURAL_DEICE);
          break;
        }
        case 702: {
          sendBasicCommand(inputDefinitions.DEFINITION_TOGGLE_PROPELLER_DEICE);
          break;
        }
        case 703: {
          sendBasicCommand(inputDefinitions.DEFINITION_TOGGLE_ALTERNATOR1);
          break;
        }
        case 704: {
          sendBasicCommand(inputDefinitions.DEFINITION_TOGGLE_ALTERNATOR2);
          break;
        }
        case 705: {
          sendBasicCommand(inputDefinitions.DEFINITION_TOGGLE_ALTERNATOR3);
          break;
        }
        case 706: {
          sendBasicCommand(inputDefinitions.DEFINITION_TOGGLE_ALTERNATOR4);
          break;
        }
        case 707: {
          sendBasicCommand(inputDefinitions.DEFINITION_ATC);
          break;
        }
        case 708: {
          sendBasicCommand(inputDefinitions.DEFINITION_ATC_MENU_1);
          break;
        }
        case 709: {
          sendBasicCommand(inputDefinitions.DEFINITION_ATC_MENU_2);
          break;
        }
        case 710: {
          sendBasicCommand(inputDefinitions.DEFINITION_ATC_MENU_3);
          break;
        }
        case 711: {
          sendBasicCommand(inputDefinitions.DEFINITION_ATC_MENU_4);
          break;
        }
        case 712: {
          sendBasicCommand(inputDefinitions.DEFINITION_ATC_MENU_5);
          break;
        }
        case 713: {
          sendBasicCommand(inputDefinitions.DEFINITION_ATC_MENU_6);
          break;
        }
        case 714: {
          sendBasicCommand(inputDefinitions.DEFINITION_ATC_MENU_7);
          break;
        }
        case 715: {
          sendBasicCommand(inputDefinitions.DEFINITION_ATC_MENU_8);
          break;
        }
        case 716: {
          sendBasicCommand(inputDefinitions.DEFINITION_ATC_MENU_9);
          break;
        }
        case 717: {
          sendBasicCommand(inputDefinitions.DEFINITION_ATC_MENU_0);
          break;
        }
        case 718: {
          sendBasicCommand(inputDefinitions.DEFINITION_EGT1_INC);
          break;
        }
        case 719: {
          sendBasicCommand(inputDefinitions.DEFINITION_EGT1_DEC);
          break;
        }
        case 720: {
          sendBasicCommand(inputDefinitions.DEFINITION_EGT2_INC);
          break;
        }
        case 721: {
          sendBasicCommand(inputDefinitions.DEFINITION_EGT2_DEC);
          break;
        }
        case 722: {
          sendBasicCommand(inputDefinitions.DEFINITION_EGT3_INC);
          break;
        }
        case 723: {
          sendBasicCommand(inputDefinitions.DEFINITION_EGT3_DEC);
          break;
        }
        case 724: {
          sendBasicCommand(inputDefinitions.DEFINITION_EGT4_INC);
          break;
        }
        case 725: {
          sendBasicCommand(inputDefinitions.DEFINITION_EGT4_DEC);
          break;
        }
        case 726: {
          sendBasicCommand(inputDefinitions.DEFINITION_EGT_INC);
          break;
        }
        case 727: {
          sendBasicCommand(inputDefinitions.DEFINITION_EGT_DEC);
          break;
        }
        case 728: {
          sendBasicCommand(inputDefinitions.DEFINITION_TOGGLE_GPS_DRIVES_NAV1);
          break;
        }
        case 729: {
          sendBasicCommand(inputDefinitions.DEFINITION_GPS_POWER_BUTTON);
          break;
        }
        case 730: {
          sendBasicCommand(inputDefinitions.DEFINITION_GPS_NEAREST_BUTTON);
          break;
        }
        case 731: {
          sendBasicCommand(inputDefinitions.DEFINITION_GPS_OBS_BUTTON);
          break;
        }
        case 732: {
          sendBasicCommand(inputDefinitions.DEFINITION_GPS_MSG_BUTTON);
          break;
        }
        case 733: {
          sendBasicCommand(inputDefinitions.DEFINITION_GPS_MSG_BUTTON_DOWN);
          break;
        }
        case 734: {
          sendBasicCommand(inputDefinitions.DEFINITION_GPS_MSG_BUTTON_UP);
          break;
        }
        case 735: {
          sendBasicCommand(inputDefinitions.DEFINITION_GPS_FLIGHTPLAN_BUTTON);
          break;
        }
        case 736: {
          sendBasicCommand(inputDefinitions.DEFINITION_GPS_TERRAIN_BUTTON);
          break;
        }
        case 737: {
          sendBasicCommand(inputDefinitions.DEFINITION_GPS_PROCEDURE_BUTTON);
          break;
        }
        case 738: {
          sendBasicCommand(inputDefinitions.DEFINITION_GPS_ZOOMIN_BUTTON);
          break;
        }
        case 739: {
          sendBasicCommand(inputDefinitions.DEFINITION_GPS_ZOOMOUT_BUTTON);
          break;
        }
        case 740: {
          sendBasicCommand(inputDefinitions.DEFINITION_GPS_DIRECTTO_BUTTON);
          break;
        }
        case 741: {
          sendBasicCommand(inputDefinitions.DEFINITION_GPS_MENU_BUTTON);
          break;
        }
        case 742: {
          sendBasicCommand(inputDefinitions.DEFINITION_GPS_CLEAR_BUTTON);
          break;
        }
        case 743: {
          sendBasicCommand(inputDefinitions.DEFINITION_GPS_CLEAR_ALL_BUTTON);
          break;
        }
        case 744: {
          sendBasicCommand(inputDefinitions.DEFINITION_GPS_CLEAR_BUTTON_DOWN);
          break;
        }
        case 745: {
          sendBasicCommand(inputDefinitions.DEFINITION_GPS_CLEAR_BUTTON_UP);
          break;
        }
        case 746: {
          sendBasicCommand(inputDefinitions.DEFINITION_GPS_ENTER_BUTTON);
          break;
        }
        case 747: {
          sendBasicCommand(inputDefinitions.DEFINITION_GPS_CURSOR_BUTTON);
          break;
        }
        case 748: {
          sendBasicCommand(inputDefinitions.DEFINITION_GPS_GROUP_KNOB_INC);
          break;
        }
        case 749: {
          sendBasicCommand(inputDefinitions.DEFINITION_GPS_GROUP_KNOB_DEC);
          break;
        }
        case 750: {
          sendBasicCommand(inputDefinitions.DEFINITION_GPS_PAGE_KNOB_INC);
          break;
        }
        case 751: {
          sendBasicCommand(inputDefinitions.DEFINITION_GPS_PAGE_KNOB_DEC);
          break;
        }
        case 752: {
          sendBasicCommand(inputDefinitions.DEFINITION_YAW_DAMPER_TOGGLE);
          break;
        }
        case 753: {
          sendBasicCommand(
              inputDefinitions.DEFINITION_G1000_PFD_FLIGHTPLAN_BUTTON);
          break;
        }
        case 754: {
          sendBasicCommand(
              inputDefinitions.DEFINITION_G1000_PFD_PROCEDURE_BUTTON);
          break;
        }
        case 755: {
          break;
        }
        case 756: {
          sendBasicCommand(
              inputDefinitions.DEFINITION_G1000_PFD_ZOOMOUT_BUTTON);
          break;
        }
        case 757: {
          sendBasicCommand(
              inputDefinitions.DEFINITION_G1000_PFD_DIRECTTO_BUTTON);
          break;
        }
        case 758: {
          sendBasicCommand(inputDefinitions.DEFINITION_G1000_PFD_MENU_BUTTON);
          break;
        }
        case 759: {
          sendBasicCommand(inputDefinitions.DEFINITION_G1000_PFD_CLEAR_BUTTON);
          break;
        }
        case 760: {
          sendBasicCommand(inputDefinitions.DEFINITION_G1000_PFD_ENTER_BUTTON);
          break;
        }
        case 761: {
          sendBasicCommand(inputDefinitions.DEFINITION_G1000_PFD_CURSOR_BUTTON);
          break;
        }
        case 762: {
          sendBasicCommand(
              inputDefinitions.DEFINITION_G1000_PFD_GROUP_KNOB_INC);
          break;
        }
        case 763: {
          sendBasicCommand(
              inputDefinitions.DEFINITION_G1000_PFD_GROUP_KNOB_DEC);
          break;
        }
        case 764: {
          sendBasicCommand(inputDefinitions.DEFINITION_G1000_PFD_PAGE_KNOB_INC);
          break;
        }
        case 765: {
          sendBasicCommand(inputDefinitions.DEFINITION_G1000_PFD_PAGE_KNOB_DEC);
          break;
        }
        case 766: {
          sendBasicCommand(inputDefinitions.DEFINITION_G1000_PFD_SOFTKEY1);
          break;
        }
        case 767: {
          sendBasicCommand(inputDefinitions.DEFINITION_G1000_PFD_SOFTKEY2);
          break;
        }
        case 768: {
          sendBasicCommand(inputDefinitions.DEFINITION_G1000_PFD_SOFTKEY3);
          break;
        }
        case 769: {
          sendBasicCommand(inputDefinitions.DEFINITION_G1000_PFD_SOFTKEY4);
          break;
        }
        case 770: {
          sendBasicCommand(inputDefinitions.DEFINITION_G1000_PFD_SOFTKEY5);
          break;
        }
        case 771: {
          sendBasicCommand(inputDefinitions.DEFINITION_G1000_PFD_SOFTKEY6);
          break;
        }
        case 772: {
          sendBasicCommand(inputDefinitions.DEFINITION_G1000_PFD_SOFTKEY7);
          break;
        }
        case 773: {
          sendBasicCommand(inputDefinitions.DEFINITION_G1000_PFD_SOFTKEY8);
          break;
        }
        case 774: {
          sendBasicCommand(inputDefinitions.DEFINITION_G1000_PFD_SOFTKEY9);
          break;
        }
        case 775: {
          sendBasicCommand(inputDefinitions.DEFINITION_G1000_PFD_SOFTKEY10);
          break;
        }
        case 776: {
          sendBasicCommand(inputDefinitions.DEFINITION_G1000_PFD_SOFTKEY11);
          break;
        }
        case 777: {
          sendBasicCommand(inputDefinitions.DEFINITION_G1000_PFD_SOFTKEY12);
          break;
        }
        case 778: {
          sendBasicCommand(
              inputDefinitions.DEFINITION_G1000_MFD_FLIGHTPLAN_BUTTON);
          break;
        }
        case 779: {
          sendBasicCommand(
              inputDefinitions.DEFINITION_G1000_MFD_PROCEDURE_BUTTON);
          break;
        }
        case 780: {
          sendBasicCommand(inputDefinitions.DEFINITION_G1000_MFD_ZOOMIN_BUTTON);
          break;
        }
        case 781: {
          sendBasicCommand(
              inputDefinitions.DEFINITION_G1000_MFD_ZOOMOUT_BUTTON);
          break;
        }
        case 782: {
          sendBasicCommand(
              inputDefinitions.DEFINITION_G1000_MFD_DIRECTTO_BUTTON);
          break;
        }
        case 783: {
          sendBasicCommand(inputDefinitions.DEFINITION_G1000_MFD_MENU_BUTTON);
          break;
        }
        case 784: {
          sendBasicCommand(inputDefinitions.DEFINITION_G1000_MFD_CLEAR_BUTTON);
          break;
        }
        case 785: {
          sendBasicCommand(inputDefinitions.DEFINITION_G1000_MFD_ENTER_BUTTON);
          break;
        }
        case 786: {
          sendBasicCommand(inputDefinitions.DEFINITION_G1000_MFD_CURSOR_BUTTON);
          break;
        }
        case 787: {
          sendBasicCommand(
              inputDefinitions.DEFINITION_G1000_MFD_GROUP_KNOB_INC);
          break;
        }
        case 788: {
          sendBasicCommand(
              inputDefinitions.DEFINITION_G1000_MFD_GROUP_KNOB_DEC);
          break;
        }
        case 789: {
          sendBasicCommand(inputDefinitions.DEFINITION_G1000_MFD_PAGE_KNOB_INC);
          break;
        }
        case 790: {
          sendBasicCommand(inputDefinitions.DEFINITION_G1000_MFD_PAGE_KNOB_DEC);
          break;
        }
        case 791: {
          sendBasicCommand(inputDefinitions.DEFINITION_G1000_MFD_SOFTKEY1);
          break;
        }
        case 792: {
          sendBasicCommand(inputDefinitions.DEFINITION_G1000_MFD_SOFTKEY2);
          break;
        }
        case 793: {
          sendBasicCommand(inputDefinitions.DEFINITION_G1000_MFD_SOFTKEY3);
          break;
        }
        case 794: {
          sendBasicCommand(inputDefinitions.DEFINITION_G1000_MFD_SOFTKEY4);
          break;
        }
        case 795: {
          sendBasicCommand(inputDefinitions.DEFINITION_G1000_MFD_SOFTKEY5);
          break;
        }
        case 796: {
          sendBasicCommand(inputDefinitions.DEFINITION_G1000_MFD_SOFTKEY6);
          break;
        }
        case 797: {
          sendBasicCommand(inputDefinitions.DEFINITION_G1000_MFD_SOFTKEY7);
          break;
        }
        case 798: {
          sendBasicCommand(inputDefinitions.DEFINITION_G1000_MFD_SOFTKEY8);
          break;
        }
        case 799: {
          sendBasicCommand(inputDefinitions.DEFINITION_G1000_MFD_SOFTKEY9);
          break;
        }
        case 800: {
          sendBasicCommand(inputDefinitions.DEFINITION_G1000_MFD_SOFTKEY10);
          break;
        }
        case 801: {
          sendBasicCommand(inputDefinitions.DEFINITION_G1000_MFD_SOFTKEY11);
          break;
        }
        case 802: {
          sendBasicCommand(inputDefinitions.DEFINITION_G1000_MFD_SOFTKEY2);
          break;
        }
        case 803: {
          sendBasicCommand(inputDefinitions.DEFINITION_AP_MAX_BANK_INC);
          break;
        }
        case 804: {
          sendBasicCommand(inputDefinitions.DEFINITION_AP_MAX_BANK_DEC);
          break;
        }
        case 805: {
          sendBasicCommand(inputDefinitions.DEFINITION_GYRO_DRIFT_INC);
          break;
        }
        case 806: {
          sendBasicCommand(inputDefinitions.DEFINITION_GYRO_DRIFT_DEC);
          break;
        }
        case 807: {
          sendBasicCommand(inputDefinitions.DEFINITION_TRUE_AIRSPEED_CAL_INC);
          break;
        }
        case 808: {
          sendBasicCommand(inputDefinitions.DEFINITION_TRUE_AIRSPEED_CAL_DEC);
          break;
        }
        case 809: {
          sendBasicCommand(
              inputDefinitions.DEFINITION_DECREASE_DECISION_HEIGHT);
          break;
        }
        case 810: {
          sendBasicCommand(
              inputDefinitions.DEFINITION_INCREASE_DECISION_HEIGHT);
          break;
        }
        case 900: {
          setElevatorTrim(index);
          break;
        }
        case 901: {
          setRudder(index);
          break;
        }
        case 902: {
          setBrakeAxis(index);
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
