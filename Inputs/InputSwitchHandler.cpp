
#include "InputSwitchHandler.h"

#include <qlist.h>
#include <qsettings.h>
#include <tchar.h>
#include <windows.h>

#include <QList>
#include <future>
#include <iostream>
#include <string>

#include "inputenum.h"

//#define Bcd2Dec(BcdNum) HornerScheme(BcdNum, 0x10, 10)
#define Dec2Bcd(DecNum) HornerScheme(DecNum, 10, 0x10)

using namespace std;

char *token, *next_token;
bool inputs[5];

int counter = 0;

int yoke[2] = {0, 0};
int oldYoke[2] = {0, 0};
float closedAxis = -16383.0;
float openAxis = 16383.0;

double oldValThrottle[4];
double oldValMixture[2];

int mappedEngines[4];
int mappedProps[4];
int mappedMixture[4];

int flaps;

int trim;
int oldTrim;

int rudderAxis;
int oldRudderAxis;

int leftBrake;
int oldLeftBrake;

int rightBrake;
int oldRightBrake;

InputEnum inputDefinitions = InputEnum();

InputSwitchHandler::InputSwitchHandler() {
  if (!settingsHandler.retrieveSetting("Ranges", "flapsmin")->isNull()) {
    for (int i = 0; i < constants::supportedEngines; i++) {
      QString minStr = "Engine " + QString::number(i + 1) + "Reverse";

      int minRange = settingsHandler.retrieveSetting("Ranges", minStr)->toInt();
      cout << minRange << endl;

      QString idleStr = "Engine " + QString::number(i + 1) + "Idle cutoff";
      int idleCutoff =
          settingsHandler.retrieveSetting("Ranges", idleStr)->toInt();

      QString maxStr = "Engine " + QString::number(i + 1) + "Max";
      int maxRange = settingsHandler.retrieveSetting("Ranges", maxStr)->toInt();

      enginelist[i] = Engine(minRange, idleCutoff, maxRange, i + 1);
    }
    for (auto &curve : curves) {
      curve.append(coordinates(0, -16383));
      curve.append(coordinates(250, -10000));
      curve.append(coordinates(400, 0));
      curve.append(coordinates(500, 0));
      curve.append(coordinates(600, 0));
      curve.append(coordinates(750, 10000));
      curve.append(coordinates(1023, 16383));
    }

    if (!settingsHandler.retrieveSetting("Ranges", "maxReverseRange")
             ->isNull()) {
      reverseAxis =
          settingsHandler.retrieveSetting("Ranges", "maxReverseRange")
              ->toFloat();
    }

    for (int i = 0; i < constants::supportedMixtureLevers; i++) {
      QString minStr = "Mixture " + QString::number(i + 1) + "Min";
      int minRange = settingsHandler.retrieveSetting("Ranges", minStr)->toInt();
      cout << minRange << endl;

      QString idleStr = "Mixture " + QString::number(i + 1) + "Max";
      int maxRange =
          settingsHandler.retrieveSetting("Ranges", idleStr)->toInt();

      mixtureRanges[i] = Range(minRange, maxRange);
    }
    for (int i = 0; i < constants::supportedPropellerLevers; i++) {
      QString minStr = "Propeller " + QString::number(i + 1) + "Min";
      int minRange = settingsHandler.retrieveSetting("Ranges", minStr)->toInt();

      QString idleStr = "Propeller " + QString::number(i + 1) + "Max";
      int maxRange =
          settingsHandler.retrieveSetting("Ranges", idleStr)->toInt();

      propellerRanges[i] = Range(minRange, maxRange);
    }
    int minFlaps =
        settingsHandler.retrieveSetting("Ranges", "FlapsMin")->toInt();
    int maxFlaps =
        settingsHandler.retrieveSetting("Ranges", "FlapsMax")->toInt();
    flapsRange = Range(minFlaps, maxFlaps);

  } else if (settingsHandler.retrieveSetting("Ranges", "FlapsMin")->isNull()) {
    for (int i = 0; i < constants::supportedEngines; i++) {
      enginelist[i] = Engine(0, 0, 1023, i);
    }
    for (int i = 0; i < constants::supportedMixtureLevers; i++) {
      mixtureRanges[i] = Range(0, 1023);
    }
    for (int i = 0; i < constants::supportedPropellerLevers; i++) {
      propellerRanges[i] = Range(0, 1023);
    }

    flapsRange = Range(0, 1023);
  }
}

UINT32 HornerScheme(UINT32 Num, UINT32 Divider, UINT32 Factor) {
  UINT32 Remainder = 0, Quotient = 0, Result = 0;
  Remainder = Num % Divider;
  Quotient = Num / Divider;
  if (!(Quotient == 0 && Remainder == 0))
    Result += HornerScheme(Quotient, Divider, Factor) * Factor + Remainder;
  return Result;
}

// int mapPercentageToAxis(int value) {
//  return -24000.0 + (16383.0 - -21000.0) * ((value - 0.0) / (100.0 - 0.0));
//}
int InputSwitchHandler::mapThrottleValueToAxis(int value, float reverse,
                                               float max, int idleCutoff) {
  int valueThrottle;
  bool reversed = max < idleCutoff;

  /*First we check the orientation by determing if max < idleCutoff
If max < idleCutoff. IF max is smaller we know the potentiometer is mounted
backwards This affects how our logic needs to operate We want to check if the
idle cutoff - reverse < 0 This check tells us wether or not the user wants to
utilize the reverse range or not visa versa for the second check */
  if ((reversed && idleCutoff - reverse < 0 && value >= idleCutoff) ||
      (!reversed && idleCutoff - reverse > 0 && value <= idleCutoff)) {
    valueThrottle =
        reverseAxis + (closedAxis - reverseAxis) *
                          ((value - reverse) / (idleCutoff - reverse));
  } else {
    valueThrottle =
        closedAxis +
        (openAxis - closedAxis) * ((value - idleCutoff) / (max - idleCutoff));
  }
  if (valueThrottle > 16383) {
    return 16383;
  }
  return valueThrottle;
}

int mapValueToAxis(int value, float min, float max) {
  return closedAxis + (openAxis - closedAxis) * ((value - min) / (max - min));
}

void InputSwitchHandler::controlYoke(int index) {
  try {
    token = strtok_s(receivedString[index], " ", &next_token);
    cout << receivedString[index] << endl;
    counter = 0;

    while (token != nullptr && counter < 3) {
      cout << token << ":Counter" << counter << endl;
      int yokeBuffer[2];
      if (token != nullptr) {
        const auto incVal = strtod(token, nullptr);

        if (counter != 0) {
          if (incVal < 10.0) {
            if (oldYoke[counter - 1] < 20.0) {
              oldYoke[counter - 1] = incVal;
            }
          } else {
            oldYoke[counter - 1] = incVal;
          }
          yokeBuffer[counter - 1] = oldYoke[counter - 1];
        }
        counter++;
        token = strtok_s(nullptr, " ", &next_token);
      }
      if (counter == 3) {
        for (int i = 0; i < 2; i++) {
          yoke[i] = yokeBuffer[i];
        }
        int mappedElevator = calibratedRange(yoke[0], 3);
        int mappedAileron = calibratedRange(yoke[1], 2);
        cout << "percentage Elevator: " << mappedElevator << endl;
        sendBasicCommandValue(inputDefinitions.DEFINITION_AXIS_ELEVATOR_SET,
                              mappedElevator);
        cout << "percentage Ailerons: " << mappedElevator << endl;
        sendBasicCommandValue(inputDefinitions.DEFINITION_AXIS_AILERONS_SET,
                              mappedAileron);
      }
    }
  } catch (const std::exception &e) {
    cout << "error in throttle" << endl;
  }
}

void InputSwitchHandler::setFlaps(int index) {
  try {
    token = strtok_s(receivedString[index], " ", &next_token);
    cout << receivedString[index] << endl;
    counter = 0;

    while (token != nullptr && counter < 2) {
      cout << token << ":Counter" << counter << endl;

      if (token != nullptr) {
        const auto incVal = strtod(token, nullptr);

        if (counter != 0) {
          flaps = incVal;
          cout << flaps << endl;
        }

        token = strtok_s(nullptr, " ", &next_token);
        counter++;
      }
      sendBasicCommandValue(inputDefinitions.DEFINITION_AXIS_FLAPS_SET,
                            mapValueToAxis(flaps, flapsRange.getMinRange(),
                                           flapsRange.getMaxRange()));
    }
  }

  catch (const std::exception &e) {
    cout << "error in flaps set" << endl;
  }
}

void InputSwitchHandler::set_throttle_values(int index) {
  // Throttle control

  int engineBuffer[4];
  try {
    token = strtok_s(receivedString[index], " ", &next_token);
    cout << receivedString[index] << endl;
    counter = 0;

    while (token != nullptr && counter < 6) {
      cout << token << ":Counter" << counter << endl;

      if (token != nullptr) {
        const auto incVal = strtod(token, nullptr);

        if (counter != 0) {
          if (incVal < 10.0) {
            if (oldValThrottle[counter] < 20.0) {
              oldValThrottle[counter - 1] = incVal;
            }
          } else {
            oldValThrottle[counter - 1] = incVal;
          }
          engineBuffer[counter - 1] = incVal;
        }

        token = strtok_s(nullptr, " ", &next_token);
        counter++;
      }
      if (counter == 5) {
        for (int i = 0; i < constants::supportedEngines; i++) {
          cout << engineBuffer[i] << endl;
          mappedEngines[i] = mapThrottleValueToAxis(
              engineBuffer[i], enginelist[i].getMinRange(),
              enginelist[i].getMaxRange(), enginelist[i].getIdleIndex());
          cout << "minrange " << i << ": " << enginelist[i].getMinRange()
               << endl;
        }
        cout << "eng "
             << ": " << mappedEngines[3] << endl;
        sendBasicCommandValue(inputDefinitions.DATA_EX_THROTTLE_1_AXIS,
                              mappedEngines[0]);
        sendBasicCommandValue(inputDefinitions.DATA_EX_THROTTLE_2_AXIS,
                              mappedEngines[1]);
        sendBasicCommandValue(inputDefinitions.DATA_EX_THROTTLE_3_AXIS,
                              mappedEngines[2]);
        sendBasicCommandValue(inputDefinitions.DATA_EX_THROTTLE_4_AXIS,
                              mappedEngines[3]);
        //        sendBasicCommandValue(inputDefinitions.DATA_THROTTLE_1_AXIS,
        //                              mappedEngines[0]);
        //        sendBasicCommandValue(inputDefinitions.DATA_THROTTLE_2_AXIS,
        //                              mappedEngines[1]);
        //        sendBasicCommandValue(inputDefinitions.DATA_THROTTLE_3_AXIS,
        //                              mappedEngines[2]);
        //        sendBasicCommandValue(inputDefinitions.DATA_THROTTLE_4_AXIS,
        //                              mappedEngines[3]);
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

          mappedMixture[counter - 1] =
              mapValueToAxis(oldValMixture[counter - 1],
                             mixtureRanges[counter - 1].getMinRange(),
                             mixtureRanges[counter - 1].getMaxRange());
          cout << "counter: " << counter << " val: " << incVal << endl;
        }

        token = strtok_s(nullptr, " ", &next_token);

        counter++;
        if (counter == 2) {
          sendBasicCommandValue(
              inputDefinitions.DEFINITION_MIXTURE_LEVER_AXIS_1,
              mappedMixture[0]);
          sendBasicCommandValue(
              inputDefinitions.DEFINITION_MIXTURE_LEVER_AXIS_2,
              mappedMixture[1]);
          sendBasicCommandValue(
              inputDefinitions.DEFINITION_MIXTURE_LEVER_AXIS_3,
              mappedMixture[0]);
          sendBasicCommandValue(
              inputDefinitions.DEFINITION_MIXTURE_LEVER_AXIS_4,
              mappedMixture[1]);
        }
      }
    }
  } catch (const std::exception &e) {
    cout << "error in throttle" << endl;
  }
}

void InputSwitchHandler::set_prop_values(int index) {
  // Throttle control
  int propAxisBuffer[4];
  try {
    token = strtok_s(receivedString[index], " ", &next_token);
    cout << receivedString[index] << endl;
    counter = 0;

    while (token != nullptr && counter < 6) {
      cout << token << ":Counter" << counter << endl;

      if (token != nullptr) {
        const auto incVal = strtod(token, nullptr);

        if (counter != 0) {
          if (incVal < 10.0) {
            if (oldValThrottle[counter] < 20.0) {
              oldValThrottle[counter - 1] = incVal;
            }
          } else {
            oldValThrottle[counter - 1] = incVal;
          }
          propAxisBuffer[counter - 1] = incVal;
        }

        token = strtok_s(nullptr, " ", &next_token);
        counter++;
      }
      if (counter == 3) {
        for (int i = 0; i < 2; i++) {
          cout << "BUFF" << propAxisBuffer[i] << endl;
          mappedProps[i] = mapValueToAxis(propAxisBuffer[i],
                                          propellerRanges[0].getMinRange(),
                                          propellerRanges[0].getMaxRange());
        }

        sendBasicCommandValue(inputDefinitions.DEFINITION_PROP_LEVER_AXIS_1,
                              mappedProps[0]);
        sendBasicCommandValue(inputDefinitions.DEFINITION_PROP_LEVER_AXIS_2,
                              mappedProps[1]);
        sendBasicCommandValue(inputDefinitions.DEFINITION_PROP_LEVER_AXIS_3,
                              mappedProps[0]);
        sendBasicCommandValue(inputDefinitions.DEFINITION_PROP_LEVER_AXIS_4,
                              mappedProps[1]);
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
    if (diff < 5000 || oldTrim == NULL) {
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
int InputSwitchHandler::calibratedRange(int value, int index) {
  int axis;

  if (static_cast<float>(value) <= curves[index][1].getX()) {
    axis = mapCoordinates(value, curves[index][0], curves[index].at(1));
  }
  // minCurve
  else if (static_cast<float>(value) < curves[index][2].getX()) {
    axis = mapCoordinates(value, curves[index][1], curves[index][2]);
  }
  // deadzone
  else if (static_cast<float>(value) >= curves[index][2].getX() &&
           static_cast<float>(value) <= curves[index][4].getX()) {
    axis = 0;
  } else if (static_cast<float>(value) <= curves[index][5].getX()) {
    axis = mapCoordinates(static_cast<float>(value), curves[index][4],
                          curves[index][5]);
  } else if (static_cast<float>(value) <= curves[index][6].getX()) {
    axis = mapCoordinates(static_cast<float>(value), curves[index][5],
                          curves[index][6]);
  }
  return axis;
}

void InputSwitchHandler::setRudder(int index) {
  try {
    token = strtok_s(receivedString[index], " ", &next_token);

    counter = 0;

    while (token != nullptr && counter < 2) {
      if (counter == 1) {
        int analogValue = stoi(token);
        // minimum to first point
        rudderAxis = calibratedRange(analogValue, 0);
      }
      cout << rudderAxis << " AXIS" << endl;
      token = strtok_s(nullptr, " ", &next_token);
      counter++;
    }
    int diff = std::abs(rudderAxis - oldRudderAxis);
    cout << diff << endl;
    if (diff < 10000 || oldRudderAxis == NULL) {
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

int InputSwitchHandler::mapCoordinates(int value, coordinates toMapMin,
                                       coordinates toMapMax) {
  return toMapMin.getY() +
         (toMapMax.getY() - toMapMin.getY()) *
             ((value - toMapMin.getX()) / (toMapMax.getX() - toMapMin.getX()));
}

void InputSwitchHandler::setBrakeAxis(int index) {
  try {
    token = strtok_s(receivedString[index], " ", &next_token);

    counter = 0;
    while (token != nullptr && counter < 3) {
      int value = stoi(token);
      if (counter == 1) {
        leftBrake = calibratedRange(value, 1);
      }
      if (counter == 2) {
        rightBrake = calibratedRange(value, 1);
      }
      token = strtok_s(nullptr, " ", &next_token);
      counter++;
    }
    cout << oldLeftBrake << "OLD L BRAKE" << endl;
    SimConnect_TransmitClientEvent(
        connect, 0, inputDefinitions.DEFINITION_AXIS_RIGHT_BRAKE_SET,
        rightBrake, SIMCONNECT_GROUP_PRIORITY_HIGHEST,
        SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
    oldLeftBrake = leftBrake;

    SimConnect_TransmitClientEvent(
        connect, 0, inputDefinitions.DEFINITION_AXIS_LEFT_BRAKE_SET, leftBrake,
        SIMCONNECT_GROUP_PRIORITY_HIGHEST,
        SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
    oldRightBrake = rightBrake;

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

void InputSwitchHandler::sendBasicCommand(SIMCONNECT_CLIENT_EVENT_ID eventID,
                                          int index) {
  HRESULT hr;
  string sizeTest = receivedString[index];
  cout << "size: " << sizeTest.length() << endl;
  hr = SimConnect_TransmitClientEvent(
      connect, 0, eventID, 0, SIMCONNECT_GROUP_PRIORITY_HIGHEST,
      SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);

  cout << hr << endl;
}

void InputSwitchHandler::sendWASMCommand(int index, int value) {
  std::string testString = std::to_string(index) + " " + std::to_string(value);

  char arrayTest[256];
  strcpy(arrayTest, testString.c_str());
  puts(arrayTest);
  qDebug() << arrayTest;

  SimConnect_SetClientData(connect, 1, 12,
                           SIMCONNECT_CLIENT_DATA_SET_FLAG_DEFAULT, 0, 256,
                           &arrayTest);
}

void InputSwitchHandler::sendBasicCommandOn(
    SIMCONNECT_CLIENT_EVENT_ID eventID) {
  SimConnect_TransmitClientEvent(connect, 0, eventID, 1,
                                 SIMCONNECT_GROUP_PRIORITY_HIGHEST,
                                 SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
}

void InputSwitchHandler::sendBasicCommandOff(
    SIMCONNECT_CLIENT_EVENT_ID eventID) {
  SimConnect_TransmitClientEvent(connect, 0, eventID, 0,
                                 SIMCONNECT_GROUP_PRIORITY_HIGHEST,
                                 SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
}

void InputSwitchHandler::sendBasicCommandValue(
    SIMCONNECT_CLIENT_EVENT_ID eventID, int value) {
  SimConnect_TransmitClientEvent(connect, 0, eventID, value,
                                 SIMCONNECT_GROUP_PRIORITY_HIGHEST,
                                 SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
}

void InputSwitchHandler::switchHandling(int index) {
  Sleep(10);
  //
  if (strlen(receivedString[index]) > 2) {
    cout << "Yes" << endl;
    prefix = std::string(&receivedString[index][0], &receivedString[index][4]);
    cout << "PREFIX: " << prefix.c_str() << endl;
    cout << "STRING: " << receivedString[index] << endl;

    try {
      int prefixVal = stoi(prefix);

      switch (prefixVal) {
        case 198: {
          set_prop_values(index);

          break;
        }

        case 199: {
          set_throttle_values(index);
          break;
        }
        case 115: {
          setMixtureValues(index);

          break;
        }
        case 103: {
          controlYoke(index);
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
          sendBasicCommand(inputDefinitions.DEFINITION_COM2_RADIO_WHOLE_INC,
                           index);
          break;
        }

          // Swap com1
        case 116: {
          sendBasicCommand(inputDefinitions.DEFINITION_COM_STANDBY_SWAP, index);
          break;
        }
          // Swap com2
        case 117: {
          sendBasicCommand(inputDefinitions.DEFINITION_COM_2_STANDBY_SWAP,
                           index);
          break;
        }
          // Swap nav1
        case 118: {
          sendBasicCommand(inputDefinitions.DEFINITION_NAV_1_STANDBY_SWAP,
                           index);
          break;
        }
          // Swap nav1
        case 119: {
          sendBasicCommand(inputDefinitions.DEFINITION_NAV_2_STANDBY_SWAP,
                           index);
          break;
        }

        case 120: {
          sendBasicCommand(inputDefinitions.DEFINITION_COM2_RADIO_FRACT_DEC,
                           index);
          break;
        }
        case 121: {
          sendBasicCommand(inputDefinitions.DEFINITION_COM2_RADIO_FRACT_INC,
                           index);
          break;
        }
        case 122: {
          sendBasicCommand(
              inputDefinitions.DEFINITION_COM2_RADIO_FRACT_DEC_CARRY, index);
          break;
        }
        case 123: {
          sendBasicCommand(
              inputDefinitions.DEFINITION_COM2_RADIO_FRACT_INC_CARRY, index);
          break;
        }
        case 124: {
          sendBasicCommand(inputDefinitions.DEFINITION_COM_RADIO_FRACT_DEC,
                           index);
          break;
        }
        case 125: {
          sendBasicCommand(inputDefinitions.DEFINITION_COM_RADIO_FRACT_INC,
                           index);
          break;
        }
        case 126: {
          sendBasicCommand(
              inputDefinitions.DEFINITION_COM_RADIO_FRACT_DEC_CARRY, index);
          break;
        }
        case 127: {
          sendBasicCommand(
              inputDefinitions.DEFINITION_COM_RADIO_FRACT_INC_CARRY, index);
          break;
        }

          // NAV
        case 128: {
          sendBasicCommand(inputDefinitions.DEFINITION_NAV_1_RADIO_FRACT_INC,
                           index);
          break;
        }
        case 129: {
          sendBasicCommand(inputDefinitions.DEFINITION_NAV_1_RADIO_FRACT_DEC,
                           index);
          break;
        }
        case 130: {
          sendBasicCommand(inputDefinitions.DEFINITION_NAV_2_RADIO_FRACT_INC,
                           index);
          break;
        }
        case 131: {
          sendBasicCommand(inputDefinitions.DEFINITION_NAV_2_RADIO_FRACT_DEC,
                           index);
          break;
        }
        case 132: {
          sendBasicCommand(inputDefinitions.DEFINITION_NAV_1_RADIO_WHOLE_INC,
                           index);
          break;
        }
        case 133: {
          sendBasicCommand(inputDefinitions.DEFINITION_NAV_1_RADIO_WHOLE_DEC,
                           index);
          break;
        }
        case 134: {
          sendBasicCommand(inputDefinitions.DEFINITION_NAV_2_RADIO_WHOLE_INC,
                           index);
          break;
        }
        case 135: {
          sendBasicCommand(inputDefinitions.DEFINITION_NAV_2_RADIO_WHOLE_DEC,
                           index);
          break;
        }
          // DME
        case 136: {
          sendBasicCommand(
              inputDefinitions.DEFINITION_RADIO_SELECTED_DME1_IDENT_ENABLE,
              index);
          break;
        }
        case 137: {
          sendBasicCommand(
              inputDefinitions.DEFINITION_RADIO_SELECTED_DME1_IDENT_DISABLE,
              index);
          break;
        }
        case 138: {
          sendBasicCommand(
              inputDefinitions.DEFINITION_RADIO_SELECTED_DME1_IDENT_SET, index);
          break;
        }
        case 139: {
          sendBasicCommand(
              inputDefinitions.DEFINITION_RADIO_SELECTED_DM1E_IDENT_TOGGLE,
              index);
          break;
        }
        case 140: {
          sendBasicCommand(
              inputDefinitions.DEFINITION_RADIO_SELECTED_DME2_IDENT_ENABLE,
              index);
          break;
        }
        case 141: {
          sendBasicCommand(
              inputDefinitions.DEFINITION_RADIO_SELECTED_DME2_IDENT_DISABLE,
              index);

          break;
        }
        case 142: {
          sendBasicCommand(
              inputDefinitions.DEFINITION_RADIO_SELECTED_DME2_IDENT_SET, index);

          break;
        }
        case 143: {
          sendBasicCommand(
              inputDefinitions.DEFINITION_RADIO_SELECTED_DME2_IDENT_TOGGLE,
              index);

          break;
        }
        case 144: {
          sendBasicCommand(inputDefinitions.DEFINITION_TOGGLE_DME, index);
          break;
        }

          // VOR1
        case 145: {
          sendBasicCommand(
              inputDefinitions.DEFINITION_RADIO_SELECTED_VOR1_IDENT_ENABLE,
              index);
          break;
        }
        case 146: {
          sendBasicCommand(
              inputDefinitions.DEFINITION_RADIO_SELECTED_VOR1_IDENT_DISABLE,
              index);
          break;
        }
          //!
        case 147: {
          sendBasicCommand(
              inputDefinitions.DEFINITION_RADIO_SELECTED_VOR1_IDENT_SET, index);
          break;
        }
        case 148: {
          sendBasicCommand(
              inputDefinitions.DEFINITION_RADIO_SELECTED_VORE_IDENT_TOGGLE,
              index);
          break;
        }
        case 149: {
          sendBasicCommand(
              inputDefinitions.DEFINITION_RADIO_SELECTED_VOR2_IDENT_ENABLE,
              index);
          break;
        }
        case 150: {
          sendBasicCommand(
              inputDefinitions.DEFINITION_RADIO_SELECTED_VOR2_IDENT_DISABLE,
              index);
          break;
        }
        case 151: {
          // TO DO SET FUNCTION
          sendBasicCommand(
              inputDefinitions.DEFINITION_RADIO_SELECTED_VOR2_IDENT_SET, index);
          break;
        }
        case 152: {
          sendBasicCommand(
              inputDefinitions.DEFINITION_RADIO_SELECTED_VOR2_IDENT_TOGGLE,
              index);
          break;
        }
        case 153: {
          sendBasicCommand(inputDefinitions.DEFINITION_TOGGLE_VOR, index);
          break;
        }
        case 154: {
          sendBasicCommand(inputDefinitions.DEFINITION_VOR1_OBI_DEC, index);
          break;
        }
        case 155: {
          sendBasicCommand(inputDefinitions.DEFINITION_VOR1_OBI_INC, index);
          break;
        }
        case 156: {
          sendBasicCommand(inputDefinitions.DEFINITION_VOR2_OBI_DEC, index);
          break;
        }
        case 157: {
          sendBasicCommand(inputDefinitions.DEFINITION_VOR2_OBI_INC, index);
          break;
        }

          // ADF
        case 158: {
          sendBasicCommand(
              inputDefinitions.DEFINITION_RADIO_SELECTED_ADF_IDENT_ENABLE,
              index);
          break;
        }
        case 159: {
          sendBasicCommand(
              inputDefinitions.DEFINITION_RADIO_SELECTED_ADF_IDENT_DISABLE,
              index);
          break;
        }
        case 160: {
          // TO DO SET FUNCTION
          sendBasicCommand(
              inputDefinitions.DEFINITION_RADIO_SELECTED_ADF_IDENT_SET, index);
          break;
        }
        case 161: {
          sendBasicCommand(
              inputDefinitions.DEFINITION_RADIO_SELECTED_ADF_IDENT_TOGGLE,
              index);
          break;
        }
        case 162: {
          sendBasicCommand(inputDefinitions.DEFINITION_ADF_CARD_INC, index);
          break;
        }
        case 163: {
          sendBasicCommand(inputDefinitions.DEFINITION_ADF_CARD_DEC, index);
          break;
        }
        case 164: {
          // TO DO SET FUNCTION
          sendBasicCommand(inputDefinitions.DEFINITION_ADF_CARD_SET, index);
          break;
        }
        case 165: {
          sendBasicCommand(inputDefinitions.DEFINITION_ADF_100_INC, index);
          break;
        }
        case 166: {
          sendBasicCommand(inputDefinitions.DEFINITION_ADF_10_INC, index);
          break;
        }
        case 167: {
          sendBasicCommand(inputDefinitions.DEFINITION_ADF_1_INC, index);
          break;
        }
        case 168: {
          sendBasicCommand(inputDefinitions.DEFINITION_ADF_100_DEC, index);
          break;
        }
        case 169: {
          sendBasicCommand(inputDefinitions.DEFINITION_ADF_10_DEC, index);
          break;
        }
        case 170: {
          sendBasicCommand(inputDefinitions.DEFINITION_ADF_1_DEC, index);
          break;
        }

          // XPNDR
        case 171: {
          sendBasicCommand(inputDefinitions.DEFINITION_XPNDR_1000_INC, index);
          break;
        }
        case 172: {
          sendBasicCommand(inputDefinitions.DEFINITION_XPNDR_100_INC, index);
          break;
        }
        case 173: {
          sendBasicCommand(inputDefinitions.DEFINITION_XPNDR_10_INC, index);
          break;
        }
        case 174: {
          sendBasicCommand(inputDefinitions.DEFINITION_XPNDR_1_INC, index);
          break;
        }
        case 175: {
          sendBasicCommand(inputDefinitions.DEFINITION_XPNDR_1000_DEC, index);
          break;
        }
        case 176: {
          sendBasicCommand(inputDefinitions.DEFINITION_XPNDR_100_DEC, index);
          break;
        }
        case 177: {
          sendBasicCommand(inputDefinitions.DEFINITION_XPNDR_10_DEC, index);
          break;
        }
        case 178: {
          sendBasicCommand(inputDefinitions.DEFINITION_XPNDR_1_DEC, index);
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
          sendBasicCommand(inputDefinitions.DEFINITION_ADF_COMPLETE_SET, index);
          break;
        }
        case 180: {
          sendBasicCommand(inputDefinitions.DEFINITION_ADF1_WHOLE_INC, index);
          break;
        }
        case 181: {
          sendBasicCommand(inputDefinitions.DEFINITION_ADF1_WHOLE_DEC, index);
          break;
        }
        case 182: {
          sendBasicCommand(inputDefinitions.DEFINITION_ADF2_100_INC, index);
          break;
        }
        case 183: {
          sendBasicCommand(inputDefinitions.DEFINITION_ADF2_10_INC, index);
          break;
        }
        case 184: {
          sendBasicCommand(inputDefinitions.DEFINITION_ADF2_1_INC, index);
          break;
        }
        case 185: {
          sendBasicCommand(inputDefinitions.DEFINITION_ADF2_RADIO_TENTHS_INC,
                           index);
          break;
        }
        case 186: {
          sendBasicCommand(inputDefinitions.DEFINITION_ADF2_100_DEC, index);
          break;
        }
        case 187: {
          sendBasicCommand(inputDefinitions.DEFINITION_ADF2_10_DEC, index);
          break;
        }
        case 188: {
          sendBasicCommand(inputDefinitions.DEFINITION_ADF2_1_DEC, index);
          break;
        }
        case 189: {
          sendBasicCommand(inputDefinitions.DEFINITION_ADF2_RADIO_TENTHS_DEC,
                           index);
          break;
        }
        case 190: {
          sendBasicCommand(inputDefinitions.DEFINITION_ADF2_WHOLE_INC, index);
          break;
        }
        case 191: {
          sendBasicCommand(inputDefinitions.DEFINITION_ADF2_WHOLE_DEC, index);
          break;
        }
        case 192: {
          sendBasicCommand(inputDefinitions.DEFINITION_ADF2_FRACT_DEC_CARRY,
                           index);
          break;
        }
        case 193: {
          sendBasicCommand(inputDefinitions.DEFINITION_ADF2_FRACT_INC_CARRY,
                           index);
          break;
        }
        case 194: {
          // TO DO SET FUNCTION
          sendBasicCommand(inputDefinitions.DEFINITION_ADF2_COMPLETE_SET,
                           index);
          break;
        }
        case 195: {
          sendBasicCommand(inputDefinitions.DEFINITION_RADIO_ADF2_IDENT_DISABLE,
                           index);
          break;
        }
        case 196: {
          sendBasicCommand(inputDefinitions.DEFINITION_RADIO_ADF2_IDENT_ENABLE,
                           index);
          break;
        }
        case 197: {
          sendBasicCommand(inputDefinitions.DEFINITION_RADIO_ADF2_IDENT_TOGGLE,
                           index);
          break;
        }
        case 200: {
          // TO DO SET FUNCTION
          sendBasicCommand(inputDefinitions.DEFINITION_RADIO_ADF2_IDENT_SET,
                           index);
          break;
        }
        case 201: {
          sendBasicCommand(inputDefinitions.DEFINITION_ENG_AUTO_IGN_1, index);
          break;
        }

        case 250: {
          sendBasicCommand(inputDefinitions.DEFINITION_G1000_PFD_ZOOMOUT_BUTTON,
                           index);
          break;
        }

        case 251: {
          sendBasicCommand(inputDefinitions.DEFINITION_G1000_PFD_ZOOMIN_BUTTON,
                           index);
          break;
        }

          // AP
        case 301: {
          sendBasicCommand(inputDefinitions.DEFINITION_AP_MASTER, index);
          break;
        }
        case 302: {
          sendBasicCommand(inputDefinitions.DEFINITION_AP_PANEL_HEADING_HOLD,
                           index);
          break;
        }
        case 303: {
          sendBasicCommand(inputDefinitions.DEFINITION_AP_PANEL_ALTITUDE_HOLD,
                           index);
          break;
        }
        case 304: {
          sendBasicCommand(inputDefinitions.DEFINITION_AP_ATT_HOLD_ON, index);
          break;
        }
        case 305: {
          sendBasicCommand(inputDefinitions.DEFINITION_AP_LOC_HOLD_ON, index);
          break;
        }
        case 306: {
          sendBasicCommand(inputDefinitions.DEFINITION_AP_APR_HOLD_ON, index);
          break;
        }
        case 307: {
          sendBasicCommand(inputDefinitions.DEFINITION_AP_HDG_HOLD_ON, index);
          break;
        }
        case 308: {
          sendBasicCommand(inputDefinitions.DEFINITION_AP_ALT_HOLD_ON, index);
          break;
        }
        case 309: {
          sendBasicCommand(inputDefinitions.DEFINITION_AP_WING_LEVELER_ON,
                           index);
          break;
        }
        case 310: {
          sendBasicCommand(inputDefinitions.DEFINITION_AP_BC_HOLD_ON, index);
          break;
        }
        case 311: {
          sendBasicCommand(inputDefinitions.DEFINITION_AP_NAV1_HOLD_ON, index);
          break;
        }
        case 312: {
          sendBasicCommand(inputDefinitions.DEFINITION_AP_ATT_HOLD_OFF, index);
          break;
        }
        case 313: {
          sendBasicCommand(inputDefinitions.DEFINITION_AP_LOC_HOLD_OFF, index);
          break;
        }
        case 314: {
          sendBasicCommand(inputDefinitions.DEFINITION_AP_APR_HOLD_OFF, index);
          break;
        }
        case 315: {
          sendBasicCommand(inputDefinitions.DEFINITION_AP_HDG_HOLD_OFF, index);
          break;
        }
        case 316: {
          sendBasicCommand(inputDefinitions.DEFINITION_AP_ALT_HOLD_OFF, index);
          break;
        }
        case 317: {
          sendBasicCommand(inputDefinitions.DEFINITION_AP_WING_LEVELER_OFF,
                           index);
          break;
        }
        case 318: {
          sendBasicCommand(inputDefinitions.DEFINITION_AP_BC_HOLD_OFF, index);
          break;
        }
        case 319: {
          sendBasicCommand(inputDefinitions.DEFINITION_AP_NAV1_HOLD_OFF, index);
          break;
        }
        case 320: {
          sendBasicCommand(inputDefinitions.DEFINITION_AP_AIRSPEED_HOLD, index);
          break;
        }
        case 321: {
          sendBasicCommand(inputDefinitions.DEFINITION_AP_PANEL_SPEED_HOLD,
                           index);
          break;
        }
        case 322: {
          sendBasicCommand(inputDefinitions.DEFINITION_AP_ALT_VAR_INC, index);
          break;
        }
        case 323: {
          sendBasicCommand(inputDefinitions.DEFINITION_AP_ALT_VAR_DEC, index);
          break;
        }
        case 324: {
          sendBasicCommand(inputDefinitions.DEFINITION_AP_VS_VAR_INC, index);
          break;
        }
        case 325: {
          sendBasicCommand(inputDefinitions.DEFINITION_AP_VS_VAR_DEC, index);
          break;
        }
        case 326: {
          sendBasicCommand(inputDefinitions.DEFINITION_AP_SPD_VAR_INC, index);
          break;
        }
        case 327: {
          sendBasicCommand(inputDefinitions.DEFINITION_AP_SPD_VAR_DEC, index);
          break;
        }
        case 328: {
          sendBasicCommand(inputDefinitions.DEFINITION_AP_PANEL_MACH_HOLD,
                           index);
          break;
        }
        case 329: {
          sendBasicCommand(inputDefinitions.DEFINITION_AP_MACH_VAR_INC, index);
          break;
        }
        case 330: {
          sendBasicCommand(inputDefinitions.DEFINITION_AP_MACH_VAR_DEC, index);
          break;
        }
        case 331: {
          sendBasicCommand(inputDefinitions.DEFINITION_AP_MACH_HOLD, index);
          break;
        }
        case 332: {
          // TO DO SET FUNCTION
          sendBasicCommand(inputDefinitions.DEFINITION_AP_ALT_VAR_SET_METRIC,
                           index);
          break;
        }
          // TO DO SET FUNCTION
        case 333: {
          sendBasicCommand(inputDefinitions.DEFINITION_AP_VS_VAR_SET_ENGLISH,
                           index);
          break;
        }
        case 334: {
          // TO DO SET FUNCTION
          sendBasicCommand(inputDefinitions.DEFINITION_AP_SPD_VAR_SET, index);
          break;
        }
        case 335: {
          // TO DO SET FUNCTION
          sendBasicCommand(inputDefinitions.DEFINITION_AP_MACH_VAR_SET, index);
          break;
        }
        case 336: {
          sendBasicCommand(inputDefinitions.DEFINITION_AP_AIRSPEED_ON, index);
          break;
        }
        case 337: {
          sendBasicCommand(inputDefinitions.DEFINITION_AP_AIRSPEED_OFF, index);
          break;
        }
        case 338: {
          // TO DO SET FUNCTION
          sendBasicCommand(inputDefinitions.DEFINITION_AP_AIRSPEED_SET, index);
          break;
        }
        case 339: {
          sendBasicCommand(inputDefinitions.DEFINITION_AP_MACH_ON, index);
          break;
        }
        case 340: {
          sendBasicCommand(inputDefinitions.DEFINITION_AP_MACH_OFF, index);
          break;
        }
        case 341: {
          // TO DO SET FUNCTION
          sendBasicCommand(inputDefinitions.DEFINITION_AP_MACH_SET, index);
          break;
        }
        case 342: {
          sendBasicCommand(inputDefinitions.DEFINITION_AP_PANEL_ALTITUDE_ON,
                           index);
          break;
        }
        case 343: {
          sendBasicCommand(inputDefinitions.DEFINITION_AP_PANEL_ALTITUDE_OFF,
                           index);
          break;
        }
        case 345: {
          // TO DO SET FUNCTION
          sendBasicCommand(inputDefinitions.DEFINITION_AP_PANEL_ALTITUDE_SET,
                           index);
          break;
        }
        case 346: {
          sendBasicCommand(inputDefinitions.DEFINITION_AP_PANEL_HEADING_ON,
                           index);
          break;
        }
        case 347: {
          sendBasicCommand(inputDefinitions.DEFINITION_AP_PANEL_HEADING_OFF,
                           index);
          break;
        }
        case 348: {
          // TO DO SET FUNCTION
          sendBasicCommand(inputDefinitions.DEFINITION_AP_PANEL_HEADING_SET,
                           index);
          break;
        }
        case 349: {
          sendBasicCommand(inputDefinitions.DEFINITION_AP_PANEL_MACH_ON, index);
          break;
        }
        case 350: {
          sendBasicCommand(inputDefinitions.DEFINITION_AP_PANEL_MACH_OFF,
                           index);
          break;
        }
        case 351: {
          // TO DO SET FUNCTION
          sendBasicCommand(inputDefinitions.DEFINITION_AP_PANEL_MACH_SET,
                           index);
          break;
        }
        case 352: {
          sendBasicCommand(inputDefinitions.DEFINITION_AP_PANEL_SPEED_ON,
                           index);
          break;
        }
        case 353: {
          sendBasicCommand(inputDefinitions.DEFINITION_AP_PANEL_SPEED_OFF,
                           index);
          break;
        }
        case 354: {
          // TO DO SET FUNCTION
          sendBasicCommand(inputDefinitions.DEFINITION_AP_PANEL_SPEED_SET,
                           index);
          break;
        }
        case 355: {
          // TO DO SET FUNCTION
          sendBasicCommand(inputDefinitions.DEFINITION_AP_ALT_VAR_SET_ENGLISH,
                           index);
          break;
        }
        case 356: {
          // TO DO SET FUNCTION
          sendBasicCommand(inputDefinitions.DEFINITION_AP_VS_VAR_SET_METRIC,
                           index);
          break;
        }
        case 357: {
          sendBasicCommand(
              inputDefinitions.DEFINITION_AP_PANEL_SPEED_HOLD_TOGGLE, index);
          break;
        }
        case 358: {
          sendBasicCommand(
              inputDefinitions.DEFINITION_AP_PANEL_MACH_HOLD_TOGGLE, index);
          break;
        }
        case 359: {
          // TO DO SET FUNCTION
          sendBasicCommand(inputDefinitions.DEFINITION_AP_NAV_SELECT_SET,
                           index);
          break;
        }
        case 360: {
          sendBasicCommand(inputDefinitions.DEFINITION_AP_PITCH_REF_INC_UP,
                           index);
          break;
        }
        case 361: {
          sendBasicCommand(inputDefinitions.DEFINITION_AP_PITCH_REF_INC_DN,
                           index);
          break;
        }
        case 362: {
          sendBasicCommand(inputDefinitions.DEFINITION_AP_PITCH_REF_SELECT,
                           index);
          break;
        }
        case 363: {
          sendBasicCommand(inputDefinitions.DEFINITION_AP_ATT_HOLD, index);
          break;
        }
        case 364: {
          sendBasicCommand(inputDefinitions.DEFINITION_AP_LOC_HOLD, index);
          break;
        }
        case 365: {
          sendBasicCommand(inputDefinitions.DEFINITION_AP_APR_HOLD, index);
          break;
        }
        case 366: {
          sendBasicCommand(inputDefinitions.DEFINITION_AP_HDG_HOLD, index);
          break;
        }
        case 367: {
          sendBasicCommand(inputDefinitions.DEFINITION_AP_ALT_HOLD, index);
          break;
        }
        case 368: {
          sendBasicCommand(inputDefinitions.DEFINITION_AP_WING_LEVELER, index);
          break;
        }
        case 369: {
          sendBasicCommand(inputDefinitions.DEFINITION_AP_BC_HOLD, index);
          break;
        }
        case 370: {
          sendBasicCommand(inputDefinitions.DEFINITION_AP_NAV1_HOLD, index);
          break;
        }
        case 371: {
          sendBasicCommand(inputDefinitions.DEFINITION_HEADING_BUG_INC, index);
          break;
        }
        case 372: {
          sendBasicCommand(inputDefinitions.DEFINITION_HEADING_BUG_DEC, index);
          break;
        }
        case 373: {
          sendBasicCommand(inputDefinitions.DEFINITION_TOGGLE_FLIGHT_DIRECTOR,
                           index);
          break;
        }
        case 374: {
          sendBasicCommand(inputDefinitions.DEFINITION_AP_VS_HOLD, index);
          break;
        }
        case 375: {
          sendBasicCommand(inputDefinitions.DEFINITION_KOHLSMAN_INC, index);
          break;
        }
        case 376: {
          sendBasicCommand(inputDefinitions.DEFINITION_KOHLSMAN_DEC, index);
          break;
        }
        case 378: {
          sendBasicCommand(inputDefinitions.DEFINITION_AUTO_THROTTLE_ARM,
                           index);
          break;
        }
        case 379: {
          sendBasicCommand(inputDefinitions.DEFINITION_AUTO_THROTTLE_TO_GA,
                           index);
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
          sendBasicCommand(inputDefinitions.DEFINITION_TOGGLE_MASTER_ALTERNATOR,
                           index);
          break;
        }
        case 408: {
          sendBasicCommand(inputDefinitions.DEFINITION_TOGGLE_EXTERNAL_POWER,
                           index);
          break;
        }
        case 420: {
          sendBasicCommand(inputDefinitions.DEFINITION_PARKING_BRAKE, index);
          break;
        }
        case 421: {
          setFlaps(index);
          break;
        }
        case 501: {
          sendBasicCommand(inputDefinitions.DEFINITION_FLAPS_UP, index);
          break;
        }
        case 502: {
          sendBasicCommand(inputDefinitions.DEFINITION_FLAPS_1, index);
          break;
        }
        case 503: {
          sendBasicCommand(inputDefinitions.DEFINITION_FLAPS_2, index);
          break;
        }
        case 504: {
          sendBasicCommand(inputDefinitions.DEFINITION_FLAPS_3, index);
          break;
        }
        case 505: {
          sendBasicCommand(inputDefinitions.DEFINITION_FLAPS_DOWN, index);
          break;
        }
        case 506: {
          sendBasicCommand(inputDefinitions.DEFINITION_FLAPS_INCR, index);
          break;
        }
        case 507: {
          sendBasicCommand(inputDefinitions.DEFINITION_FLAPS_DECR, index);
          break;
        }
        case 508: {
          sendBasicCommand(inputDefinitions.DEFINITION_PITOT_HEAT_TOGGLE,
                           index);
          break;
        }
        case 509: {
          sendBasicCommand(inputDefinitions.DEFINITION_PITOT_HEAT_ON, index);
          break;
        }
        case 510: {
          sendBasicCommand(inputDefinitions.DEFINITION_PITOT_HEAT_OFF, index);
          break;
        }
        case 511: {
          sendBasicCommand(inputDefinitions.DEFINITION_MAGNETO, index);
          break;
        }
        case 512: {
          sendBasicCommand(inputDefinitions.DEFINITION_MAGNETO_DECR, index);
          break;
        }
        case 513: {
          sendBasicCommand(inputDefinitions.DEFINITION_MAGNETO_INCR, index);
          break;
        }
        case 514: {
          sendBasicCommand(inputDefinitions.DEFINITION_MAGNETO1_OFF, index);
          break;
        }
        case 515: {
          sendBasicCommand(inputDefinitions.DEFINITION_MAGNETO1_RIGHT, index);
          break;
        }
        case 516: {
          sendBasicCommand(inputDefinitions.DEFINITION_MAGNETO1_LEFT, index);
          break;
        }
        case 517: {
          sendBasicCommand(inputDefinitions.DEFINITION_MAGNETO1_BOTH, index);
          break;
        }
        case 518: {
          sendBasicCommand(inputDefinitions.DEFINITION_MAGNETO1_START, index);
          break;
        }
        case 519: {
          sendBasicCommand(inputDefinitions.DEFINITION_MAGNETO2_OFF, index);
          break;
        }
        case 520: {
          sendBasicCommand(inputDefinitions.DEFINITION_MAGNETO2_RIGHT, index);
          break;
        }
        case 521: {
          sendBasicCommand(inputDefinitions.DEFINITION_MAGNETO2_LEFT, index);
          break;
        }
        case 522: {
          sendBasicCommand(inputDefinitions.DEFINITION_MAGNETO2_BOTH, index);
          break;
        }
        case 523: {
          sendBasicCommand(inputDefinitions.DEFINITION_MAGNETO2_START, index);
          break;
        }
        case 524: {
          sendBasicCommand(inputDefinitions.DEFINITION_MAGNETO3_OFF, index);
          break;
        }
        case 525: {
          sendBasicCommand(inputDefinitions.DEFINITION_MAGNETO3_RIGHT, index);
          break;
        }
        case 526: {
          sendBasicCommand(inputDefinitions.DEFINITION_MAGNETO3_LEFT, index);
          break;
        }
        case 527: {
          sendBasicCommand(inputDefinitions.DEFINITION_MAGNETO3_BOTH, index);
          break;
        }
        case 528: {
          sendBasicCommand(inputDefinitions.DEFINITION_MAGNETO3_START, index);
          break;
        }
        case 529: {
          sendBasicCommand(inputDefinitions.DEFINITION_MAGNETO4_OFF, index);
          break;
        }
        case 530: {
          sendBasicCommand(inputDefinitions.DEFINITION_MAGNETO4_RIGHT, index);
          break;
        }
        case 531: {
          sendBasicCommand(inputDefinitions.DEFINITION_MAGNETO4_LEFT, index);
          break;
        }
        case 532: {
          sendBasicCommand(inputDefinitions.DEFINITION_MAGNETO4_BOTH, index);
          break;
        }
        case 533: {
          sendBasicCommand(inputDefinitions.DEFINITION_MAGNETO4_START, index);
          break;
        }
        case 534: {
          sendBasicCommand(inputDefinitions.DEFINITION_MAGNETO_OFF, index);
          break;
        }
        case 535: {
          sendBasicCommand(inputDefinitions.DEFINITION_MAGNETO_RIGHT, index);
          break;
        }
        case 536: {
          sendBasicCommand(inputDefinitions.DEFINITION_MAGNETO_LEFT, index);
          break;
        }
        case 537: {
          sendBasicCommand(inputDefinitions.DEFINITION_MAGNETO_BOTH, index);
          break;
        }
        case 538: {
          sendBasicCommand(inputDefinitions.DEFINITION_MAGNETO_START, index);
          break;
        }
        case 539: {
          sendBasicCommand(inputDefinitions.DEFINITION_MAGNETO1_DECR, index);
          break;
        }
        case 540: {
          sendBasicCommand(inputDefinitions.DEFINITION_MAGNETO1_INCR, index);
          break;
        }
        case 541: {
          sendBasicCommand(inputDefinitions.DEFINITION_MAGNETO2_DECR, index);
          break;
        }
        case 542: {
          sendBasicCommand(inputDefinitions.DEFINITION_MAGNETO2_INCR, index);
          break;
        }
        case 543: {
          sendBasicCommand(inputDefinitions.DEFINITION_MAGNETO3_DECR, index);
          break;
        }
        case 544: {
          sendBasicCommand(inputDefinitions.DEFINITION_MAGNETO3_INCR, index);
          break;
        }
        case 545: {
          sendBasicCommand(inputDefinitions.DEFINITION_MAGNETO4_DECR, index);
          break;
        }
        case 546: {
          sendBasicCommand(inputDefinitions.DEFINITION_MAGNETO4_INCR, index);
          break;
        }
        case 547: {
          // TO DO SET FUNCTION
          sendBasicCommand(inputDefinitions.DEFINITION_MAGNETO_SET, index);
          break;
        }
        case 548: {
          // TO DO SET FUNCTION
          sendBasicCommand(inputDefinitions.DEFINITION_MAGNETO1_SET, index);
          break;
        }
        case 549: {
          // TO DO SET FUNCTION
          sendBasicCommand(inputDefinitions.DEFINITION_MAGNETO2_SET, index);
          break;
        }
        case 550: {
          // TO DO SET FUNCTION
          sendBasicCommand(inputDefinitions.DEFINITION_MAGNETO3_SET, index);
          break;
        }
        case 551: {
          // TO DO SET FUNCTION
          sendBasicCommand(inputDefinitions.DEFINITION_MAGNETO4_SET, index);
          break;
        }
        case 552: {
          sendBasicCommand(inputDefinitions.DEFINITION_ANTI_ICE_ON, index);
          break;
        }
        case 553: {
          sendBasicCommand(inputDefinitions.DEFINITION_ANTI_ICE_OFF, index);
          break;
        }
        case 554: {
          // TO DO SET FUNCTION
          sendBasicCommand(inputDefinitions.DEFINITION_ANTI_ICE_SET, index);
          break;
        }
        case 555: {
          sendBasicCommand(inputDefinitions.DEFINITION_ANTI_ICE_TOGGLE, index);
          break;
        }
        case 556: {
          sendBasicCommand(inputDefinitions.DEFINITION_ANTI_ICE_TOGGLE_ENG1,
                           index);
          break;
        }
        case 557: {
          sendBasicCommand(inputDefinitions.DEFINITION_ANTI_ICE_TOGGLE_ENG2,
                           index);
          break;
        }
        case 558: {
          sendBasicCommand(inputDefinitions.DEFINITION_ANTI_ICE_TOGGLE_ENG3,
                           index);
          break;
        }
        case 559: {
          sendBasicCommand(inputDefinitions.DEFINITION_ANTI_ICE_TOGGLE_ENG4,
                           index);
          break;
        }
        case 560: {
          // TO DO SET FUNCTION
          sendBasicCommand(inputDefinitions.DEFINITION_ANTI_ICE_SET_ENG1,
                           index);
          break;
        }
        case 561: {
          // TO DO SET FUNCTION
          sendBasicCommand(inputDefinitions.DEFINITION_ANTI_ICE_SET_ENG2,
                           index);
          break;
        }
        case 562: {
          // TO DO SET FUNCTION
          sendBasicCommand(inputDefinitions.DEFINITION_ANTI_ICE_SET_ENG3,
                           index);
          break;
        }
        case 563: {
          // TO DO SET FUNCTION
          sendBasicCommand(inputDefinitions.DEFINITION_ANTI_ICE_SET_ENG4,
                           index);
          break;
        }
        case 564: {
          sendBasicCommand(inputDefinitions.DEFINITION_TOGGLE_FUEL_VALVE_ALL,
                           index);
          break;
        }
        case 565: {
          sendBasicCommand(inputDefinitions.DEFINITION_TOGGLE_FUEL_VALVE_ENG1,
                           index);
          break;
        }
        case 566: {
          sendBasicCommand(inputDefinitions.DEFINITION_TOGGLE_FUEL_VALVE_ENG2,
                           index);
          break;
        }
        case 567: {
          sendBasicCommand(inputDefinitions.DEFINITION_TOGGLE_FUEL_VALVE_ENG3,
                           index);
          break;
        }
        case 568: {
          sendBasicCommand(inputDefinitions.DEFINITION_TOGGLE_FUEL_VALVE_ENG4,
                           index);
          break;
        }
        case 569: {
          sendBasicCommand(inputDefinitions.DEFINITION_FUEL_SELECTOR_OFF,
                           index);
          break;
        }
        case 570: {
          sendBasicCommand(inputDefinitions.DEFINITION_FUEL_SELECTOR_ALL,
                           index);
          break;
        }
        case 571: {
          sendBasicCommand(inputDefinitions.DEFINITION_FUEL_SELECTOR_LEFT,
                           index);
          break;
        }
        case 572: {
          sendBasicCommand(inputDefinitions.DEFINITION_FUEL_SELECTOR_RIGHT,
                           index);
          break;
        }
        case 573: {
          sendBasicCommand(inputDefinitions.DEFINITION_FUEL_SELECTOR_LEFT_AUX,
                           index);
          break;
        }
        case 574: {
          sendBasicCommand(inputDefinitions.DEFINITION_FUEL_SELECTOR_RIGHT_AUX,
                           index);
          break;
        }
        case 575: {
          sendBasicCommand(inputDefinitions.DEFINITION_FUEL_SELECTOR_CENTER,
                           index);
          break;
        }
        case 576: {
          // TO DO SET FUEL SELECTOR
          sendBasicCommand(inputDefinitions.DEFINITION_FUEL_SELECTOR_SET,
                           index);
          break;
        }
        case 577: {
          sendBasicCommand(inputDefinitions.DEFINITION_FUEL_SELECTOR_2_OFF,
                           index);
          break;
        }
        case 578: {
          sendBasicCommand(inputDefinitions.DEFINITION_FUEL_SELECTOR_2_ALL,
                           index);
          break;
        }
        case 579: {
          sendBasicCommand(inputDefinitions.DEFINITION_FUEL_SELECTOR_2_LEFT,
                           index);
          break;
        }
        case 580: {
          sendBasicCommand(inputDefinitions.DEFINITION_FUEL_SELECTOR_2_RIGHT,
                           index);
          break;
        }
        case 581: {
          sendBasicCommand(inputDefinitions.DEFINITION_FUEL_SELECTOR_2_LEFT_AUX,
                           index);
          break;
        }
        case 582: {
          sendBasicCommand(
              inputDefinitions.DEFINITION_FUEL_SELECTOR_2_RIGHT_AUX, index);
          break;
        }
        case 583: {
          sendBasicCommand(inputDefinitions.DEFINITION_FUEL_SELECTOR_2_CENTER,
                           index);
          break;
        }
          // TO DO SET FUNCTION
        case 584: {
          sendBasicCommand(inputDefinitions.DEFINITION_FUEL_SELECTOR_2_SET,
                           index);
          break;
        }
        case 585: {
          sendBasicCommand(inputDefinitions.DEFINITION_FUEL_SELECTOR_3_OFF,
                           index);
          break;
        }
        case 586: {
          sendBasicCommand(inputDefinitions.DEFINITION_FUEL_SELECTOR_3_ALL,
                           index);
          break;
        }
        case 587: {
          sendBasicCommand(inputDefinitions.DEFINITION_FUEL_SELECTOR_3_LEFT,
                           index);
          break;
        }
        case 588: {
          sendBasicCommand(inputDefinitions.DEFINITION_FUEL_SELECTOR_3_RIGHT,
                           index);
          break;
        }
        case 589: {
          sendBasicCommand(inputDefinitions.DEFINITION_FUEL_SELECTOR_3_LEFT_AUX,
                           index);
          break;
        }
        case 590: {
          sendBasicCommand(
              inputDefinitions.DEFINITION_FUEL_SELECTOR_3_RIGHT_AUX, index);
          break;
        }
        case 591: {
          sendBasicCommand(inputDefinitions.DEFINITION_FUEL_SELECTOR_3_CENTER,
                           index);
          break;
        }
        case 592: {
          // TO DO SET FUNCTION
          sendBasicCommand(inputDefinitions.DEFINITION_FUEL_SELECTOR_3_SET,
                           index);
          break;
        }
        case 593: {
          sendBasicCommand(inputDefinitions.DEFINITION_FUEL_SELECTOR_4_OFF,
                           index);
          break;
        }
        case 594: {
          sendBasicCommand(inputDefinitions.DEFINITION_FUEL_SELECTOR_4_ALL,
                           index);
          break;
        }
        case 595: {
          sendBasicCommand(inputDefinitions.DEFINITION_FUEL_SELECTOR_4_LEFT,
                           index);
          break;
        }
        case 596: {
          sendBasicCommand(inputDefinitions.DEFINITION_FUEL_SELECTOR_4_RIGHT,
                           index);
          break;
        }
        case 597: {
          sendBasicCommand(inputDefinitions.DEFINITION_FUEL_SELECTOR_4_LEFT_AUX,
                           index);
          break;
        }
        case 598: {
          sendBasicCommand(
              inputDefinitions.DEFINITION_FUEL_SELECTOR_4_RIGHT_AUX, index);
          break;
        }
        case 599: {
          sendBasicCommand(inputDefinitions.DEFINITION_FUEL_SELECTOR_4_CENTER,
                           index);
          break;
        }
        case 600: {
          // TO DO SET FUNCTION
          sendBasicCommand(inputDefinitions.DEFINITION_FUEL_SELECTOR_4_SET,
                           index);
          break;
        }
        case 601: {
          sendBasicCommand(inputDefinitions.DEFINITION_CROSS_FEED_OPEN, index);
          break;
        }
        case 602: {
          sendBasicCommand(inputDefinitions.DEFINITION_CROSS_FEED_TOGGLE,
                           index);
          break;
        }
        case 603: {
          sendBasicCommand(inputDefinitions.DEFINITION_CROSS_FEED_OFF, index);
          break;
        }

        case 604: {
          sendBasicCommand(inputDefinitions.DEFINITION_ELEV_TRIM_DN, index);
          break;
        }
        case 605: {
          sendBasicCommand(inputDefinitions.DEFINITION_DOWN, index);
          break;
        }
        case 606: {
          sendBasicCommand(inputDefinitions.DEFINITION_ELEV_TRIM_UP, index);
          break;
        }
        case 607: {
          sendBasicCommand(inputDefinitions.DEFINITION_ELEV_UP, index);
          break;
        }
        case 608: {
          sendBasicCommand(inputDefinitions.DEFINITION_SPOILERS_ARM_TOGGLE,
                           index);
          break;
        }
        case 609: {
          sendBasicCommand(inputDefinitions.DEFINITION_SPOILERS_ON, index);
          break;
        }
        case 610: {
          sendBasicCommand(inputDefinitions.DEFINITION_SPOILERS_OFF, index);
          break;
        }
        case 611: {
          sendBasicCommand(inputDefinitions.DEFINITION_SPOILERS_ARM_ON, index);
          break;
        }
        case 612: {
          sendBasicCommand(inputDefinitions.DEFINITION_SPOILERS_ARM_OFF, index);
          break;
        }
        case 613: {
          sendBasicCommand(inputDefinitions.DEFINITION_AILERON_TRIM_LEFT,
                           index);
          break;
        }
        case 614: {
          sendBasicCommand(inputDefinitions.DEFINITION_AILERON_TRIM_RIGHT,
                           index);
          break;
        }
        case 615: {
          sendBasicCommand(inputDefinitions.DEFINITION_RUDDER_TRIM_LEFT, index);
          break;
        }
        case 616: {
          sendBasicCommand(inputDefinitions.DEFINITION_RUDDER_TRIM_RIGHT,
                           index);
          break;
        }
        case 617: {
          // TO DO SET FUNCTION
          sendBasicCommand(inputDefinitions.DEFINITION_COWLFLAP1_SET, index);
          break;
        }
        case 618: {
          // TO DO SET FUNCTION
          sendBasicCommand(inputDefinitions.DEFINITION_COWLFLAP2_SET, index);
          break;
        }
        case 619: {
          // TO DO SET FUNCTION
          sendBasicCommand(inputDefinitions.DEFINITION_COWLFLAP3_SET, index);
          break;
        }
        case 620: {
          // TO DO SET FUNCTION
          sendBasicCommand(inputDefinitions.DEFINITION_COWLFLAP4_SET, index);
          break;
        }
        case 621: {
          sendBasicCommand(inputDefinitions.DEFINITION_INC_COWL_FLAPS, index);
          break;
        }
        case 622: {
          sendBasicCommand(inputDefinitions.DEFINITION_DEC_COWL_FLAPS, index);
          break;
        }
        case 623: {
          sendBasicCommand(inputDefinitions.DEFINITION_INC_COWL_FLAPS1, index);
          break;
        }
        case 624: {
          sendBasicCommand(inputDefinitions.DEFINITION_DEC_COWL_FLAPS1, index);
          break;
        }
        case 625: {
          sendBasicCommand(inputDefinitions.DEFINITION_INC_COWL_FLAPS2, index);
          break;
        }
        case 626: {
          sendBasicCommand(inputDefinitions.DEFINITION_DEC_COWL_FLAPS2, index);
          break;
        }
        case 627: {
          sendBasicCommand(inputDefinitions.DEFINITION_INC_COWL_FLAPS3, index);
          break;
        }
        case 628: {
          sendBasicCommand(inputDefinitions.DEFINITION_DEC_COWL_FLAPS3, index);
          break;
        }
        case 629: {
          sendBasicCommand(inputDefinitions.DEFINITION_INC_COWL_FLAPS4, index);
          break;
        }
        case 630: {
          sendBasicCommand(inputDefinitions.DEFINITION_DEC_COWL_FLAPS4, index);
          break;
        }
        case 631: {
          sendBasicCommand(inputDefinitions.DEFINITION_FUEL_PUMP, index);
          break;
        }
        case 632: {
          sendBasicCommand(inputDefinitions.DEFINITION_TOGGLE_ELECT_FUEL_PUMP,
                           index);
          break;
        }
        case 633: {
          sendBasicCommand(inputDefinitions.DEFINITION_TOGGLE_ELECT_FUEL_PUMP1,
                           index);
          break;
        }
        case 634: {
          sendBasicCommand(inputDefinitions.DEFINITION_TOGGLE_ELECT_FUEL_PUMP2,
                           index);
          break;
        }
        case 635: {
          sendBasicCommand(inputDefinitions.DEFINITION_TOGGLE_ELECT_FUEL_PUMP3,
                           index);
          break;
        }
        case 636: {
          sendBasicCommand(inputDefinitions.DEFINITION_TOGGLE_ELECT_FUEL_PUMP4,
                           index);
          break;
        }
        case 637: {
          sendBasicCommand(inputDefinitions.DEFINITION_ENGINE_PRIMER, index);
          break;
        }
        case 638: {
          sendBasicCommand(inputDefinitions.DEFINITION_TOGGLE_PRIMER, index);
          break;
        }
        case 639: {
          sendBasicCommand(inputDefinitions.DEFINITION_TOGGLE_PRIMER1, index);
          break;
        }
        case 640: {
          sendBasicCommand(inputDefinitions.DEFINITION_TOGGLE_PRIMER2, index);
          break;
        }
        case 641: {
          sendBasicCommand(inputDefinitions.DEFINITION_TOGGLE_PRIMER3, index);
          break;
        }
        case 642: {
          sendBasicCommand(inputDefinitions.DEFINITION_TOGGLE_PRIMER4, index);
          break;
        }
        case 643: {
          sendBasicCommand(inputDefinitions.DEFINITION_TOGGLE_FEATHER_SWITCHES,
                           index);
          break;
        }
        case 644: {
          sendBasicCommand(inputDefinitions.DEFINITION_TOGGLE_FEATHER_SWITCH_1,
                           index);
          break;
        }
        case 645: {
          sendBasicCommand(inputDefinitions.DEFINITION_TOGGLE_FEATHER_SWITCH_2,
                           index);
          break;
        }
        case 646: {
          sendBasicCommand(inputDefinitions.DEFINITION_TOGGLE_FEATHER_SWITCH_3,
                           index);
          break;
        }
        case 647: {
          sendBasicCommand(inputDefinitions.DEFINITION_TOGGLE_FEATHER_SWITCH_4,
                           index);
          break;
        }
        case 648: {
          sendBasicCommand(inputDefinitions.DEFINITION_TOGGLE_PROPELLER_SYNC,
                           index);
          break;
        }
        case 649: {
          sendBasicCommand(inputDefinitions.DEFINITION_TOGGLE_AUTOFEATHER_ARM,
                           index);
          break;
        }
        case 650: {
          sendBasicCommand(inputDefinitions.DEFINITION_TOGGLE_AFTERBURNER,
                           index);
          break;
        }
        case 651: {
          sendBasicCommand(inputDefinitions.DEFINITION_TOGGLE_AFTERBURNER1,
                           index);
          break;
        }
        case 652: {
          sendBasicCommand(inputDefinitions.DEFINITION_TOGGLE_AFTERBURNER2,
                           index);
          break;
        }
        case 653: {
          sendBasicCommand(inputDefinitions.DEFINITION_TOGGLE_AFTERBURNER3,
                           index);
          break;
        }
        case 654: {
          sendBasicCommand(inputDefinitions.DEFINITION_TOGGLE_AFTERBURNER4,
                           index);
          break;
        }
        case 655: {
          sendBasicCommand(inputDefinitions.DEFINITION_ENGINE, index);
          break;
        }
        case 656: {
          sendBasicCommand(inputDefinitions.DEFINITION_SPOILERS_TOGGLE, index);
          break;
        }
        case 657: {
          sendBasicCommand(inputDefinitions.DEFINITION_TOGGLE_STARTER1, index);
          break;
        }
        case 658: {
          sendBasicCommand(inputDefinitions.DEFINITION_TOGGLE_STARTER2, index);
          break;
        }
        case 659: {
          sendBasicCommand(inputDefinitions.DEFINITION_TOGGLE_STARTER3, index);
          break;
        }
        case 660: {
          sendBasicCommand(inputDefinitions.DEFINITION_TOGGLE_STARTER4, index);
          break;
        }
        case 661: {
          sendBasicCommand(inputDefinitions.DEFINITION_TOGGLE_ALL_STARTERS,
                           index);
          break;
        }
        case 662: {
          sendBasicCommand(inputDefinitions.DEFINITION_ENGINE_AUTO_START,
                           index);
          break;
        }
        case 663: {
          sendBasicCommand(inputDefinitions.DEFINITION_ENGINE_AUTO_SHUTDOWN,
                           index);
          break;
        }
        case 664: {
          sendBasicCommand(inputDefinitions.DEFINITION_FUEL_SELECTOR_LEFT_MAIN,
                           index);
          break;
        }
        case 665: {
          sendBasicCommand(
              inputDefinitions.DEFINITION_FUEL_SELECTOR_2_LEFT_MAIN, index);

          break;
        }
        case 666: {
          sendBasicCommand(
              inputDefinitions.DEFINITION_FUEL_SELECTOR_3_LEFT_MAIN, index);
          sendBasicCommandValue(inputDefinitions.DEFINITION_CRJ_SMOKE_ON, 2);
          break;
        }
        case 667: {
          sendBasicCommand(
              inputDefinitions.DEFINITION_FUEL_SELECTOR_4_LEFT_MAIN, index);
          break;
        }
        case 668: {
          sendBasicCommand(inputDefinitions.DEFINITION_FUEL_SELECTOR_RIGHT_MAIN,
                           index);
          break;
        }
        case 669: {
          sendBasicCommand(
              inputDefinitions.DEFINITION_FUEL_SELECTOR_2_RIGHT_MAIN, index);
          break;
        }
        case 670: {
          sendBasicCommand(
              inputDefinitions.DEFINITION_FUEL_SELECTOR_3_RIGHT_MAIN, index);
          break;
        }
        case 671: {
          sendBasicCommand(
              inputDefinitions.DEFINITION_FUEL_SELECTOR_4_RIGHT_MAIN, index);
          break;
        }
        case 672: {
          sendBasicCommand(inputDefinitions.DEFINITION_APU_STARTER, index);
          break;
        }
        case 673: {
          sendBasicCommand(inputDefinitions.DEFINITION_APU_OFF_SWITCH, index);
          break;
        }
        case 674: {
          sendBasicCommand(
              inputDefinitions.DEFINITION_APU_GENERATOR_SWITCH_TOGGLE, index);
          break;
        }
        case 675: {
          sendBasicCommand(
              inputDefinitions.DEFINITION_CABIN_NO_SMOKING_ALERT_SWITCH_TOGGLE,
              index);
          break;
        }
        case 676: {
          sendBasicCommand(
              inputDefinitions.DEFINITION_CABIN_SEATBELTS_ALERT_SWITCH_TOGGLE,
              index);
          break;
        }
        case 677: {
          sendBasicCommand(inputDefinitions.DEFINITION_STROBES_TOGGLE, index);
          break;
        }
        case 678: {
          sendBasicCommand(inputDefinitions.DEFINITION_ALL_LIGHTS_TOGGLE,
                           index);
          break;
        }
        case 679: {
          sendBasicCommand(inputDefinitions.DEFINITION_PANEL_LIGHTS_TOGGLE,
                           index);
          break;
        }
        case 680: {
          sendBasicCommand(inputDefinitions.DEFINITION_LANDING_LIGHTS_TOGGLE,
                           index);
          break;
        }
        case 681: {
          sendBasicCommand(inputDefinitions.DEFINITION_LANDING_LIGHT_UP, index);
          break;
        }
        case 682: {
          sendBasicCommand(inputDefinitions.DEFINITION_LANDING_LIGHT_DOWN,
                           index);
          break;
        }
        case 683: {
          sendBasicCommand(inputDefinitions.DEFINITION_LANDING_LIGHT_LEFT,
                           index);
          break;
        }
        case 684: {
          sendBasicCommand(inputDefinitions.DEFINITION_LANDING_LIGHT_RIGHT,
                           index);
          break;
        }
        case 685: {
          sendBasicCommand(inputDefinitions.DEFINITION_LANDING_LIGHT_HOME,
                           index);
          break;
        }
        case 686: {
          sendBasicCommand(inputDefinitions.DEFINITION_STROBES_ON, index);
          break;
        }
        case 687: {
          sendBasicCommand(inputDefinitions.DEFINITION_STROBES_OFF, index);
          break;
        }
        case 688: {
          sendBasicCommand(inputDefinitions.DEFINITION_PANEL_LIGHTS_ON, index);
          break;
        }
        case 689: {
          sendBasicCommand(inputDefinitions.DEFINITION_PANEL_LIGHTS_OFF, index);
          break;
        }
        case 690: {
          sendBasicCommand(inputDefinitions.DEFINITION_LANDING_LIGHTS_ON,
                           index);
          break;
        }
        case 691: {
          sendBasicCommand(inputDefinitions.DEFINITION_LANDING_LIGHTS_OFF,
                           index);
          break;
        }
        case 692: {
          sendBasicCommand(inputDefinitions.DEFINITION_TOGGLE_BEACON_LIGHTS,
                           index);
          break;
        }
        case 693: {
          sendBasicCommand(inputDefinitions.DEFINITION_TOGGLE_TAXI_LIGHTS,
                           index);
          break;
        }
        case 694: {
          sendBasicCommand(inputDefinitions.DEFINITION_TOGGLE_LOGO_LIGHTS,
                           index);
          break;
        }
        case 695: {
          sendBasicCommand(
              inputDefinitions.DEFINITION_TOGGLE_RECOGNITION_LIGHTS, index);
          break;
        }
        case 696: {
          sendBasicCommand(inputDefinitions.DEFINITION_TOGGLE_WING_LIGHTS,
                           index);
          break;
        }
        case 697: {
          sendBasicCommand(inputDefinitions.DEFINITION_TOGGLE_NAV_LIGHTS,
                           index);
          break;
        }
        case 698: {
          sendBasicCommand(inputDefinitions.DEFINITION_TOGGLE_CABIN_LIGHTS,
                           index);
          break;
        }
        case 699: {
          sendBasicCommand(inputDefinitions.DEFINITION_GEAR_UP, index);
          break;
        }
        case 700: {
          sendBasicCommand(inputDefinitions.DEFINITION_GEAR_DOWN, index);
          break;
        }
        case 701: {
          sendBasicCommand(inputDefinitions.DEFINITION_TOGGLE_STRUCTURAL_DEICE,
                           index);
          break;
        }
        case 702: {
          sendBasicCommand(inputDefinitions.DEFINITION_TOGGLE_PROPELLER_DEICE,
                           index);
          break;
        }
        case 703: {
          sendBasicCommand(inputDefinitions.DEFINITION_TOGGLE_ALTERNATOR1,
                           index);
          break;
        }
        case 704: {
          sendBasicCommand(inputDefinitions.DEFINITION_TOGGLE_ALTERNATOR2,
                           index);
          break;
        }
        case 705: {
          sendBasicCommand(inputDefinitions.DEFINITION_TOGGLE_ALTERNATOR3,
                           index);
          break;
        }
        case 706: {
          sendBasicCommand(inputDefinitions.DEFINITION_TOGGLE_ALTERNATOR4,
                           index);
          break;
        }
        case 707: {
          sendBasicCommand(inputDefinitions.DEFINITION_ATC, index);
          break;
        }
        case 708: {
          sendBasicCommand(inputDefinitions.DEFINITION_ATC_MENU_1, index);
          break;
        }
        case 709: {
          sendBasicCommand(inputDefinitions.DEFINITION_ATC_MENU_2, index);
          break;
        }
        case 710: {
          sendBasicCommand(inputDefinitions.DEFINITION_ATC_MENU_3, index);
          break;
        }
        case 711: {
          sendBasicCommand(inputDefinitions.DEFINITION_ATC_MENU_4, index);
          break;
        }
        case 712: {
          sendBasicCommand(inputDefinitions.DEFINITION_ATC_MENU_5, index);
          break;
        }
        case 713: {
          sendBasicCommand(inputDefinitions.DEFINITION_ATC_MENU_6, index);
          break;
        }
        case 714: {
          sendBasicCommand(inputDefinitions.DEFINITION_ATC_MENU_7, index);
          break;
        }
        case 715: {
          sendBasicCommand(inputDefinitions.DEFINITION_ATC_MENU_8, index);
          break;
        }
        case 716: {
          sendBasicCommand(inputDefinitions.DEFINITION_ATC_MENU_9, index);
          break;
        }
        case 717: {
          sendBasicCommand(inputDefinitions.DEFINITION_ATC_MENU_0, index);
          break;
        }
        case 718: {
          sendBasicCommand(inputDefinitions.DEFINITION_EGT1_INC, index);
          break;
        }
        case 719: {
          sendBasicCommand(inputDefinitions.DEFINITION_EGT1_DEC, index);
          break;
        }
        case 720: {
          sendBasicCommand(inputDefinitions.DEFINITION_EGT2_INC, index);
          break;
        }
        case 721: {
          sendBasicCommand(inputDefinitions.DEFINITION_EGT2_DEC, index);
          break;
        }
        case 722: {
          sendBasicCommand(inputDefinitions.DEFINITION_EGT3_INC, index);
          break;
        }
        case 723: {
          sendBasicCommand(inputDefinitions.DEFINITION_EGT3_DEC, index);
          break;
        }
        case 724: {
          sendBasicCommand(inputDefinitions.DEFINITION_EGT4_INC, index);
          break;
        }
        case 725: {
          sendBasicCommand(inputDefinitions.DEFINITION_EGT4_DEC, index);
          break;
        }
        case 726: {
          sendBasicCommand(inputDefinitions.DEFINITION_EGT_INC, index);
          break;
        }
        case 727: {
          sendBasicCommand(inputDefinitions.DEFINITION_EGT_DEC, index);
          break;
        }
        case 728: {
          sendBasicCommand(inputDefinitions.DEFINITION_TOGGLE_GPS_DRIVES_NAV1,
                           index);
          break;
        }
        case 729: {
          sendBasicCommand(inputDefinitions.DEFINITION_GPS_POWER_BUTTON, index);
          break;
        }
        case 730: {
          sendBasicCommand(inputDefinitions.DEFINITION_GPS_NEAREST_BUTTON,
                           index);
          break;
        }
        case 731: {
          sendBasicCommand(inputDefinitions.DEFINITION_GPS_OBS_BUTTON, index);
          break;
        }
        case 732: {
          sendBasicCommand(inputDefinitions.DEFINITION_GPS_MSG_BUTTON, index);
          break;
        }
        case 733: {
          sendBasicCommand(inputDefinitions.DEFINITION_GPS_MSG_BUTTON_DOWN,
                           index);
          break;
        }
        case 734: {
          sendBasicCommand(inputDefinitions.DEFINITION_GPS_MSG_BUTTON_UP,
                           index);
          break;
        }
        case 735: {
          sendBasicCommand(inputDefinitions.DEFINITION_GPS_FLIGHTPLAN_BUTTON,
                           index);
          break;
        }
        case 736: {
          sendBasicCommand(inputDefinitions.DEFINITION_GPS_TERRAIN_BUTTON,
                           index);
          break;
        }
        case 737: {
          sendBasicCommand(inputDefinitions.DEFINITION_GPS_PROCEDURE_BUTTON,
                           index);
          break;
        }
        case 738: {
          sendBasicCommand(inputDefinitions.DEFINITION_GPS_ZOOMIN_BUTTON,
                           index);
          break;
        }
        case 739: {
          sendBasicCommand(inputDefinitions.DEFINITION_GPS_ZOOMOUT_BUTTON,
                           index);
          break;
        }
        case 740: {
          sendBasicCommand(inputDefinitions.DEFINITION_GPS_DIRECTTO_BUTTON,
                           index);
          break;
        }
        case 741: {
          sendBasicCommand(inputDefinitions.DEFINITION_GPS_MENU_BUTTON, index);
          break;
        }
        case 742: {
          sendBasicCommand(inputDefinitions.DEFINITION_GPS_CLEAR_BUTTON, index);
          break;
        }
        case 743: {
          sendBasicCommand(inputDefinitions.DEFINITION_GPS_CLEAR_ALL_BUTTON,
                           index);
          break;
        }
        case 744: {
          sendBasicCommand(inputDefinitions.DEFINITION_GPS_CLEAR_BUTTON_DOWN,
                           index);
          break;
        }
        case 745: {
          sendBasicCommand(inputDefinitions.DEFINITION_GPS_CLEAR_BUTTON_UP,
                           index);
          break;
        }
        case 746: {
          sendBasicCommand(inputDefinitions.DEFINITION_GPS_ENTER_BUTTON, index);
          break;
        }
        case 747: {
          sendBasicCommand(inputDefinitions.DEFINITION_GPS_CURSOR_BUTTON,
                           index);
          break;
        }
        case 748: {
          sendBasicCommand(inputDefinitions.DEFINITION_GPS_GROUP_KNOB_INC,
                           index);
          break;
        }
        case 749: {
          sendBasicCommand(inputDefinitions.DEFINITION_GPS_GROUP_KNOB_DEC,
                           index);
          break;
        }
        case 750: {
          sendBasicCommand(inputDefinitions.DEFINITION_GPS_PAGE_KNOB_INC,
                           index);
          break;
        }
        case 751: {
          sendBasicCommand(inputDefinitions.DEFINITION_GPS_PAGE_KNOB_DEC,
                           index);
          break;
        }
        case 752: {
          sendBasicCommand(inputDefinitions.DEFINITION_YAW_DAMPER_TOGGLE,
                           index);
          break;
        }

        case 803: {
          sendBasicCommand(inputDefinitions.DEFINITION_AP_MAX_BANK_INC, index);
          break;
        }
        case 804: {
          sendBasicCommand(inputDefinitions.DEFINITION_AP_MAX_BANK_DEC, index);
          break;
        }
        case 805: {
          sendBasicCommand(inputDefinitions.DEFINITION_GYRO_DRIFT_INC, index);
          break;
        }
        case 806: {
          sendBasicCommand(inputDefinitions.DEFINITION_GYRO_DRIFT_DEC, index);
          break;
        }
        case 807: {
          sendBasicCommand(inputDefinitions.DEFINITION_TRUE_AIRSPEED_CAL_INC,
                           index);
          break;
        }
        case 808: {
          sendBasicCommand(inputDefinitions.DEFINITION_TRUE_AIRSPEED_CAL_DEC,
                           index);
          break;
        }
        case 809: {
          sendBasicCommand(inputDefinitions.DEFINITION_DECREASE_DECISION_HEIGHT,
                           index);
          break;
        }
        case 810: {
          sendBasicCommand(inputDefinitions.DEFINITION_INCREASE_DECISION_HEIGHT,
                           index);
          break;
        }

        case 908: {
          sendBasicCommand(inputDefinitions.DEFINITION_SIM_PAUSE_ON, index);
          break;
        }
        case 909: {
          sendBasicCommand(inputDefinitions.DEFINITION_SIM_PAUSE_OFF, index);
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
          int value = 0;
          bool valFound = std::strlen(receivedString[index]) > 6;
          if (valFound) {
            value = stoi(std::string(reinterpret_cast<const char *>(
                                         &receivedString[index]))
                             .substr(4));
          }

          sendWASMCommand(prefixVal, value);
          cout << value << "val" << endl;
          break;
        }
      }

    } catch (const std::exception &e) {
      cout << "error" << endl;
    }
  }
}

void InputSwitchHandler::setCurve(QList<coordinates> curve, int index) {
  curves[index].clear();
  for (auto &coord : curve) {
    curves[index].append(coord);
  }

  cout << "received value:" << curve[3].getX() << "saved  value "
       << curves[index][3].getX() << " ";
}
