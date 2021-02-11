#define DATA_LENGTH 255
#define MAX_RETURNED_ITEMS 255
#include <headers/ConnectWorker.h>
#include <headers/SimConnect.h>
#include <qsettings.h>
#include <qstandardpaths.h>
#include <tchar.h>
#include <windows.h>

#include <string>

#include "stdio.h"
#include "strsafe.h"
/*!
   \class ConnectWorker : public QThread
   \inmodule Bitsanddroidsgui
  \brief The ConnectWorker class.

 */
char output[DATA_LENGTH];
bool strincProcessing = false;
bool connectionError = false;
bool lastConnectionState = false;
float prevSpeed = 0.0f;
float currentSpeed;
int eps = 1;

SerialPort *arduino;

int quit = 0;
HANDLE hSimConnect = NULL;

struct Struct1 {
  char title[256];
};

struct StructOneDatum {
  int id;
  float value;
};
struct dataChange {
  float val;
  std::string prefix;
};

struct StructDatum {
  StructOneDatum datum[MAX_RETURNED_ITEMS];
};

enum EVENT_ID { EVENT_SIM_START };

enum DATA_DEFINE_ID {
  DEFINITION_PDR,
  DEFINITION_STRING,

};

enum DATA_REQUEST_ID {
  REQUEST_PDR,
  REQUEST_STRING,
};
enum DATA_NAMES {
  DATA_TITLE,
  DATA_COM_FREQ1,
  DATA_COM_FREQ_STANDBY1,
  DATA_COM_FREQ2,
  DATA_COM_FREQ_STANDBY2,
  DATA_COM_ALTMTR,
  DATA_HEADING_LOCK,
  DATA_ALTITUDE_TARGET,
  DATA_VERTICAL_SPEED,
  DATA_BAROMETER,

  DATA_TRUE_VERTICAL_SPEED,
  DATA_INDICATED_AIRSPEED,
  DATA_GPS_GROUNDSPEED,
  DATA_INDICATED_HEADING,
  DATA_INDICATED_ALTITUDE,
  DATA_SELECTED_QUANTITY_FUEL,
  // GPS
  DATA_GPS_COURSE_TO_STEER,

  // Coms
  DATA_NAV_ACTIVE_FREQUENCY1,
  DATA_NAV_STANDBY_FREQUENCY1,
  DATA_NAV_ACTIVE_FREQUENCY2,
  DATA_NAV_STANDBY_FREQUENCY2,
  DATA_NAV_RADIAL_ERROR1,
  DATA_NAV_VOR_LATLONALT1,
  // ADF
  DATA_ADF_ACTIVE_FREQUENCY1,
  DATA_ADF_STANDBY_FREQUENCY1,
  DATA_ADF_RADIAL1,
  DATA_ADF_SIGNAL1,
  DATA_ADF_ACTIVE_FREQUENCY2,
  DATA_ADF_STANDBY_FREQUENCY2,
  DATA_ADF_RADIAL2,
  DATA_ADF_SIGNAL2,

  // DME
  DATA_NAV_DME1,
  DATA_NAV_DMESPEED1,

  DATA_NAV_DME2,
  DATA_NAV_DMESPEED2,

  // Transponder
  DATA_TRANSPONDER_CODE1,
  DATA_TRANSPONDER_CODE2,

  // Lights
  DATA_LIGHT_TAXI_ON,
  DATA_LIGHT_STROBE_ON,
  DATA_LIGHT_PANEL_ON,
  DATA_LIGHT_RECOGNITION_ON,
  DATA_LIGHT_WING_ON,
  DATA_LIGHT_LOGO_ON,
  DATA_LIGHT_CABIN_ON,
  DATA_LIGHT_HEAD_ON,
  DATA_LIGHT_BRAKE_ON,
  DATA_LIGHT_NAV_ON,
  DATA_LIGHT_BEACON_ON,
  DATA_LIGHT_LANDING_ON,

  // Warnings
  DATA_STALL_WARNING,
  DATA_OVERSPEED_WARNING,

  // Rudder/trim
  DATA_AILERON_TRIM,
  DATA_AILERON_TRIM_PCT,
  DATA_RUDDER_TRIM,
  DATA_RUDDER_TRIM_PCT,
  DATA_ELEVATOR_TRIM_PCT,
  DATA_ELEVATOR_TRIM_POSITION,

  // Gear
  DATA_GEAR_HANDLE_POSITION,
  DATA_GEAR_HYDRAULIC_PRESSURE,
  DATA_TAILWHEEL_LOCK_ON,
  DATA_GEAR_CENTER_POSITION,
  DATA_GEAR_LEFT_POSITION,
  DATA_GEAR_RIGHT_POSITION,
  DATA_GEAR_TAIL_POSITION,
  DATA_GEAR_AUX_POSITION,
  DATA_GEAR_TOTAL_PCT_EXTENDED,

  // Flaps
  DATA_FLAPS_HANDLE_PERCENT,
  DATA_FLAPS_HANDLE_INDEX,
  DATA_FLAPS_NUM_HANDLE_POSITIONS,
  DATA_TRAILING_EDGE_FLAPS_LEFT_PERCENT,
  DATA_TRAILING_EDGE_FLAPS_RIGHT_PERCENT,
  DATA_TRAILING_EDGE_FLAPS_LEFT_ANGLE,
  DATA_TRAILING_EDGE_FLAPS_RIGHT_ANGLE,
  DATA_LEADING_EDGE_FLAPS_LEFT_PERCENT,
  DATA_LEADING_EDGE_FLAPS_RIGHT_PERCENT,
  DATA_LEADING_EDGE_FLAPS_LEFT_ANGLE,
  DATA_LEADING_EDGE_FLAPS_RIGHT_ANGLE,

  // AP
  DATA_AUTOPILOT_AVAILABLE,
  DATA_AUTOPILOT_MASTER,
  DATA_AUTOPILOT_WING_LEVELER,
  DATA_AUTOPILOT_NAV1_LOCK,
  DATA_AUTOPILOT_HEADING_LOCK,
  DATA_AUTOPILOT_ALTITUDE_LOCK,
  DATA_AUTOPILOT_ATTITUDE_HOLD,
  DATA_AUTOPILOT_GLIDESLOPE_HOLD,
  DATA_AUTOPILOT_APPROACH_HOLD,
  DATA_AUTOPILOT_BACKCOURSE_HOLD,
  DATA_AUTOPILOT_FLIGHT_DIRECTOR_ACTIVE,
  DATA_AUTOPILOT_AIRSPEED_HOLD,
  DATA_AUTOPILOT_MACH_HOLD,
  DATA_AUTOPILOT_YAW_DAMPER,
  DATA_AUTOPILOT_THROTTLE_ARM,
  DATA_AUTOPILOT_TAKEOFF_POWER_ACTIVE,
  DATA_AUTOTHROTTLE_ACTIVE,
  DATA_AUTOPILOT_VERTICAL_HOLD,
  DATA_AUTOPILOT_RPM_HOLD,

  DATA_PLANE_NAME

};

using namespace std;

ConnectWorker::ConnectWorker() {}

void sendToArduino(float received, std::string prefix, const char *portName) {
  auto intVal = static_cast<int>(received);
  SerialPort arduino(portName);
  if (!arduino.isConnected()) {
    connectionError = true;
  } else {
    connectionError = false;
  }
  const auto value = intVal;
  auto input_string = prefix + std::to_string(value);
  auto *const c_string = new char[input_string.size() + 1];
  std::copy(input_string.begin(), input_string.end(), c_string);
  c_string[input_string.size()] = '\n';
  arduino.writeSerialPort(c_string, input_string.length());
  delete[] c_string;
}
void sendCharToArduino(const char *received, std::string prefix,
                       const char *portName) {
  SerialPort arduino(portName);
  if (!arduino.isConnected()) {
    connectionError = true;
  } else {
    connectionError = false;
  }
  const auto value = received;
  auto input_string = prefix + value;
  auto *const c_string = new char[input_string.size() + 1];
  std::copy(input_string.begin(), input_string.end(), c_string);
  c_string[input_string.size()] = '\n';

  arduino.writeSerialPort(c_string, input_string.length());
  delete[] c_string;
}
void sendLengthToArduino(float received, std::string prefix,
                         const char *portName, int strLength) {
  auto intVal = static_cast<int>(received);
  SerialPort arduino(portName);
  if (!arduino.isConnected()) {
    connectionError = true;
  } else {
    connectionError = false;
  }
  const auto value = intVal;
  auto input_string = prefix + std::to_string(value);
  switch ((strLength + 3) - input_string.size()) {
    case 1: {
      input_string += " ";
      break;
    }
    case 2: {
      input_string += "  ";
      break;
    }
    case 3: {
      input_string += "   ";
      break;
    }
    case 4: {
      input_string += "    ";
      break;
    }
    case 5: {
      input_string += "     ";
      break;
    }
    default:
      break;
  }

  auto *const c_string = new char[input_string.size() + 1];
  std::copy(input_string.begin(), input_string.end(), c_string);
  c_string[input_string.size()] = '\n';

  arduino.writeSerialPort(c_string, input_string.length());
  delete[] c_string;
}

void sendFloatToArduino(float received, std::string prefix,
                        const char *portName) {
  SerialPort arduino(portName);
  if (!arduino.isConnected()) {
    connectionError = true;
  } else {
    connectionError = false;
  }
  auto input_string = prefix + std::to_string(received);
  auto *const c_string = new char[input_string.size() + 1];
  std::copy(input_string.begin(), input_string.end(), c_string);
  c_string[input_string.size()] = '\n';
  if (received < 0) {
    arduino.writeSerialPort(c_string, 7);
  } else {
    arduino.writeSerialPort(c_string, 6);
  }

  delete[] c_string;
}

void sendBoolToArduino(float received, std::string prefix,
                       const char *portName) {
  int intVal;
  if (received == 0) {
    intVal = 0;
  } else {
    intVal = 1;
  }

  SerialPort arduino(portName);
  if (!arduino.isConnected()) {
    connectionError = true;
  } else {
    connectionError = false;
  }
  const auto value = intVal;
  auto input_string = prefix + std::to_string(value);
  auto *const c_string = new char[input_string.size() + 1];
  std::copy(input_string.begin(), input_string.end(), c_string);
  c_string[input_string.size()] = '\n';

  arduino.writeSerialPort(c_string, input_string.length());
  delete[] c_string;
}

int radianToDegree(double rec) {
  double pi = 3.14159;
  double radian = rec;
  return (radian * (180 / pi));
}

float radianToDegreeFloat(double rec) {
  double pi = 3.14159;
  double radian = rec;
  return (radian * (180 / pi));
}

void ConnectWorker::MyDispatchProcRD(SIMCONNECT_RECV *pData, DWORD cbData,
                                     void *pContext) {
  QString path =
      QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
  QSettings settings(path + "/" + "settings.ini", QSettings::IniFormat);
  settings.beginGroup("Coms");
  string val = settings.value("outputComActiveBase").toString().toStdString();
  const char *valPort = val.c_str();
  settings.endGroup();
  settings.sync();

  settings.beginGroup("Settings");
  int updatePerXFrames = settings.value("updateEveryXFramesLineEdit").toInt();
  if (settings.value("updateEveryXFramesLineEdit").isNull()) {
    updatePerXFrames = 15;
  }
  HRESULT hr;
  switch (pData->dwID) {
    case SIMCONNECT_RECV_ID_EVENT: {
      SIMCONNECT_RECV_EVENT *evt = (SIMCONNECT_RECV_EVENT *)pData;

      switch (evt->uEventID) {
        case EVENT_SIM_START:

          // Now the sim is running, request information on the user aircraft
          hr = SimConnect_RequestDataOnSimObject(
              hSimConnect, REQUEST_PDR, DEFINITION_PDR,
              SIMCONNECT_OBJECT_ID_USER, SIMCONNECT_PERIOD_VISUAL_FRAME,
              SIMCONNECT_DATA_REQUEST_FLAG_CHANGED |
                  SIMCONNECT_DATA_REQUEST_FLAG_TAGGED,
              0, updatePerXFrames);
          if (strincProcessing) {
            hr = SimConnect_RequestDataOnSimObject(
                hSimConnect, REQUEST_STRING, DEFINITION_STRING,
                SIMCONNECT_OBJECT_ID_USER, SIMCONNECT_PERIOD_VISUAL_FRAME,
                SIMCONNECT_DATA_REQUEST_FLAG_CHANGED |
                    SIMCONNECT_DATA_REQUEST_FLAG_TAGGED,
                0, 15);
          }

          break;

        default:
          break;
      }
      break;
    }

    case SIMCONNECT_RECV_ID_SIMOBJECT_DATA: {
      SIMCONNECT_RECV_SIMOBJECT_DATA *pObjData =
          (SIMCONNECT_RECV_SIMOBJECT_DATA *)pData;

      switch (pObjData->dwRequestID) {
        case REQUEST_STRING: {
          Struct1 *pS = (Struct1 *)&pObjData->dwData;
          sendCharToArduino(pS->title, "999", valPort);
          cout << "Plane: " << pS->title << endl;
          cout << "hi" << endl;
          break;
        }

        case REQUEST_PDR: {
          int count = 0;
          auto pS = reinterpret_cast<StructDatum *>(&pObjData->dwData);

          while (count < (int)pObjData->dwDefineCount)

          {
            string valString = std::to_string(pS->datum[count].value);
            switch (pS->datum[count].id) {
              // GPS
              case DATA_GPS_COURSE_TO_STEER: {
                sendToArduino(radianToDegree(pS->datum[count].value), "454",
                              valPort);
                break;
              }

                // Avionics
              case DATA_VERTICAL_SPEED: {
                sendLengthToArduino(pS->datum[count].value, "590", valPort, 5);
                printf("\nAP Vertical speed = %f", pS->datum[count].value);
                break;
              }
              case DATA_COM_ALTMTR: {
                sendLengthToArduino(pS->datum[count].value * 1000, "337",
                                    valPort, 4);
                printf("\nKohlman hg = %f", pS->datum[count].value);
                break;
              }
              case DATA_HEADING_LOCK: {
                sendLengthToArduino(pS->datum[count].value, "582", valPort, 4);
                printf("\nHeading lock = %f", pS->datum[count].value);
                break;
              }
              case DATA_ALTITUDE_TARGET: {
                sendToArduino(pS->datum[count].value, "584", valPort);
                printf("\nAP Altitude target = %f", pS->datum[count].value);
                cout << "called alt target: " << pS->datum[count].value << endl;
                break;
              }
              case DATA_BAROMETER: {
                sendLengthToArduino(pS->datum[count].value, "632", valPort, 4);
                printf("\nBarometer = %f", pS->datum[count].value);
                break;
              }

                // Coms
              case DATA_COM_FREQ1: {
                sendToArduino(pS->datum[count].value, "900", valPort);
                printf("\nactive freq 1 = %f", pS->datum[count].value);
                break;
              }
              case DATA_COM_FREQ_STANDBY1: {
                sendToArduino(pS->datum[count].value, "901", valPort);
                printf("\nstandby freq 1 = %f", pS->datum[count].value);
                break;
              }
              case DATA_COM_FREQ2: {
                sendToArduino(pS->datum[count].value, "902", valPort);
                printf("\nactive freq 2 = %f", pS->datum[count].value);
                break;
              }
              case DATA_COM_FREQ_STANDBY2: {
                sendToArduino(pS->datum[count].value, "903", valPort);
                printf("\nstandby freq 2 = %f", pS->datum[count].value);
                break;
              }
              case DATA_NAV_ACTIVE_FREQUENCY1: {
                sendToArduino(pS->datum[count].value * 100, "910", valPort);
                break;
              }
              case DATA_NAV_STANDBY_FREQUENCY1: {
                sendToArduino(pS->datum[count].value * 100, "911", valPort);
                break;
              }
              case DATA_NAV_ACTIVE_FREQUENCY2: {
                sendToArduino(pS->datum[count].value * 100, "912", valPort);
                break;
              }
              case DATA_NAV_STANDBY_FREQUENCY2: {
                sendToArduino(pS->datum[count].value * 100, "913", valPort);
                break;
              }
              case DATA_NAV_RADIAL_ERROR1: {
                sendToArduino(pS->datum[count].value, "914", valPort);
                break;
              }
              case DATA_NAV_VOR_LATLONALT1: {
                sendToArduino(pS->datum[count].value, "915", valPort);
                break;
              }

                // DME
              case DATA_NAV_DME1: {
                sendToArduino(pS->datum[count].value, "950", valPort);
                break;
              }
              case DATA_NAV_DMESPEED1: {
                sendToArduino(pS->datum[count].value, "951", valPort);
                break;
              }
              case DATA_NAV_DME2: {
                sendToArduino(pS->datum[count].value, "952", valPort);
                break;
              }
              case DATA_NAV_DMESPEED2: {
                sendToArduino(pS->datum[count].value, "953", valPort);
                break;
              }

                // ADF
              case DATA_ADF_ACTIVE_FREQUENCY1: {
                sendLengthToArduino(pS->datum[count].value / 1000, "954",
                                    valPort, 4);
                break;
              }
              case DATA_ADF_STANDBY_FREQUENCY1: {
                sendLengthToArduino(pS->datum[count].value / 1000, "955",
                                    valPort, 4);
                break;
              }
              case DATA_ADF_RADIAL1: {
                sendToArduino(pS->datum[count].value, "956", valPort);
                break;
              }
              case DATA_ADF_SIGNAL1: {
                sendToArduino(pS->datum[count].value, "957", valPort);
                break;
              }

              case DATA_ADF_ACTIVE_FREQUENCY2: {
                sendLengthToArduino(pS->datum[count].value / 1000, "958",
                                    valPort, 4);
                break;
              }
              case DATA_ADF_STANDBY_FREQUENCY2: {
                sendLengthToArduino(pS->datum[count].value / 1000, "959",
                                    valPort, 4);
                break;
              }
              case DATA_ADF_RADIAL2: {
                sendToArduino(pS->datum[count].value, "960", valPort);
                break;
              }
              case DATA_ADF_SIGNAL2: {
                sendToArduino(pS->datum[count].value, "961", valPort);
                break;
              }

                // Transponder
              case DATA_TRANSPONDER_CODE1: {
                sendToArduino(pS->datum[count].value, "962", valPort);
                break;
              }
              case DATA_TRANSPONDER_CODE2: {
                sendToArduino(pS->datum[count].value, "963", valPort);
                break;
              }

                // Aircraft data
              case DATA_INDICATED_AIRSPEED: {
                sendLengthToArduino(pS->datum[count].value, "326", valPort, 3);
                printf("\nIndicated airspeed = %f", pS->datum[count].value);
                break;
              }
              case DATA_INDICATED_ALTITUDE: {
                sendLengthToArduino(pS->datum[count].value, "335", valPort, 5);

                printf("\nIndicated altitude = %f", pS->datum[count].value);
                break;
              }
              case DATA_INDICATED_HEADING: {
                sendLengthToArduino(radianToDegree(pS->datum[count].value),
                                    "344", valPort, 3);
                printf("\nIndicated heading = %f", pS->datum[count].value);
                break;
              }
              case DATA_GPS_GROUNDSPEED: {
                sendToArduino(pS->datum[count].value * 1.94, "430", valPort);
                printf("\nGPS groundspeed = %f", pS->datum[count].value);
                break;
              }
              case DATA_TRUE_VERTICAL_SPEED: {
                sendToArduino(pS->datum[count].value, "330", valPort);
                printf("\nTrue vertical speed = %f", pS->datum[count].value);
                break;
              }
              case DATA_SELECTED_QUANTITY_FUEL: {
                sendToArduino(pS->datum[count].value * 100, "275", valPort);
                printf("\nFuel in = %f", pS->datum[count].value * 100);
                break;
              }

              // lights
              case DATA_LIGHT_TAXI_ON: {
                sendBoolToArduino(pS->datum[count].value, "033", valPort);
                QString test =
                    QString::fromStdString("Taxi light: " + valString);

                cout << "Taxi: " << pS->datum[count].value << endl;
                break;
              }
              case DATA_LIGHT_STROBE_ON: {
                sendBoolToArduino(pS->datum[count].value, "034", valPort);
                cout << "Strobe: " << pS->datum[count].value << endl;

                break;
              }
              case DATA_LIGHT_PANEL_ON: {
                sendBoolToArduino(pS->datum[count].value, "035", valPort);
                cout << "Panel: " << pS->datum[count].value << endl;
                break;
              }
              case DATA_LIGHT_RECOGNITION_ON: {
                sendBoolToArduino(pS->datum[count].value, "036", valPort);
                cout << "Recognition: " << pS->datum[count].value << endl;
                break;
              }
              case DATA_LIGHT_WING_ON: {
                sendBoolToArduino(pS->datum[count].value, "037", valPort);
                cout << "Wing light: " << pS->datum[count].value << endl;
                break;
              }
              case DATA_LIGHT_LOGO_ON: {
                sendBoolToArduino(pS->datum[count].value, "038", valPort);
                cout << "Logo light: " << pS->datum[count].value << endl;
                break;
              }
              case DATA_LIGHT_CABIN_ON: {
                sendBoolToArduino(pS->datum[count].value, "039", valPort);
                cout << "Cabin light: " << pS->datum[count].value << endl;
                break;
              }
              case DATA_LIGHT_HEAD_ON: {
                sendBoolToArduino(pS->datum[count].value, "040", valPort);
                cout << "Head light: " << pS->datum[count].value << endl;
                break;
              }
              case DATA_LIGHT_BRAKE_ON: {
                sendBoolToArduino(pS->datum[count].value, "041", valPort);
                cout << "Brake light: " << pS->datum[count].value << endl;
                break;
              }
              case DATA_LIGHT_NAV_ON: {
                sendBoolToArduino(pS->datum[count].value, "042", valPort);
                cout << "Nav light: " << pS->datum[count].value << endl;
                break;
              }
              case DATA_LIGHT_BEACON_ON: {
                sendBoolToArduino(pS->datum[count].value, "043", valPort);
                cout << "Beacon light: " << pS->datum[count].value << endl;
                break;
              }
              case DATA_LIGHT_LANDING_ON: {
                sendBoolToArduino(pS->datum[count].value, "044", valPort);
                cout << "Landing light: " << pS->datum[count].value << endl;
                break;
              }

              // warnings
              case DATA_STALL_WARNING: {
                sendBoolToArduino(pS->datum[count].value, "333", valPort);
                break;
              }
              case DATA_OVERSPEED_WARNING: {
                sendBoolToArduino(pS->datum[count].value, "334", valPort);
                break;
              }

              // Gear
              case DATA_GEAR_HANDLE_POSITION: {
                sendBoolToArduino(pS->datum[count].value, "526", valPort);
                break;
              }
              case DATA_GEAR_HYDRAULIC_PRESSURE: {
                sendLengthToArduino(pS->datum[count].value, "527", valPort, 6);
                break;
              }
              case DATA_TAILWHEEL_LOCK_ON: {
                sendBoolToArduino(pS->datum[count].value, "528", valPort);
                break;
              }
              case DATA_GEAR_CENTER_POSITION: {
                sendLengthToArduino(pS->datum[count].value * 100, "529",
                                    valPort, 3);
                break;
              }
              case DATA_GEAR_LEFT_POSITION: {
                sendLengthToArduino(pS->datum[count].value * 100, "530",
                                    valPort, 3);
                break;
              }
              case DATA_GEAR_RIGHT_POSITION: {
                sendLengthToArduino(pS->datum[count].value * 100, "531",
                                    valPort, 3);
                break;
              }
              case DATA_GEAR_TAIL_POSITION: {
                sendLengthToArduino(pS->datum[count].value * 100, "532",
                                    valPort, 3);
                break;
              }
              case DATA_GEAR_AUX_POSITION: {
                sendLengthToArduino(pS->datum[count].value * 100, "533",
                                    valPort, 3);
                break;
              }
              case DATA_GEAR_TOTAL_PCT_EXTENDED: {
                sendLengthToArduino(pS->datum[count].value * 100, "536",
                                    valPort, 3);
                break;
              }

              // Flaps
              case DATA_FLAPS_HANDLE_PERCENT: {
                sendLengthToArduino(pS->datum[count].value * 100, "510",
                                    valPort, 3);
                break;
              }
              case DATA_FLAPS_HANDLE_INDEX: {
                sendLengthToArduino(pS->datum[count].value, "511", valPort, 3);
                break;
              }
              case DATA_FLAPS_NUM_HANDLE_POSITIONS: {
                sendLengthToArduino(pS->datum[count].value, "512", valPort, 3);
                break;
              }
              case DATA_TRAILING_EDGE_FLAPS_LEFT_PERCENT: {
                sendLengthToArduino(pS->datum[count].value * 100, "513",
                                    valPort, 3);
                break;
              }
              case DATA_TRAILING_EDGE_FLAPS_RIGHT_PERCENT: {
                sendLengthToArduino(pS->datum[count].value * 100, "514",
                                    valPort, 3);
                break;
              }
              case DATA_TRAILING_EDGE_FLAPS_LEFT_ANGLE: {
                sendLengthToArduino(radianToDegree(pS->datum[count].value),
                                    "515", valPort, 3);
                break;
              }
              case DATA_TRAILING_EDGE_FLAPS_RIGHT_ANGLE: {
                sendLengthToArduino(radianToDegree(pS->datum[count].value),
                                    "516", valPort, 3);
                break;
              }
              case DATA_LEADING_EDGE_FLAPS_LEFT_PERCENT: {
                sendLengthToArduino(pS->datum[count].value * 100, "517",
                                    valPort, 3);
                break;
              }
              case DATA_LEADING_EDGE_FLAPS_RIGHT_PERCENT: {
                sendLengthToArduino(pS->datum[count].value * 100, "518",
                                    valPort, 3);
                break;
              }
              case DATA_LEADING_EDGE_FLAPS_LEFT_ANGLE: {
                sendLengthToArduino(radianToDegree(pS->datum[count].value),
                                    "519", valPort, 3);
                break;
              }
              case DATA_LEADING_EDGE_FLAPS_RIGHT_ANGLE: {
                sendLengthToArduino(radianToDegree(pS->datum[count].value),
                                    "520", valPort, 3);
                break;
              }

              // Rudder trim
              case DATA_ELEVATOR_TRIM_POSITION: {
                sendFloatToArduino(pS->datum[count].value * 100, "498",
                                   valPort);
                break;
              }
              case DATA_ELEVATOR_TRIM_PCT: {
                sendLengthToArduino(pS->datum[count].value * 100, "500",
                                    valPort, 3);
                break;
              }
              case DATA_AILERON_TRIM: {
                sendFloatToArduino(radianToDegreeFloat(pS->datum[count].value),
                                   "562", valPort);
                break;
              }
              case DATA_AILERON_TRIM_PCT: {
                sendLengthToArduino(pS->datum[count].value * 100, "563",
                                    valPort, 3);
                break;
              }
              case DATA_RUDDER_TRIM: {
                sendFloatToArduino(radianToDegreeFloat(pS->datum[count].value),
                                   "566", valPort);
                break;
              }
              case DATA_RUDDER_TRIM_PCT: {
                sendLengthToArduino(pS->datum[count].value * 100, "567",
                                    valPort, 3);
                break;
              }

              // AP
              case DATA_AUTOPILOT_AVAILABLE: {
                sendBoolToArduino(pS->datum[count].value, "576", valPort);
                break;
              }
              case DATA_AUTOPILOT_MASTER: {
                sendBoolToArduino(pS->datum[count].value, "577", valPort);
                break;
              }
              case DATA_AUTOPILOT_WING_LEVELER: {
                sendBoolToArduino(pS->datum[count].value, "579", valPort);
                break;
              }
              case DATA_AUTOPILOT_NAV1_LOCK: {
                sendBoolToArduino(pS->datum[count].value, "580", valPort);
                break;
              }
              case DATA_AUTOPILOT_HEADING_LOCK: {
                sendBoolToArduino(pS->datum[count].value, "581", valPort);
                break;
              }
              case DATA_AUTOPILOT_ALTITUDE_LOCK: {
                sendBoolToArduino(pS->datum[count].value, "583", valPort);
                break;
              }
              case DATA_AUTOPILOT_ATTITUDE_HOLD: {
                sendBoolToArduino(pS->datum[count].value, "585", valPort);
                break;
              }
              case DATA_AUTOPILOT_GLIDESLOPE_HOLD: {
                sendBoolToArduino(pS->datum[count].value, "586", valPort);
                break;
              }
              case DATA_AUTOPILOT_APPROACH_HOLD: {
                sendBoolToArduino(pS->datum[count].value, "588", valPort);
                break;
              }
              case DATA_AUTOPILOT_BACKCOURSE_HOLD: {
                sendBoolToArduino(pS->datum[count].value, "589", valPort);
                break;
              }
              case DATA_AUTOPILOT_FLIGHT_DIRECTOR_ACTIVE: {
                sendBoolToArduino(pS->datum[count].value, "591", valPort);
                break;
              }
              case DATA_AUTOPILOT_AIRSPEED_HOLD: {
                sendBoolToArduino(pS->datum[count].value, "594", valPort);
                break;
              }
              case DATA_AUTOPILOT_MACH_HOLD: {
                sendBoolToArduino(pS->datum[count].value, "596", valPort);
                break;
              }
              case DATA_AUTOPILOT_YAW_DAMPER: {
                sendBoolToArduino(pS->datum[count].value, "598", valPort);
                break;
              }
              case DATA_AUTOPILOT_THROTTLE_ARM: {
                sendBoolToArduino(pS->datum[count].value, "600", valPort);
                break;
              }
              case DATA_AUTOPILOT_TAKEOFF_POWER_ACTIVE: {
                sendBoolToArduino(pS->datum[count].value, "601", valPort);
                break;
              }
              case DATA_AUTOTHROTTLE_ACTIVE: {
                sendBoolToArduino(pS->datum[count].value, "602", valPort);
                break;
              }
              case DATA_AUTOPILOT_VERTICAL_HOLD: {
                sendBoolToArduino(pS->datum[count].value, "604", valPort);
                break;
              }
              case DATA_AUTOPILOT_RPM_HOLD: {
                sendBoolToArduino(pS->datum[count].value, "605", valPort);
                break;
              }

              default:
                printf("\nUnknown datum ID: %i", pS->datum[count].id);
                break;
            }
            ++count;
          }
          break;
        }
        default:
          break;
      }
      break;
    }

    case SIMCONNECT_RECV_ID_QUIT: {
      // quit = 1;
      break;
    }

    default:
      printf("\n Unknown dwID: %d", pData->dwID);
      cout << pData->dwVersion << endl;
      break;
  }
}
void ConnectWorker::testDataRequest() {
  HRESULT hr;

  if (SUCCEEDED(
          SimConnect_Open(&hSimConnect, "Tagged Data", NULL, 0, nullptr, 0))) {
    printf("\nConnected to Flight Simulator!");

    emit updateLastValUI("Connected to the game");
    // Set up the data definition, ensuring that all the elements are in Float32
    // units, to match the StructDatum structure The number of entries in the
    // DEFINITION_PDR definition should be equal to the maxReturnedItems define

    // Ap

    if (cbAPHeadingLock) {
      hr = SimConnect_AddToDataDefinition(
          hSimConnect, DEFINITION_PDR, "AUTOPILOT HEADING LOCK DIR", "Degrees",
          SIMCONNECT_DATATYPE_FLOAT32, 0, DATA_HEADING_LOCK);
    }
    if (cbAPAltitudeLock) {
      hr = SimConnect_AddToDataDefinition(
          hSimConnect, DEFINITION_PDR, "AUTOPILOT ALTITUDE LOCK VAR", "Feet",
          SIMCONNECT_DATATYPE_FLOAT32, 0, DATA_ALTITUDE_TARGET);
    }
    if (cbAPVerticalLock) {
      hr = SimConnect_AddToDataDefinition(
          hSimConnect, DEFINITION_PDR, "AUTOPILOT VERTICAL HOLD VAR",
          "Feet/minute", SIMCONNECT_DATATYPE_FLOAT32, 0, DATA_VERTICAL_SPEED);
    }
    if (cbBarometerPressure) {
      hr = SimConnect_AddToDataDefinition(
          hSimConnect, DEFINITION_PDR, "BAROMETER PRESSURE", "Millibars",
          SIMCONNECT_DATATYPE_FLOAT32, 10, DATA_BAROMETER);
    }
    if (cbIndicatedVerticalSpeed) {
      hr = SimConnect_AddToDataDefinition(
          hSimConnect, DEFINITION_PDR, "VERTICAL SPEED", "Feet per second",
          SIMCONNECT_DATATYPE_FLOAT32, 10, DATA_TRUE_VERTICAL_SPEED);
    }
    if (cbIndicatedAirspeed) {
      hr = SimConnect_AddToDataDefinition(
          hSimConnect, DEFINITION_PDR, "AIRSPEED INDICATED", "Knots",
          SIMCONNECT_DATATYPE_FLOAT32, 1, DATA_INDICATED_AIRSPEED);
    }
    if (cbGPSGroundspeed) {
      hr = SimConnect_AddToDataDefinition(
          hSimConnect, DEFINITION_PDR, "GPS GROUND SPEED", "Meters per second",
          SIMCONNECT_DATATYPE_FLOAT32, 1, DATA_GPS_GROUNDSPEED);
    }
    if (cbIndicatedHeading) {
      hr = SimConnect_AddToDataDefinition(
          hSimConnect, DEFINITION_PDR, "HEADING INDICATOR", "Radians",
          SIMCONNECT_DATATYPE_FLOAT32, 0.02, DATA_INDICATED_HEADING);
    }
    if (cbIndicatedAltitude) {
      hr = SimConnect_AddToDataDefinition(
          hSimConnect, DEFINITION_PDR, "INDICATED ALTITUDE", "Feet",
          SIMCONNECT_DATATYPE_FLOAT32, 10, DATA_INDICATED_ALTITUDE);
    }
    if (cbSelectedQuantityPercent) {
      hr = SimConnect_AddToDataDefinition(
          hSimConnect, DEFINITION_PDR, "FUEL SELECTED QUANTITY PERCENT",
          "Percent Over 100", SIMCONNECT_DATATYPE_FLOAT32, 0.01,
          DATA_SELECTED_QUANTITY_FUEL);
    }

    // coms
    if (cbActiveCom1) {
      hr = SimConnect_AddToDataDefinition(
          hSimConnect, DEFINITION_PDR, "COM ACTIVE FREQUENCY:1", "Khz",
          SIMCONNECT_DATATYPE_FLOAT32, 0, DATA_COM_FREQ1);
    }
    if (cbActiveCom2) {
      hr = SimConnect_AddToDataDefinition(
          hSimConnect, DEFINITION_PDR, "COM ACTIVE FREQUENCY:2", "Khz",
          SIMCONNECT_DATATYPE_FLOAT32, 0, DATA_COM_FREQ2);
    }
    if (cbKohlman) {
      hr = SimConnect_AddToDataDefinition(
          hSimConnect, DEFINITION_PDR, "KOHLSMAN SETTING HG", "inHg",
          SIMCONNECT_DATATYPE_FLOAT32, 0, DATA_COM_ALTMTR);
    }
    if (cbStandbyCom1) {
      hr = SimConnect_AddToDataDefinition(
          hSimConnect, DEFINITION_PDR, "COM STANDBY FREQUENCY:1", "Khz",
          SIMCONNECT_DATATYPE_FLOAT32, 0, DATA_COM_FREQ_STANDBY1);
    }
    if (cbStandbyCom2) {
      hr = SimConnect_AddToDataDefinition(
          hSimConnect, DEFINITION_PDR, "COM STANDBY FREQUENCY:2", "Khz",
          SIMCONNECT_DATATYPE_FLOAT32, 0, DATA_COM_FREQ_STANDBY2);
    }
    if (cbNavActiveFrequency1) {
      hr = SimConnect_AddToDataDefinition(
          hSimConnect, DEFINITION_PDR, "NAV ACTIVE FREQUENCY:1", "MHz",
          SIMCONNECT_DATATYPE_FLOAT32, 0, DATA_NAV_ACTIVE_FREQUENCY1);
    }
    if (cbNavStandbyFrequency1) {
      hr = SimConnect_AddToDataDefinition(
          hSimConnect, DEFINITION_PDR, "NAV STANDBY FREQUENCY:1", "MHz",
          SIMCONNECT_DATATYPE_FLOAT32, 0, DATA_NAV_STANDBY_FREQUENCY1);
    }
    if (cbNavActiveFrequency2) {
      hr = SimConnect_AddToDataDefinition(
          hSimConnect, DEFINITION_PDR, "NAV ACTIVE FREQUENCY:2", "MHz",
          SIMCONNECT_DATATYPE_FLOAT32, 0, DATA_NAV_ACTIVE_FREQUENCY2);
    }
    if (cbNavStandbyFrequency2) {
      hr = SimConnect_AddToDataDefinition(
          hSimConnect, DEFINITION_PDR, "NAV STANDBY FREQUENCY:2", "MHz",
          SIMCONNECT_DATATYPE_FLOAT32, 0, DATA_NAV_STANDBY_FREQUENCY2);
    }
    //
    if (cbNavRadialError1) {
      hr = SimConnect_AddToDataDefinition(
          hSimConnect, DEFINITION_PDR, "NAV RADIAL ERROR:1", "Degrees",
          SIMCONNECT_DATATYPE_FLOAT32, 0, DATA_NAV_RADIAL_ERROR1);
    }
    if (cbNavVorLatlonalt1) {
      hr = SimConnect_AddToDataDefinition(
          hSimConnect, DEFINITION_PDR, "NAV VOR LATLONALT:1",
          "SIMCONNECT_DATA_LATLONALT", SIMCONNECT_DATATYPE_LATLONALT, 0,
          DATA_NAV_VOR_LATLONALT1);
    }

    // ADF
    if (cbAdfActiveFrequency1) {
      hr = SimConnect_AddToDataDefinition(
          hSimConnect, DEFINITION_PDR, "ADF ACTIVE FREQUENCY:1", "Hz",
          SIMCONNECT_DATATYPE_FLOAT32, 0, DATA_ADF_ACTIVE_FREQUENCY1);
    }
    if (cbAdfStandbyFrequency1) {
      hr = SimConnect_AddToDataDefinition(
          hSimConnect, DEFINITION_PDR, "ADF STANDBY FREQUENCY:1", "Hz",
          SIMCONNECT_DATATYPE_FLOAT32, 0, DATA_ADF_STANDBY_FREQUENCY1);
    }
    if (cbAdfRadial1) {
      hr = SimConnect_AddToDataDefinition(
          hSimConnect, DEFINITION_PDR, "ADF RADIAL:1", "Degrees",
          SIMCONNECT_DATATYPE_FLOAT32, 0, DATA_ADF_RADIAL1);
    }
    if (cbAdfSignal1) {
      hr = SimConnect_AddToDataDefinition(
          hSimConnect, DEFINITION_PDR, "ADF SIGNAL:1", "Number",
          SIMCONNECT_DATATYPE_FLOAT32, 0, DATA_ADF_SIGNAL1);
    }

    if (cbAdfActiveFrequency2) {
      hr = SimConnect_AddToDataDefinition(
          hSimConnect, DEFINITION_PDR, "ADF ACTIVE FREQUENCY:2", "Hz",
          SIMCONNECT_DATATYPE_FLOAT32, 0, DATA_ADF_ACTIVE_FREQUENCY2);
    }
    if (cbAdfStandbyFrequency2) {
      hr = SimConnect_AddToDataDefinition(
          hSimConnect, DEFINITION_PDR, "ADF STANDBY FREQUENCY:2", "Khz",
          SIMCONNECT_DATATYPE_FLOAT32, 0, DATA_ADF_STANDBY_FREQUENCY2);
    }
    if (cbAdfRadial2) {
      hr = SimConnect_AddToDataDefinition(
          hSimConnect, DEFINITION_PDR, "ADF RADIAL:2", "Degrees",
          SIMCONNECT_DATATYPE_FLOAT32, 0, DATA_ADF_RADIAL2);
    }
    if (cbAdfSignal2) {
      hr = SimConnect_AddToDataDefinition(
          hSimConnect, DEFINITION_PDR, "ADF SIGNAL:2", "Number",
          SIMCONNECT_DATATYPE_FLOAT32, 0, DATA_ADF_SIGNAL2);
    }

    // DME
    if (cbNavDme1) {
      hr = SimConnect_AddToDataDefinition(
          hSimConnect, DEFINITION_PDR, "NAV DME:1", "Nautical miles",
          SIMCONNECT_DATATYPE_FLOAT32, 0, DATA_NAV_DME1);
    }
    if (cbNavDmespeed1) {
      hr = SimConnect_AddToDataDefinition(
          hSimConnect, DEFINITION_PDR, "NAV DMESPEED:1", "Knots",
          SIMCONNECT_DATATYPE_FLOAT32, 0, DATA_NAV_DMESPEED1);
    }

    if (cbNavDme2) {
      hr = SimConnect_AddToDataDefinition(
          hSimConnect, DEFINITION_PDR, "NAV DME:2", "Nautical miles",
          SIMCONNECT_DATATYPE_FLOAT32, 0, DATA_NAV_DME2);
    }
    if (cbNavDmespeed1) {
      hr = SimConnect_AddToDataDefinition(
          hSimConnect, DEFINITION_PDR, "NAV DMESPEED:2", "Knots",
          SIMCONNECT_DATATYPE_FLOAT32, 0, DATA_NAV_DMESPEED2);
    }

    // Transponder
    if (cbTransponderCode1) {
      hr = SimConnect_AddToDataDefinition(
          hSimConnect, DEFINITION_PDR, "TRANSPONDER CODE:1", "number",
          SIMCONNECT_DATATYPE_FLOAT32, 0, DATA_TRANSPONDER_CODE1);
    }
    if (cbTransponderCode2) {
      hr = SimConnect_AddToDataDefinition(
          hSimConnect, DEFINITION_PDR, "TRANSPONDER CODE:2", "number",
          SIMCONNECT_DATATYPE_FLOAT32, 0, DATA_TRANSPONDER_CODE2);
    }

    // lights
    if (cbLightTaxiOn) {
      hr = SimConnect_AddToDataDefinition(
          hSimConnect, DEFINITION_PDR, "LIGHT TAXI ON", "Bool",
          SIMCONNECT_DATATYPE_INT32, 0, DATA_LIGHT_TAXI_ON);
    }
    if (cbLightStrobeOn) {
      hr = SimConnect_AddToDataDefinition(
          hSimConnect, DEFINITION_PDR, "LIGHT STROBE ON", "Bool",
          SIMCONNECT_DATATYPE_INT32, 0, DATA_LIGHT_STROBE_ON);
    }
    if (cbLightPanelOn) {
      hr = SimConnect_AddToDataDefinition(
          hSimConnect, DEFINITION_PDR, "LIGHT PANEL ON", "Bool",
          SIMCONNECT_DATATYPE_INT32, 0, DATA_LIGHT_PANEL_ON);
    }
    if (cbLightRecognitionOn) {
      hr = SimConnect_AddToDataDefinition(
          hSimConnect, DEFINITION_PDR, "LIGHT RECOGNITION ON", "Bool",
          SIMCONNECT_DATATYPE_INT32, 0, DATA_LIGHT_RECOGNITION_ON);
    }
    if (cbLightWingOn) {
      hr = SimConnect_AddToDataDefinition(
          hSimConnect, DEFINITION_PDR, "LIGHT WING ON", "Bool",
          SIMCONNECT_DATATYPE_INT32, 0, DATA_LIGHT_WING_ON);
    }
    if (cbLightLogoOn) {
      hr = SimConnect_AddToDataDefinition(
          hSimConnect, DEFINITION_PDR, "LIGHT LOGO ON", "Bool",
          SIMCONNECT_DATATYPE_INT32, 0, DATA_LIGHT_LOGO_ON);
    }
    if (cbLightCabinOn) {
      hr = SimConnect_AddToDataDefinition(
          hSimConnect, DEFINITION_PDR, "LIGHT CABIN ON", "Bool",
          SIMCONNECT_DATATYPE_INT32, 0, DATA_LIGHT_CABIN_ON);
    }
    if (cbLightHeadOn) {
      hr = SimConnect_AddToDataDefinition(
          hSimConnect, DEFINITION_PDR, "LIGHT HEAD ON", "Bool",
          SIMCONNECT_DATATYPE_INT32, 0, DATA_LIGHT_HEAD_ON);
    }
    if (cbLightBrakeOn) {
      hr = SimConnect_AddToDataDefinition(
          hSimConnect, DEFINITION_PDR, "LIGHT BRAKE ON", "Bool",
          SIMCONNECT_DATATYPE_INT32, 0, DATA_LIGHT_BRAKE_ON);
    }
    if (cbLightNavOn) {
      hr = SimConnect_AddToDataDefinition(
          hSimConnect, DEFINITION_PDR, "LIGHT NAV ON", "Bool",
          SIMCONNECT_DATATYPE_INT32, 0, DATA_LIGHT_NAV_ON);
    }
    if (cbLightBeaconOn) {
      hr = SimConnect_AddToDataDefinition(
          hSimConnect, DEFINITION_PDR, "LIGHT BEACON ON", "Bool",
          SIMCONNECT_DATATYPE_INT32, 0, DATA_LIGHT_BEACON_ON);
    }
    if (cbLightLandingOn) {
      hr = SimConnect_AddToDataDefinition(
          hSimConnect, DEFINITION_PDR, "LIGHT LANDING ON", "Bool",
          SIMCONNECT_DATATYPE_INT32, 0, DATA_LIGHT_LANDING_ON);
    }

    // warnings
    if (cbStallWarning) {
      hr = SimConnect_AddToDataDefinition(
          hSimConnect, DEFINITION_PDR, "STALL WARNING", "Bool",
          SIMCONNECT_DATATYPE_INT32, 0, DATA_STALL_WARNING);
    }
    if (cbOverspeedWarning) {
      hr = SimConnect_AddToDataDefinition(
          hSimConnect, DEFINITION_PDR, "OVERSPEED WARNING", "Bool",
          SIMCONNECT_DATATYPE_INT32, 0, DATA_OVERSPEED_WARNING);
    }

    // Rudder/trim
    if (cbAileronTrim) {
      hr = SimConnect_AddToDataDefinition(
          hSimConnect, DEFINITION_PDR, "AILERON TRIM", "Radians",
          SIMCONNECT_DATATYPE_FLOAT32, 0, DATA_AILERON_TRIM);
    }
    if (cbAileronTrimPct) {
      hr = SimConnect_AddToDataDefinition(
          hSimConnect, DEFINITION_PDR, "AILERON TRIM PCT", "Percent Over 100",
          SIMCONNECT_DATATYPE_FLOAT32, 0.01, DATA_AILERON_TRIM_PCT);
    }
    if (cbRudderTrim) {
      hr = SimConnect_AddToDataDefinition(
          hSimConnect, DEFINITION_PDR, "RUDDER TRIM", "Radians",
          SIMCONNECT_DATATYPE_FLOAT32, 0.01, DATA_RUDDER_TRIM);
    }
    if (cbRudderTrimPct) {
      hr = SimConnect_AddToDataDefinition(
          hSimConnect, DEFINITION_PDR, "RUDDER TRIM PCT", "Percent Over 100",
          SIMCONNECT_DATATYPE_FLOAT32, 0.01, DATA_RUDDER_TRIM_PCT);
    }
    if (cbAileronTrimPct) {
      hr = SimConnect_AddToDataDefinition(
          hSimConnect, DEFINITION_PDR, "AILERON TRIM PCT",
          "SIMCONNECT_DATA_XYZ", SIMCONNECT_DATATYPE_FLOAT32, 0.01,
          DATA_AILERON_TRIM_PCT);
    }
    if (cbRudderTrimPct) {
      hr = SimConnect_AddToDataDefinition(
          hSimConnect, DEFINITION_PDR, "RUDDER TRIM PCT", "Percent over 100",
          SIMCONNECT_DATATYPE_FLOAT32, 0.01, DATA_RUDDER_TRIM_PCT);
    }
    if (cbElevatorTrimPosition) {
      hr = SimConnect_AddToDataDefinition(
          hSimConnect, DEFINITION_PDR, "ELEVATOR TRIM POSITION", "Radians",
          SIMCONNECT_DATATYPE_FLOAT32, 0.01, DATA_ELEVATOR_TRIM_POSITION);
    }
    if (cbElevatorTrimPct) {
      hr = SimConnect_AddToDataDefinition(
          hSimConnect, DEFINITION_PDR, "ELEVATOR TRIM PCT", "Percent Over 100",
          SIMCONNECT_DATATYPE_FLOAT32, 0.01, DATA_ELEVATOR_TRIM_PCT);
    }

    // Flaps
    if (cbFlapsHandlePercent) {
      hr = SimConnect_AddToDataDefinition(
          hSimConnect, DEFINITION_PDR, "FLAPS HANDLE PERCENT",
          "Percent Over 100", SIMCONNECT_DATATYPE_FLOAT32, 0.01,
          DATA_FLAPS_HANDLE_PERCENT);
    }
    if (cbFlapsHandleIndex) {
      hr = SimConnect_AddToDataDefinition(
          hSimConnect, DEFINITION_PDR, "FLAPS HANDLE INDEX", "Number",
          SIMCONNECT_DATATYPE_FLOAT32, 0, DATA_FLAPS_HANDLE_INDEX);
    }
    if (cbFlapsNumHandlePositions) {
      hr = SimConnect_AddToDataDefinition(
          hSimConnect, DEFINITION_PDR, "FLAPS NUM HANDLE POSITIONS", "Number",
          SIMCONNECT_DATATYPE_FLOAT32, 0, DATA_FLAPS_NUM_HANDLE_POSITIONS);
    }
    if (cbTrailingEdgeFlapsLeftPercent) {
      hr = SimConnect_AddToDataDefinition(
          hSimConnect, DEFINITION_PDR, "TRAILING EDGE FLAPS LEFT PERCENT",
          "Percent Over 100", SIMCONNECT_DATATYPE_FLOAT32, 0.01,
          DATA_TRAILING_EDGE_FLAPS_LEFT_PERCENT);
    }
    if (cbTrailingEdgeFlapsRightPercent) {
      hr = SimConnect_AddToDataDefinition(
          hSimConnect, DEFINITION_PDR, "TRAILING EDGE FLAPS RIGHT PERCENT",
          "Percent Over 100", SIMCONNECT_DATATYPE_FLOAT32, 0.01,
          DATA_TRAILING_EDGE_FLAPS_RIGHT_PERCENT);
    }
    if (cbTrailingEdgeFlapsLeftAngle) {
      hr = SimConnect_AddToDataDefinition(
          hSimConnect, DEFINITION_PDR, "TRAILING EDGE FLAPS LEFT ANGLE",
          "Radians", SIMCONNECT_DATATYPE_FLOAT32, 0.01,
          DATA_TRAILING_EDGE_FLAPS_LEFT_ANGLE);
    }
    if (cbTrailingEdgeFlapsRightAngle) {
      hr = SimConnect_AddToDataDefinition(
          hSimConnect, DEFINITION_PDR, "TRAILING EDGE FLAPS RIGHT ANGLE",
          "Radians", SIMCONNECT_DATATYPE_FLOAT32, 0.01,
          DATA_TRAILING_EDGE_FLAPS_RIGHT_ANGLE);
    }
    if (cbLeadingEdgeFlapsLeftPercent) {
      hr = SimConnect_AddToDataDefinition(
          hSimConnect, DEFINITION_PDR, "LEADING EDGE FLAPS LEFT PERCENT",
          "Percent Over 100", SIMCONNECT_DATATYPE_FLOAT32, 0.01,
          DATA_LEADING_EDGE_FLAPS_LEFT_PERCENT);
    }
    if (cbLeadingEdgeFlapsRightPercent) {
      hr = SimConnect_AddToDataDefinition(
          hSimConnect, DEFINITION_PDR, "LEADING EDGE FLAPS RIGHT PERCENT",
          "Percent Over 100", SIMCONNECT_DATATYPE_FLOAT32, 0.01,
          DATA_LEADING_EDGE_FLAPS_RIGHT_PERCENT);
    }
    if (cbLeadingEdgeFlapsLeftAngle) {
      hr = SimConnect_AddToDataDefinition(
          hSimConnect, DEFINITION_PDR, "LEADING EDGE FLAPS LEFT ANGLE",
          "Radians", SIMCONNECT_DATATYPE_FLOAT32, 0.01,
          DATA_LEADING_EDGE_FLAPS_LEFT_ANGLE);
    }
    if (cbLeadingEdgeFlapsRightAngle) {
      hr = SimConnect_AddToDataDefinition(
          hSimConnect, DEFINITION_PDR, "LEADING EDGE FLAPS RIGHT ANGLE",
          "Radians", SIMCONNECT_DATATYPE_FLOAT32, 0.01,
          DATA_LEADING_EDGE_FLAPS_RIGHT_ANGLE);
    }

    // Gear
    if (cbGearHandlePosition) {
      hr = SimConnect_AddToDataDefinition(
          hSimConnect, DEFINITION_PDR, "GEAR HANDLE POSITION", "Bool",
          SIMCONNECT_DATATYPE_INT32, 0, DATA_GEAR_HANDLE_POSITION);
    }
    if (cbGearHydraulicPressure) {
      hr = SimConnect_AddToDataDefinition(
          hSimConnect, DEFINITION_PDR, "GEAR HYDRAULIC PRESSURE", "psf",
          SIMCONNECT_DATATYPE_FLOAT32, 1, DATA_GEAR_HYDRAULIC_PRESSURE);
    }
    if (cbTailwheelLockOn) {
      hr = SimConnect_AddToDataDefinition(
          hSimConnect, DEFINITION_PDR, "TAILWHEEL LOCK ON", "Bool",
          SIMCONNECT_DATATYPE_INT32, 0, DATA_TAILWHEEL_LOCK_ON);
    }
    if (cbGearCenterPosition) {
      hr = SimConnect_AddToDataDefinition(
          hSimConnect, DEFINITION_PDR, "GEAR CENTER POSITION",
          "Percent Over 100", SIMCONNECT_DATATYPE_FLOAT32, 0.01,
          DATA_GEAR_CENTER_POSITION);
    }
    if (cbGearLeftPosition) {
      hr = SimConnect_AddToDataDefinition(
          hSimConnect, DEFINITION_PDR, "GEAR LEFT POSITION", "Percent Over 100",
          SIMCONNECT_DATATYPE_FLOAT32, 0.01, DATA_GEAR_LEFT_POSITION);
    }
    if (cbGearRightPosition) {
      hr = SimConnect_AddToDataDefinition(
          hSimConnect, DEFINITION_PDR, "GEAR RIGHT POSITION",
          "Percent Over 100", SIMCONNECT_DATATYPE_FLOAT32, 0.01,
          DATA_GEAR_RIGHT_POSITION);
    }
    if (cbGearTailPosition) {
      hr = SimConnect_AddToDataDefinition(
          hSimConnect, DEFINITION_PDR, "GEAR TAIL POSITION", "Percent Over 100",
          SIMCONNECT_DATATYPE_FLOAT32, 0.01, DATA_GEAR_TAIL_POSITION);
    }
    if (cbGearAuxPosition) {
      hr = SimConnect_AddToDataDefinition(
          hSimConnect, DEFINITION_PDR, "GEAR AUX POSITION", "Percent Over 100",
          SIMCONNECT_DATATYPE_FLOAT32, 0.01, DATA_GEAR_AUX_POSITION);
    }
    if (cbGearTotalPctExtended) {
      hr = SimConnect_AddToDataDefinition(
          hSimConnect, DEFINITION_PDR, "GEAR TOTAL PCT EXTENDED", "Percentage",
          SIMCONNECT_DATATYPE_FLOAT32, 0.01, DATA_GEAR_TOTAL_PCT_EXTENDED);
    }

    // AP
    if (cbAutopilotAvailable) {
      hr = SimConnect_AddToDataDefinition(
          hSimConnect, DEFINITION_PDR, "AUTOPILOT AVAILABLE", "Bool",
          SIMCONNECT_DATATYPE_INT32, 0, DATA_AUTOPILOT_AVAILABLE);
    }
    if (cbAutopilotMaster) {
      hr = SimConnect_AddToDataDefinition(
          hSimConnect, DEFINITION_PDR, "AUTOPILOT MASTER", "Bool",
          SIMCONNECT_DATATYPE_INT32, 0, DATA_AUTOPILOT_MASTER);
    }
    if (cbAutopilotWingLeveler) {
      hr = SimConnect_AddToDataDefinition(
          hSimConnect, DEFINITION_PDR, "AUTOPILOT WING LEVELER", "Bool",
          SIMCONNECT_DATATYPE_INT32, 0, DATA_AUTOPILOT_WING_LEVELER);
    }
    if (cbAutopilotNav1Lock) {
      hr = SimConnect_AddToDataDefinition(
          hSimConnect, DEFINITION_PDR, "AUTOPILOT NAV1 LOCK", "Bool",
          SIMCONNECT_DATATYPE_INT32, 0, DATA_AUTOPILOT_NAV1_LOCK);
    }
    if (cbAutopilotHeadingLock) {
      hr = SimConnect_AddToDataDefinition(
          hSimConnect, DEFINITION_PDR, "AUTOPILOT HEADING LOCK", "Bool",
          SIMCONNECT_DATATYPE_INT32, 0, DATA_AUTOPILOT_HEADING_LOCK);
    }
    if (cbAutopilotAltitudeLock) {
      hr = SimConnect_AddToDataDefinition(
          hSimConnect, DEFINITION_PDR, "AUTOPILOT ALTITUDE LOCK", "Bool",
          SIMCONNECT_DATATYPE_INT32, 0, DATA_AUTOPILOT_ALTITUDE_LOCK);
    }
    if (cbAutopilotAttitudeHold) {
      hr = SimConnect_AddToDataDefinition(
          hSimConnect, DEFINITION_PDR, "AUTOPILOT ATTITUDE HOLD", "Bool",
          SIMCONNECT_DATATYPE_INT32, 0, DATA_AUTOPILOT_ATTITUDE_HOLD);
    }
    if (cbAutopilotGlideslopeHold) {
      hr = SimConnect_AddToDataDefinition(
          hSimConnect, DEFINITION_PDR, "AUTOPILOT GLIDESLOPE HOLD", "Bool",
          SIMCONNECT_DATATYPE_INT32, 0, DATA_AUTOPILOT_GLIDESLOPE_HOLD);
    }
    if (cbAutopilotApproachHold) {
      hr = SimConnect_AddToDataDefinition(
          hSimConnect, DEFINITION_PDR, "AUTOPILOT APPROACH HOLD", "Bool",
          SIMCONNECT_DATATYPE_INT32, 0, DATA_AUTOPILOT_APPROACH_HOLD);
    }
    if (cbAutopilotBackcourseHold) {
      hr = SimConnect_AddToDataDefinition(
          hSimConnect, DEFINITION_PDR, "AUTOPILOT BACKCOURSE HOLD", "Bool",
          SIMCONNECT_DATATYPE_INT32, 0, DATA_AUTOPILOT_BACKCOURSE_HOLD);
    }
    if (cbAutopilotFlightDirectorActive) {
      hr = SimConnect_AddToDataDefinition(
          hSimConnect, DEFINITION_PDR, "AUTOPILOT FLIGHT DIRECTOR ACTIVE",
          "Bool", SIMCONNECT_DATATYPE_INT32, 0,
          DATA_AUTOPILOT_FLIGHT_DIRECTOR_ACTIVE);
    }
    if (cbAutopilotAirspeedHold) {
      hr = SimConnect_AddToDataDefinition(
          hSimConnect, DEFINITION_PDR, "AUTOPILOT AIRSPEED HOLD", "Bool",
          SIMCONNECT_DATATYPE_INT32, 0, DATA_AUTOPILOT_AIRSPEED_HOLD);
    }
    if (cbAutopilotMachHold) {
      hr = SimConnect_AddToDataDefinition(
          hSimConnect, DEFINITION_PDR, "AUTOPILOT MACH HOLD", "Bool",
          SIMCONNECT_DATATYPE_INT32, 0, DATA_AUTOPILOT_MACH_HOLD);
    }
    if (cbAutopilotYawDamper) {
      hr = SimConnect_AddToDataDefinition(
          hSimConnect, DEFINITION_PDR, "AUTOPILOT YAW DAMPER", "Bool",
          SIMCONNECT_DATATYPE_INT32, 0, DATA_AUTOPILOT_YAW_DAMPER);
    }
    if (cbAutopilotThrottleArm) {
      hr = SimConnect_AddToDataDefinition(
          hSimConnect, DEFINITION_PDR, "AUTOPILOT THROTTLE ARM", "Bool",
          SIMCONNECT_DATATYPE_INT32, 0, DATA_AUTOPILOT_THROTTLE_ARM);
    }
    if (cbAutopilotTakeoffPowerActive) {
      hr = SimConnect_AddToDataDefinition(
          hSimConnect, DEFINITION_PDR, "AUTOPILOT TAKEOFF POWER ACTIVE", "Bool",
          SIMCONNECT_DATATYPE_INT32, 0, DATA_AUTOPILOT_TAKEOFF_POWER_ACTIVE);
    }
    if (cbAutothrottleActive) {
      hr = SimConnect_AddToDataDefinition(
          hSimConnect, DEFINITION_PDR, "AUTOTHROTTLE ACTIVE", "Bool",
          SIMCONNECT_DATATYPE_INT32, 0, DATA_AUTOTHROTTLE_ACTIVE);
    }
    if (cbAutopilotNav1Lock) {
      hr = SimConnect_AddToDataDefinition(
          hSimConnect, DEFINITION_PDR, "AUTOPILOT NAV1 LOCK", "Bool",
          SIMCONNECT_DATATYPE_INT32, 0, DATA_AUTOPILOT_NAV1_LOCK);
    }
    if (cbAutopilotVerticalHold) {
      hr = SimConnect_AddToDataDefinition(
          hSimConnect, DEFINITION_PDR, "AUTOPILOT VERTICAL HOLD", "Bool",
          SIMCONNECT_DATATYPE_INT32, 0, DATA_AUTOPILOT_VERTICAL_HOLD);
    }
    if (cbAutopilotRpmHold) {
      hr = SimConnect_AddToDataDefinition(
          hSimConnect, DEFINITION_PDR, "AUTOPILOT RPM HOLD", "Bool",
          SIMCONNECT_DATATYPE_INT32, 0, DATA_AUTOPILOT_RPM_HOLD);
    }
    // GPS
    if (cbGpsCourseToSteer) {
      hr = SimConnect_AddToDataDefinition(
          hSimConnect, DEFINITION_PDR, "GPS COURSE TO STEER", "Radians",
          SIMCONNECT_DATATYPE_FLOAT32, 0.01, DATA_GPS_COURSE_TO_STEER);
    }

    // PLANE DATA
    if (cbPlaneName) {
      hr = SimConnect_AddToDataDefinition(hSimConnect, DEFINITION_STRING,
                                          "TITLE", NULL,
                                          SIMCONNECT_DATATYPE_STRING256);
      strincProcessing = true;
    }

    // Request an event when the simulation starts

    hr =
        SimConnect_SubscribeToSystemEvent(hSimConnect, EVENT_SIM_START, "1sec");

    printf("\nLaunch a flight.");

    while (abort != true) {
      if (connectionError) {
        if (lastConnectionState != connectionError) {
          emit updateLastValUI("Error connecting to Arduino");
          lastConnectionState = connectionError;
        }
      } else {
        emit updateLastValUI("Succesfully connected to your Arduino");
      }
      SimConnect_CallDispatch(hSimConnect, MyDispatchProcRD, nullptr);
      Sleep(1);
      quit();
    }
    hr = SimConnect_Close(hSimConnect);
  }
}
ConnectWorker::~ConnectWorker() {
  lastConnectionState = false;
  connectionError = false;
  mutex.lock();
  abort = true;
  condition.wakeOne();
  mutex.unlock();
}
