#define DATA_LENGTH 255
#define MAX_RETURNED_ITEMS 255
#include <headers/ConnectWorker.h>

#include <qsettings.h>
#include <qstandardpaths.h>
#include <tchar.h>
#include <windows.h>

#include <string>

#include "stdio.h"
#include "strsafe.h"
#include <headers/SimConnect.h>

#include <outputs/outputmapper.h>
char output[DATA_LENGTH];
bool strincProcessing = false;
bool connectionError = false;
float prevSpeed = 0.0f;
float currentSpeed;
int eps = 1;
bool lastConnectionState = false;

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

  // DATA
  DATA_PLANE_NAME,
  DATA_PLANE_ALT_ABOVE_GROUND,
  DATA_SIM_ON_GROUND,

  DATA_NAV_OBS_INDEX1,
  DATA_NAV_OBS_INDEX2,

  DATA_AUTOPILOT,

  DATA_BRAKE_LEFT_POSITION,
  DATA_BRAKE_RIGHT_POSITION,
  DATA_BRAKE_INDICATOR,
  DATA_BRAKE_PARKING_POSITION,
  DATA_BRAKE_PARKING_INDICATOR,
  DATA_PITOT_HEAT,
  DATA_FUEL_TANK_CENTER_LEVEL,
  DATA_FUEL_TANK_CENTER2_LEVEL,
  DATA_FUEL_TANK_CENTER3_LEVEL,
  DATA_FUEL_TANK_LEFT_MAIN_LEVEL,
  DATA_FUEL_TANK_LEFT_AUX_LEVEL,
  DATA_FUEL_TANK_LEFT_TIP_LEVEL,
  DATA_FUEL_TANK_RIGHT_MAIN_LEVEL,
  DATA_FUEL_TANK_RIGHT_AUX_LEVEL,
  DATA_FUEL_TANK_RIGHT_TIP_LEVEL,
  DATA_FUEL_TANK_EXTERNAL1_LEVEL,
  DATA_FUEL_TANK_EXTERNAL2_LEVEL,
  DATA_FUEL_TANK_CENTER_CAPACITY,
  DATA_FUEL_TANK_CENTER2_CAPACITY,
  DATA_FUEL_TANK_CENTER3_CAPACITY,
  DATA_FUEL_TANK_LEFT_MAIN_CAPACITY,
  DATA_FUEL_TANK_LEFT_AUX_CAPACITY,
  DATA_FUEL_TANK_LEFT_TIP_CAPACITY,
  DATA_FUEL_TANK_RIGHT_MAIN_CAPACITY,
  DATA_FUEL_TANK_RIGHT_AUX_CAPACITY,
  DATA_FUEL_TANK_RIGHT_TIP_CAPACITY,
  DATA_FUEL_TANK_EXTERNAL1_CAPACITY,
  DATA_FUEL_TANK_EXTERNAL2_CAPACITY,
  DATA_FUEL_LEFT_CAPACITY,
  DATA_FUEL_RIGHT_CAPACITY,
  DATA_FUEL_TANK_CENTER_QUANTITY,
  DATA_FUEL_TANK_CENTER2_QUANTITY,
  DATA_FUEL_TANK_CENTER3_QUANTITY,
  DATA_FUEL_TANK_LEFT_MAIN_QUANTITY,
  DATA_FUEL_TANK_LEFT_AUX_QUANTITY,
  DATA_FUEL_TANK_LEFT_TIP_QUANTITY,
  DATA_FUEL_TANK_RIGHT_MAIN_QUANTITY,
  DATA_FUEL_TANK_RIGHT_AUX_QUANTITY,
  DATA_FUEL_TANK_RIGHT_TIP_QUANTITY,
  DATA_FUEL_TANK_EXTERNAL1_QUANTITY,
  DATA_FUEL_TANK_EXTERNAL2_QUANTITY,
  DATA_FUEL_LEFT_QUANTITY,
  DATA_FUEL_RIGHT_QUANTITY,
  DATA_FUEL_TOTAL_QUANTITY,

};

using namespace std;
SerialPort *arduinoTest;
ConnectWorker::ConnectWorker() {}

void sendToArduino(float received, std::string prefix) {
  auto intVal = static_cast<int>(received);

  if (!arduinoTest->isConnected()) {
    connectionError = true;
  } else {
    connectionError = false;
  }
  const auto value = intVal;

  auto input_string = prefix + std::to_string(value);
  cout << "size: " << input_string.size() << endl;
  auto *const c_string = new char[input_string.size() + 1];
  std::copy(input_string.begin(), input_string.end(), c_string);
  c_string[input_string.size()] = '\n';
  cout << strlen(c_string) << endl;
  cout << c_string << endl;
  arduinoTest->writeSerialPort(c_string, strlen(c_string));
  delete[] c_string;
}
void sendCharToArduino(const char *received, std::string prefix) {
  if (!arduinoTest->isConnected()) {
    connectionError = true;
  } else {
    connectionError = false;
  }
  const auto value = received;
  auto input_string = prefix + value;
  auto *const c_string = new char[input_string.size() + 1];
  std::copy(input_string.begin(), input_string.end(), c_string);
  c_string[input_string.size()] = '\n';
  arduinoTest->writeSerialPort(c_string, strlen(c_string));
  delete[] c_string;
}
void sendLengthToArduino(float received, std::string prefix, int strLength) {
  auto intVal = static_cast<int>(received);

  if (!arduinoTest->isConnected()) {
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
  arduinoTest->writeSerialPort(c_string, strlen(c_string));
  delete[] c_string;
}

void sendFloatToArduino(float received, std::string prefix) {
  if (!arduinoTest->isConnected()) {
    connectionError = true;
  } else {
    connectionError = false;
  }
  auto input_string = prefix + std::to_string(received);
  auto *const c_string = new char[input_string.size() + 1];
  std::copy(input_string.begin(), input_string.end(), c_string);
  c_string[input_string.size()] = '\n';
  if (received < 0) {
    arduinoTest->writeSerialPort(c_string, 7);
  } else {
    arduinoTest->writeSerialPort(c_string, 6);
  }

  delete[] c_string;
}

void sendBoolToArduino(float received, std::string prefix) {
  int intVal;
  if (received == 0) {
    intVal = 0;
  } else {
    intVal = 1;
  }

  if (!arduinoTest->isConnected()) {
    connectionError = true;
  } else {
    connectionError = false;
  }
  const auto value = intVal;
  auto input_string = prefix + std::to_string(value);
  auto *const c_string = new char[input_string.size() + 1];
  std::copy(input_string.begin(), input_string.end(), c_string);
  c_string[input_string.size()] = '\n';

  arduinoTest->writeSerialPort(c_string, strlen(c_string));
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
          outputMapper outputMapper;
          //outputMapper.mapOutputs(NULL,hSimConnect);
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


      }
      break;
    }

    case SIMCONNECT_RECV_ID_SIMOBJECT_DATA: {
      SIMCONNECT_RECV_SIMOBJECT_DATA *pObjData =
          (SIMCONNECT_RECV_SIMOBJECT_DATA *)pData;

      switch (pObjData->dwRequestID) {
        case REQUEST_STRING: {
          Struct1 *pS = (Struct1 *)&pObjData->dwData;
          sendCharToArduino(pS->title, "999");
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
                sendToArduino(radianToDegree(pS->datum[count].value), "454");
                break;
              }
              case DATA_PLANE_ALT_ABOVE_GROUND: {
                sendToArduino(pS->datum[count].value, "312");
                break;
              }

              // DATA
              case DATA_SIM_ON_GROUND: {
                sendBoolToArduino(pS->datum[count].value, "323");
                break;
              }

                // Avionics
              case DATA_VERTICAL_SPEED: {
                sendLengthToArduino(pS->datum[count].value, "590", 5);
                printf("\nAP Vertical speed = %f", pS->datum[count].value);
                break;
              }
              case DATA_COM_ALTMTR: {
                int inHg = pS->datum[count].value * 1000;
                if (inHg % (inHg / 10) >= 5) {
                  inHg += 10;
                }
                sendToArduino(inHg / 10, "337");
                printf("\nKohlman hg = %i", inHg / 10);
                break;
              }
              case DATA_HEADING_LOCK: {
                sendLengthToArduino(pS->datum[count].value, "582", 4);
                printf("\nHeading lock = %f", pS->datum[count].value);
                break;
              }
              case DATA_ALTITUDE_TARGET: {
                sendToArduino(pS->datum[count].value, "584");
                printf("\nAP Altitude target = %f", pS->datum[count].value);
                cout << "called alt target: " << pS->datum[count].value << endl;
                break;
              }
              case DATA_BAROMETER: {
                sendLengthToArduino(pS->datum[count].value, "632", 4);
                printf("\nBarometer = %f", pS->datum[count].value);
                break;
              }

                // Coms
              case DATA_COM_FREQ1: {
                sendToArduino(pS->datum[count].value, "900");
                printf("\nactive freq 1 = %f", pS->datum[count].value);
                break;
              }
              case DATA_COM_FREQ_STANDBY1: {
                sendToArduino(pS->datum[count].value, "901");
                printf("\nstandby freq 1 = %f", pS->datum[count].value);
                break;
              }
              case DATA_COM_FREQ2: {
                sendToArduino(pS->datum[count].value, "902");
                printf("\nactive freq 2 = %f", pS->datum[count].value);
                break;
              }
              case DATA_COM_FREQ_STANDBY2: {
                sendToArduino(pS->datum[count].value, "903");
                printf("\nstandby freq 2 = %f", pS->datum[count].value);
                break;
              }
              case DATA_NAV_ACTIVE_FREQUENCY1: {
                sendToArduino(pS->datum[count].value * 100, "910");
                break;
              }
              case DATA_NAV_STANDBY_FREQUENCY1: {
                sendToArduino(pS->datum[count].value * 100, "911");
                break;
              }
              case DATA_NAV_ACTIVE_FREQUENCY2: {
                sendToArduino(pS->datum[count].value * 100, "912");
                break;
              }
              case DATA_NAV_STANDBY_FREQUENCY2: {
                sendToArduino(pS->datum[count].value * 100, "913");
                break;
              }
              case DATA_NAV_RADIAL_ERROR1: {
                sendToArduino(pS->datum[count].value, "914");
                break;
              }
              case DATA_NAV_VOR_LATLONALT1: {
                sendToArduino(pS->datum[count].value, "915");
                break;
              }

                // DME
              case DATA_NAV_DME1: {
                sendToArduino(pS->datum[count].value, "950");
                break;
              }
              case DATA_NAV_DMESPEED1: {
                sendToArduino(pS->datum[count].value, "951");
                break;
              }
              case DATA_NAV_DME2: {
                sendToArduino(pS->datum[count].value, "952");
                break;
              }
              case DATA_NAV_DMESPEED2: {
                sendToArduino(pS->datum[count].value, "953");
                break;
              }

                // ADF
              case DATA_ADF_ACTIVE_FREQUENCY1: {
                sendLengthToArduino(pS->datum[count].value / 1000, "954", 4);
                break;
              }
              case DATA_ADF_STANDBY_FREQUENCY1: {
                sendLengthToArduino(pS->datum[count].value / 1000, "955", 4);
                break;
              }
              case DATA_ADF_RADIAL1: {
                sendToArduino(pS->datum[count].value, "956");
                break;
              }
              case DATA_ADF_SIGNAL1: {
                sendToArduino(pS->datum[count].value, "957");
                break;
              }

              case DATA_ADF_ACTIVE_FREQUENCY2: {
                sendLengthToArduino(pS->datum[count].value / 1000, "958", 4);
                break;
              }
              case DATA_ADF_STANDBY_FREQUENCY2: {
                sendLengthToArduino(pS->datum[count].value / 1000, "959", 4);
                break;
              }
              case DATA_ADF_RADIAL2: {
                sendToArduino(pS->datum[count].value, "960");
                break;
              }
              case DATA_ADF_SIGNAL2: {
                sendToArduino(pS->datum[count].value, "961");
                break;
              }

                // Transponder
              case DATA_TRANSPONDER_CODE1: {
                sendToArduino(pS->datum[count].value, "962");
                break;
              }
              case DATA_TRANSPONDER_CODE2: {
                sendToArduino(pS->datum[count].value, "963");
                break;
              }

                // Aircraft data
              case DATA_INDICATED_AIRSPEED: {
                sendLengthToArduino(pS->datum[count].value, "326", 3);
                printf("\nIndicated airspeed = %f", pS->datum[count].value);
                break;
              }
              case DATA_INDICATED_ALTITUDE: {
                sendLengthToArduino(pS->datum[count].value, "335", 5);

                printf("\nIndicated altitude = %f", pS->datum[count].value);
                break;
              }
              case DATA_INDICATED_HEADING: {
                sendLengthToArduino(radianToDegree(pS->datum[count].value),
                                    "344", 3);
                printf("\nIndicated heading = %f", pS->datum[count].value);
                break;
              }
              case DATA_GPS_GROUNDSPEED: {
                sendToArduino(pS->datum[count].value * 1.94, "430");
                printf("\nGPS groundspeed = %f", pS->datum[count].value);
                break;
              }
              case DATA_TRUE_VERTICAL_SPEED: {
                sendToArduino(pS->datum[count].value, "330");
                printf("\nTrue vertical speed = %f", pS->datum[count].value);
                break;
              }
              case DATA_SELECTED_QUANTITY_FUEL: {
                sendToArduino(pS->datum[count].value * 100, "275");
                printf("\nFuel in = %f", pS->datum[count].value * 100);
                break;
              }

              // lights
              case DATA_LIGHT_TAXI_ON: {
                sendBoolToArduino(pS->datum[count].value, "033");
                QString test =
                    QString::fromStdString("Taxi light: " + valString);

                cout << "Taxi: " << pS->datum[count].value << endl;
                break;
              }
              case DATA_LIGHT_STROBE_ON: {
                sendBoolToArduino(pS->datum[count].value, "034");
                cout << "Strobe: " << pS->datum[count].value << endl;

                break;
              }
              case DATA_LIGHT_PANEL_ON: {
                sendBoolToArduino(pS->datum[count].value, "035");
                cout << "Panel: " << pS->datum[count].value << endl;
                break;
              }
              case DATA_LIGHT_RECOGNITION_ON: {
                sendBoolToArduino(pS->datum[count].value, "036");
                cout << "Recognition: " << pS->datum[count].value << endl;
                break;
              }
              case DATA_LIGHT_WING_ON: {
                sendBoolToArduino(pS->datum[count].value, "037");
                cout << "Wing light: " << pS->datum[count].value << endl;
                break;
              }
              case DATA_LIGHT_LOGO_ON: {
                sendBoolToArduino(pS->datum[count].value, "038");
                cout << "Logo light: " << pS->datum[count].value << endl;
                break;
              }
              case DATA_LIGHT_CABIN_ON: {
                sendBoolToArduino(pS->datum[count].value, "039");
                cout << "Cabin light: " << pS->datum[count].value << endl;
                break;
              }
              case DATA_LIGHT_HEAD_ON: {
                sendBoolToArduino(pS->datum[count].value, "040");
                cout << "Head light: " << pS->datum[count].value << endl;
                break;
              }
              case DATA_LIGHT_BRAKE_ON: {
                sendBoolToArduino(pS->datum[count].value, "041");
                cout << "Brake light: " << pS->datum[count].value << endl;
                break;
              }
              case DATA_LIGHT_NAV_ON: {
                sendBoolToArduino(pS->datum[count].value, "042");
                cout << "Nav light: " << pS->datum[count].value << endl;
                break;
              }
              case DATA_LIGHT_BEACON_ON: {
                sendBoolToArduino(pS->datum[count].value, "043");
                cout << "Beacon light: " << pS->datum[count].value << endl;
                break;
              }
              case DATA_LIGHT_LANDING_ON: {
                sendBoolToArduino(pS->datum[count].value, "044");
                cout << "Landing light: " << pS->datum[count].value << endl;
                break;
              }

              // warnings
              case DATA_STALL_WARNING: {
                sendBoolToArduino(pS->datum[count].value, "333");
                break;
              }
              case DATA_OVERSPEED_WARNING: {
                sendBoolToArduino(pS->datum[count].value, "334");
                break;
              }

              // Gear
              case DATA_GEAR_HANDLE_POSITION: {
                sendBoolToArduino(pS->datum[count].value, "526");
                break;
              }
              case DATA_GEAR_HYDRAULIC_PRESSURE: {
                sendLengthToArduino(pS->datum[count].value, "527", 6);
                break;
              }
              case DATA_TAILWHEEL_LOCK_ON: {
                sendBoolToArduino(pS->datum[count].value, "528");
                break;
              }
              case DATA_GEAR_CENTER_POSITION: {
                sendLengthToArduino(pS->datum[count].value * 100, "529", 3);
                break;
              }
              case DATA_GEAR_LEFT_POSITION: {
                sendLengthToArduino(pS->datum[count].value * 100, "530", 3);
                break;
              }
              case DATA_GEAR_RIGHT_POSITION: {
                sendLengthToArduino(pS->datum[count].value * 100, "531", 3);
                break;
              }
              case DATA_GEAR_TAIL_POSITION: {
                sendLengthToArduino(pS->datum[count].value * 100, "532", 3);
                break;
              }
              case DATA_GEAR_AUX_POSITION: {
                sendLengthToArduino(pS->datum[count].value * 100, "533", 3);
                break;
              }
              case DATA_GEAR_TOTAL_PCT_EXTENDED: {
                sendLengthToArduino(pS->datum[count].value * 100, "536", 3);
                break;
              }

              // Flaps
              case DATA_FLAPS_HANDLE_PERCENT: {
                sendLengthToArduino(pS->datum[count].value * 100, "510", 3);
                break;
              }
              case DATA_FLAPS_HANDLE_INDEX: {
                sendLengthToArduino(pS->datum[count].value, "511", 3);
                break;
              }
              case DATA_FLAPS_NUM_HANDLE_POSITIONS: {
                sendLengthToArduino(pS->datum[count].value, "512", 3);
                break;
              }
              case DATA_TRAILING_EDGE_FLAPS_LEFT_PERCENT: {
                sendLengthToArduino(pS->datum[count].value * 100, "513", 3);
                break;
              }
              case DATA_TRAILING_EDGE_FLAPS_RIGHT_PERCENT: {
                sendLengthToArduino(pS->datum[count].value * 100, "514", 3);
                break;
              }
              case DATA_TRAILING_EDGE_FLAPS_LEFT_ANGLE: {
                sendLengthToArduino(radianToDegree(pS->datum[count].value),
                                    "515", 3);
                break;
              }
              case DATA_TRAILING_EDGE_FLAPS_RIGHT_ANGLE: {
                sendLengthToArduino(radianToDegree(pS->datum[count].value),
                                    "516", 3);
                break;
              }
              case DATA_LEADING_EDGE_FLAPS_LEFT_PERCENT: {
                sendLengthToArduino(pS->datum[count].value * 100, "517", 3);
                break;
              }
              case DATA_LEADING_EDGE_FLAPS_RIGHT_PERCENT: {
                sendLengthToArduino(pS->datum[count].value * 100, "518", 3);
                break;
              }
              case DATA_LEADING_EDGE_FLAPS_LEFT_ANGLE: {
                sendLengthToArduino(radianToDegree(pS->datum[count].value),
                                    "519", 3);
                break;
              }
              case DATA_LEADING_EDGE_FLAPS_RIGHT_ANGLE: {
                sendLengthToArduino(radianToDegree(pS->datum[count].value),
                                    "520", 3);

                break;
              }

              // Rudder trim
              case DATA_ELEVATOR_TRIM_POSITION: {
                sendToArduino(radianToDegree(pS->datum[count].value), "498");
                break;
              }
              case DATA_ELEVATOR_TRIM_PCT: {
                sendToArduino(pS->datum[count].value * 100, "500");
                cout << "elev trim pct" << pS->datum[count].value * 100 << endl;
                break;
              }
              case DATA_AILERON_TRIM: {
                sendToArduino(radianToDegree(pS->datum[count].value), "562");
                break;
              }
              case DATA_AILERON_TRIM_PCT: {
                sendLengthToArduino(pS->datum[count].value * 100, "563", 3);
                break;
              }
              case DATA_RUDDER_TRIM: {
                sendFloatToArduino(radianToDegree(pS->datum[count].value),
                                   "566");
                break;
              }
              case DATA_RUDDER_TRIM_PCT: {
                sendLengthToArduino(pS->datum[count].value * 100, "567", 3);
                break;
              }

              // AP
              case DATA_AUTOPILOT_AVAILABLE: {
                sendBoolToArduino(pS->datum[count].value, "576");
                break;
              }
              case DATA_AUTOPILOT_MASTER: {
                sendBoolToArduino(pS->datum[count].value, "577");
                break;
              }
              case DATA_AUTOPILOT_WING_LEVELER: {
                sendBoolToArduino(pS->datum[count].value, "579");
                break;
              }
              case DATA_AUTOPILOT_NAV1_LOCK: {
                sendBoolToArduino(pS->datum[count].value, "580");
                break;
              }
              case DATA_AUTOPILOT_HEADING_LOCK: {
                sendBoolToArduino(pS->datum[count].value, "581");
                break;
              }
              case DATA_AUTOPILOT_ALTITUDE_LOCK: {
                sendBoolToArduino(pS->datum[count].value, "583");
                break;
              }
              case DATA_AUTOPILOT_ATTITUDE_HOLD: {
                sendBoolToArduino(pS->datum[count].value, "585");
                break;
              }
              case DATA_AUTOPILOT_GLIDESLOPE_HOLD: {
                sendBoolToArduino(pS->datum[count].value, "586");
                break;
              }
              case DATA_AUTOPILOT_APPROACH_HOLD: {
                sendBoolToArduino(pS->datum[count].value, "588");
                break;
              }
              case DATA_AUTOPILOT_BACKCOURSE_HOLD: {
                sendBoolToArduino(pS->datum[count].value, "589");
                break;
              }
              case DATA_AUTOPILOT_FLIGHT_DIRECTOR_ACTIVE: {
                sendBoolToArduino(pS->datum[count].value, "591");
                break;
              }
              case DATA_AUTOPILOT_AIRSPEED_HOLD: {
                sendBoolToArduino(pS->datum[count].value, "594");
                break;
              }
              case DATA_AUTOPILOT_MACH_HOLD: {
                sendBoolToArduino(pS->datum[count].value, "596");
                break;
              }
              case DATA_AUTOPILOT_YAW_DAMPER: {
                sendBoolToArduino(pS->datum[count].value, "598");
                break;
              }
              case DATA_AUTOPILOT_THROTTLE_ARM: {
                sendBoolToArduino(pS->datum[count].value, "600");
                break;
              }
              case DATA_AUTOPILOT_TAKEOFF_POWER_ACTIVE: {
                sendBoolToArduino(pS->datum[count].value, "601");
                break;
              }
              case DATA_AUTOTHROTTLE_ACTIVE: {
                sendBoolToArduino(pS->datum[count].value, "602");
                break;
              }
              case DATA_AUTOPILOT_VERTICAL_HOLD: {
                sendBoolToArduino(pS->datum[count].value, "604");
                break;
              }
              case DATA_AUTOPILOT_RPM_HOLD: {
                sendBoolToArduino(pS->datum[count].value, "605");
                break;
              }

              case DATA_NAV_OBS_INDEX1: {
                sendToArduino(pS->datum[count].value, "606");
                cout << pS->datum[count].value << endl;
                break;
              }
              case DATA_NAV_OBS_INDEX2: {
                sendToArduino(pS->datum[count].value, "607");
                cout << pS->datum[count].value << endl;
                break;
              }
              case DATA_BRAKE_PARKING_INDICATOR: {
                sendBoolToArduino(pS->datum[count].value, "505");
                cout << pS->datum[count].value << endl;
                break;
              }
              case DATA_FUEL_TANK_CENTER_LEVEL: {
                sendToArduino(pS->datum[count].value * 100, "234");
                break;
              }
              case DATA_FUEL_TANK_CENTER2_LEVEL: {
                sendToArduino(pS->datum[count].value * 100, "235");
                break;
              }
              case DATA_FUEL_TANK_CENTER3_LEVEL: {
                sendToArduino(pS->datum[count].value * 100, "236");
                break;
              }
              case DATA_FUEL_TANK_LEFT_MAIN_LEVEL: {
                sendToArduino(pS->datum[count].value * 100, "237");
                break;
              }
              case DATA_FUEL_TANK_LEFT_AUX_LEVEL: {
                sendToArduino(pS->datum[count].value * 100, "238");
                break;
              }
              case DATA_FUEL_TANK_LEFT_TIP_LEVEL: {
                sendToArduino(pS->datum[count].value * 100, "239");
                break;
              }
              case DATA_FUEL_TANK_RIGHT_MAIN_LEVEL: {
                sendToArduino(pS->datum[count].value * 100, "240");
                cout << "right main lvl: " << pS->datum[count].value << endl;

                break;
              }
              case DATA_FUEL_TANK_RIGHT_AUX_LEVEL: {
                sendToArduino(pS->datum[count].value * 100, "241");
                break;
              }
              case DATA_FUEL_TANK_RIGHT_TIP_LEVEL: {
                sendToArduino(pS->datum[count].value * 100, "242");
                break;
              }
              case DATA_FUEL_TANK_EXTERNAL1_LEVEL: {
                sendToArduino(pS->datum[count].value * 100, "243");
                break;
              }
              case DATA_FUEL_TANK_EXTERNAL2_LEVEL: {
                sendToArduino(pS->datum[count].value * 100, "244");
                break;
              }
              case DATA_FUEL_TANK_CENTER_CAPACITY: {
                sendToArduino(pS->datum[count].value, "245");
                break;
              }
              case DATA_FUEL_TANK_CENTER2_CAPACITY: {
                sendToArduino(pS->datum[count].value, "246");
                break;
              }
              case DATA_FUEL_TANK_CENTER3_CAPACITY: {
                sendToArduino(pS->datum[count].value, "247");
                break;
              }
              case DATA_FUEL_TANK_LEFT_MAIN_CAPACITY: {
                sendToArduino(pS->datum[count].value, "248");
                break;
              }
              case DATA_FUEL_TANK_LEFT_AUX_CAPACITY: {
                sendToArduino(pS->datum[count].value, "249");
                break;
              }
              case DATA_FUEL_TANK_LEFT_TIP_CAPACITY: {
                sendToArduino(pS->datum[count].value, "250");
                break;
              }
              case DATA_FUEL_TANK_RIGHT_MAIN_CAPACITY: {
                sendToArduino(pS->datum[count].value, "251");
                cout << pS->datum[count].value << endl;
                break;
              }
              case DATA_FUEL_TANK_RIGHT_AUX_CAPACITY: {
                sendToArduino(pS->datum[count].value, "252");
                break;
              }
              case DATA_FUEL_TANK_RIGHT_TIP_CAPACITY: {
                sendToArduino(pS->datum[count].value, "253");
                break;
              }
              case DATA_FUEL_TANK_EXTERNAL1_CAPACITY: {
                sendToArduino(pS->datum[count].value, "254");
                break;
              }
              case DATA_FUEL_TANK_EXTERNAL2_CAPACITY: {
                sendToArduino(pS->datum[count].value, "255");
                break;
              }
              case DATA_FUEL_LEFT_CAPACITY: {
                sendFloatToArduino(pS->datum[count].value, "256");
                cout << "fuelCapL: " << pS->datum[count].value << endl;
                break;
              }
              case DATA_FUEL_RIGHT_CAPACITY: {
                sendFloatToArduino(pS->datum[count].value, "257");
                cout << "fuelCapR: " << pS->datum[count].value << endl;
                break;
              }
              case DATA_FUEL_TANK_CENTER_QUANTITY: {
                sendToArduino(pS->datum[count].value, "258");
                break;
              }
              case DATA_FUEL_TANK_CENTER2_QUANTITY: {
                sendToArduino(pS->datum[count].value, "259");
                break;
              }
              case DATA_FUEL_TANK_CENTER3_QUANTITY: {
                sendToArduino(pS->datum[count].value, "260");
                break;
              }
              case DATA_FUEL_TANK_LEFT_MAIN_QUANTITY: {
                sendToArduino(pS->datum[count].value, "261");
                break;
              }
              case DATA_FUEL_TANK_LEFT_AUX_QUANTITY: {
                sendToArduino(pS->datum[count].value, "262");
                break;
              }
              case DATA_FUEL_TANK_LEFT_TIP_QUANTITY: {
                sendToArduino(pS->datum[count].value, "263");
                break;
              }
              case DATA_FUEL_TANK_RIGHT_MAIN_QUANTITY: {
                sendToArduino(pS->datum[count].value, "264");
                break;
              }
              case DATA_FUEL_TANK_RIGHT_AUX_QUANTITY: {
                sendToArduino(pS->datum[count].value, "265");
                break;
              }
              case DATA_FUEL_TANK_RIGHT_TIP_QUANTITY: {
                sendToArduino(pS->datum[count].value, "266");
                break;
              }
              case DATA_FUEL_TANK_EXTERNAL1_QUANTITY: {
                sendToArduino(pS->datum[count].value, "267");
                break;
              }
              case DATA_FUEL_TANK_EXTERNAL2_QUANTITY: {
                sendToArduino(pS->datum[count].value, "268");
                break;
              }
              case DATA_FUEL_LEFT_QUANTITY: {
                sendFloatToArduino(pS->datum[count].value, "269");
                cout << "fuelQL: " << pS->datum[count].value << endl;
                break;
              }
              case DATA_FUEL_RIGHT_QUANTITY: {
                sendFloatToArduino(pS->datum[count].value, "270");
                cout << "fuelQR: " << pS->datum[count].value << endl;
                break;
              }
              case DATA_FUEL_TOTAL_QUANTITY: {
                sendToArduino(pS->datum[count].value, "271");
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

    // DATA
    if (cbPlaneAltAboveGround) {
      hr = SimConnect_AddToDataDefinition(
          hSimConnect, DEFINITION_PDR, "PLANE ALT ABOVE GROUND", "Feet",
          SIMCONNECT_DATATYPE_FLOAT32, 10, DATA_PLANE_ALT_ABOVE_GROUND);
    }
    if (cbSimOnGround) {
      hr = SimConnect_AddToDataDefinition(
          hSimConnect, DEFINITION_PDR, "SIM ON GROUND", "Bool",
          SIMCONNECT_DATATYPE_INT32, 0, DATA_SIM_ON_GROUND);
    }

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
    if (cbNavObs1) {
      SimConnect_AddToDataDefinition(hSimConnect, DEFINITION_PDR, "NAV OBS:1",
                                     "Degrees", SIMCONNECT_DATATYPE_FLOAT32,
                                     0.1, DATA_NAV_OBS_INDEX1);
    }
    if (cbNavObs2) {
      SimConnect_AddToDataDefinition(hSimConnect, DEFINITION_PDR, "NAV OBS:2",
                                     "Degrees", SIMCONNECT_DATATYPE_FLOAT32,
                                     0.1, DATA_NAV_OBS_INDEX2);
    }
    if (cbFuelTankCenterLevel) {
      hr = SimConnect_AddToDataDefinition(
          hSimConnect, DEFINITION_PDR, "FUEL TANK CENTER LEVEL",
          "Percent Over 100", SIMCONNECT_DATATYPE_FLOAT32, 0.01,
          DATA_FUEL_TANK_CENTER_LEVEL);
    }
    if (cbFuelTankCenter2Level) {
      hr = SimConnect_AddToDataDefinition(
          hSimConnect, DEFINITION_PDR, "FUEL TANK CENTER2 LEVEL",
          "Percent Over 100", SIMCONNECT_DATATYPE_FLOAT32, 0.01,
          DATA_FUEL_TANK_CENTER2_LEVEL);
    }
    if (cbFuelTankCenter3Level) {
      hr = SimConnect_AddToDataDefinition(
          hSimConnect, DEFINITION_PDR, "FUEL TANK CENTER3 LEVEL",
          "Percent Over 100", SIMCONNECT_DATATYPE_FLOAT32, 0.01,
          DATA_FUEL_TANK_CENTER3_LEVEL);
    }
    if (cbFuelTankLeftMainLevel) {
      hr = SimConnect_AddToDataDefinition(
          hSimConnect, DEFINITION_PDR, "FUEL TANK LEFT MAIN LEVEL",
          "Percent Over 100", SIMCONNECT_DATATYPE_FLOAT32, 0.01,
          DATA_FUEL_TANK_LEFT_MAIN_LEVEL);
    }
    if (cbFuelTankLeftAuxLevel) {
      hr = SimConnect_AddToDataDefinition(
          hSimConnect, DEFINITION_PDR, "FUEL TANK LEFT AUX LEVEL",
          "Percent Over 100", SIMCONNECT_DATATYPE_FLOAT32, 0.01,
          DATA_FUEL_TANK_LEFT_AUX_LEVEL);
    }
    if (cbFuelTankLeftTipLevel) {
      hr = SimConnect_AddToDataDefinition(
          hSimConnect, DEFINITION_PDR, "FUEL TANK LEFT TIP LEVEL",
          "Percent Over 100", SIMCONNECT_DATATYPE_FLOAT32, 0.01,
          DATA_FUEL_TANK_LEFT_TIP_LEVEL);
    }
    if (cbFuelTankRightMainLevel) {
      hr = SimConnect_AddToDataDefinition(
          hSimConnect, DEFINITION_PDR, "FUEL TANK RIGHT MAIN LEVEL",
          "Percent Over 100", SIMCONNECT_DATATYPE_FLOAT32, 0.01,
          DATA_FUEL_TANK_RIGHT_MAIN_LEVEL);
    }
    if (cbFuelTankRightAuxLevel) {
      hr = SimConnect_AddToDataDefinition(
          hSimConnect, DEFINITION_PDR, "FUEL TANK RIGHT AUX LEVEL",
          "Percent Over 100", SIMCONNECT_DATATYPE_FLOAT32, 0.01,
          DATA_FUEL_TANK_RIGHT_AUX_LEVEL);
    }
    if (cbFuelTankRightTipLevel) {
      hr = SimConnect_AddToDataDefinition(
          hSimConnect, DEFINITION_PDR, "FUEL TANK RIGHT TIP LEVEL",
          "Percent Over 100", SIMCONNECT_DATATYPE_FLOAT32, 0.01,
          DATA_FUEL_TANK_RIGHT_TIP_LEVEL);
    }
    if (cbFuelTankExternal1Level) {
      hr = SimConnect_AddToDataDefinition(
          hSimConnect, DEFINITION_PDR, "FUEL TANK EXTERNAL1 LEVEL",
          "Percent Over 100", SIMCONNECT_DATATYPE_FLOAT32, 0.01,
          DATA_FUEL_TANK_EXTERNAL1_LEVEL);
    }
    if (cbFuelTankExternal2Level) {
      hr = SimConnect_AddToDataDefinition(
          hSimConnect, DEFINITION_PDR, "FUEL TANK EXTERNAL2 LEVEL",
          "Percent Over 100", SIMCONNECT_DATATYPE_FLOAT32, 0.01,
          DATA_FUEL_TANK_EXTERNAL2_LEVEL);
    }
    if (cbFuelTankCenterCapacity) {
      hr = SimConnect_AddToDataDefinition(
          hSimConnect, DEFINITION_PDR, "FUEL TANK CENTER CAPACITY", "Gallons",
          SIMCONNECT_DATATYPE_FLOAT32, 1, DATA_FUEL_TANK_CENTER_CAPACITY);
    }
    if (cbFuelTankCenter2Capacity) {
      hr = SimConnect_AddToDataDefinition(
          hSimConnect, DEFINITION_PDR, "FUEL TANK CENTER2 CAPACITY", "Gallons",
          SIMCONNECT_DATATYPE_FLOAT32, 1, DATA_FUEL_TANK_CENTER2_CAPACITY);
    }
    if (cbFuelTankCenter3Capacity) {
      hr = SimConnect_AddToDataDefinition(
          hSimConnect, DEFINITION_PDR, "FUEL TANK CENTER3 CAPACITY", "Gallons",
          SIMCONNECT_DATATYPE_FLOAT32, 1, DATA_FUEL_TANK_CENTER3_CAPACITY);
    }
    if (cbFuelTankLeftMainCapacity) {
      hr = SimConnect_AddToDataDefinition(
          hSimConnect, DEFINITION_PDR, "FUEL TANK LEFT MAIN CAPACITY",
          "Gallons", SIMCONNECT_DATATYPE_FLOAT32, 1,
          DATA_FUEL_TANK_LEFT_MAIN_CAPACITY);
    }
    if (cbFuelTankLeftAuxCapacity) {
      hr = SimConnect_AddToDataDefinition(
          hSimConnect, DEFINITION_PDR, "FUEL TANK LEFT AUX CAPACITY", "Gallons",
          SIMCONNECT_DATATYPE_FLOAT32, 1, DATA_FUEL_TANK_LEFT_AUX_CAPACITY);
    }
    if (cbFuelTankLeftTipCapacity) {
      hr = SimConnect_AddToDataDefinition(
          hSimConnect, DEFINITION_PDR, "FUEL TANK LEFT TIP CAPACITY", "Gallons",
          SIMCONNECT_DATATYPE_FLOAT32, 1, DATA_FUEL_TANK_LEFT_TIP_CAPACITY);
    }
    if (cbFuelTankRightMainCapacity) {
      hr = SimConnect_AddToDataDefinition(
          hSimConnect, DEFINITION_PDR, "FUEL TANK RIGHT MAIN CAPACITY",
          "Gallons", SIMCONNECT_DATATYPE_FLOAT32, 1,
          DATA_FUEL_TANK_RIGHT_MAIN_CAPACITY);
    }
    if (cbFuelTankRightAuxCapacity) {
      hr = SimConnect_AddToDataDefinition(
          hSimConnect, DEFINITION_PDR, "FUEL TANK RIGHT AUX CAPACITY",
          "Gallons", SIMCONNECT_DATATYPE_FLOAT32, 1,
          DATA_FUEL_TANK_RIGHT_AUX_CAPACITY);
    }
    if (cbFuelTankRightTipCapacity) {
      hr = SimConnect_AddToDataDefinition(
          hSimConnect, DEFINITION_PDR, "FUEL TANK RIGHT TIP CAPACITY",
          "Gallons", SIMCONNECT_DATATYPE_FLOAT32, 1,
          DATA_FUEL_TANK_RIGHT_TIP_CAPACITY);
    }
    if (cbFuelTankExternal1Capacity) {
      hr = SimConnect_AddToDataDefinition(
          hSimConnect, DEFINITION_PDR, "FUEL TANK EXTERNAL1 CAPACITY",
          "Gallons", SIMCONNECT_DATATYPE_FLOAT32, 1,
          DATA_FUEL_TANK_EXTERNAL1_CAPACITY);
    }
    if (cbFuelTankExternal2Capacity) {
      hr = SimConnect_AddToDataDefinition(
          hSimConnect, DEFINITION_PDR, "FUEL TANK EXTERNAL2 CAPACITY",
          "Gallons", SIMCONNECT_DATATYPE_FLOAT32, 1,
          DATA_FUEL_TANK_EXTERNAL2_CAPACITY);
    }
    if (cbFuelLeftCapacity) {
      hr = SimConnect_AddToDataDefinition(
          hSimConnect, DEFINITION_PDR, "FUEL LEFT CAPACITY", "Gallons",
          SIMCONNECT_DATATYPE_FLOAT32, 1, DATA_FUEL_LEFT_CAPACITY);
    }
    if (cbFuelRightCapacity) {
      hr = SimConnect_AddToDataDefinition(
          hSimConnect, DEFINITION_PDR, "FUEL RIGHT CAPACITY", "Gallons",
          SIMCONNECT_DATATYPE_FLOAT32, 1, DATA_FUEL_RIGHT_CAPACITY);
    }
    if (cbFuelTankCenterQuantity) {
      hr = SimConnect_AddToDataDefinition(
          hSimConnect, DEFINITION_PDR, "FUEL TANK CENTER QUANTITY", "Gallons",
          SIMCONNECT_DATATYPE_FLOAT32, 1, DATA_FUEL_TANK_CENTER_QUANTITY);
    }
    if (cbFuelTankCenter2Quantity) {
      hr = SimConnect_AddToDataDefinition(
          hSimConnect, DEFINITION_PDR, "FUEL TANK CENTER2 QUANTITY", "Gallons",
          SIMCONNECT_DATATYPE_FLOAT32, 1, DATA_FUEL_TANK_CENTER2_QUANTITY);
    }
    if (cbFuelTankCenter3Quantity) {
      hr = SimConnect_AddToDataDefinition(
          hSimConnect, DEFINITION_PDR, "FUEL TANK CENTER3 QUANTITY", "Gallons",
          SIMCONNECT_DATATYPE_FLOAT32, 1, DATA_FUEL_TANK_CENTER3_QUANTITY);
    }
    if (cbFuelTankLeftMainQuantity) {
      hr = SimConnect_AddToDataDefinition(
          hSimConnect, DEFINITION_PDR, "FUEL TANK LEFT MAIN QUANTITY",
          "Gallons", SIMCONNECT_DATATYPE_FLOAT32, 1,
          DATA_FUEL_TANK_LEFT_MAIN_QUANTITY);
    }
    if (cbFuelTankLeftAuxQuantity) {
      hr = SimConnect_AddToDataDefinition(
          hSimConnect, DEFINITION_PDR, "FUEL TANK LEFT AUX QUANTITY", "Gallons",
          SIMCONNECT_DATATYPE_FLOAT32, 1, DATA_FUEL_TANK_LEFT_AUX_QUANTITY);
    }
    if (cbFuelTankLeftTipQuantity) {
      hr = SimConnect_AddToDataDefinition(
          hSimConnect, DEFINITION_PDR, "FUEL TANK LEFT TIP QUANTITY", "Gallons",
          SIMCONNECT_DATATYPE_FLOAT32, 1, DATA_FUEL_TANK_LEFT_TIP_QUANTITY);
    }
    if (cbFuelTankRightMainQuantity) {
      hr = SimConnect_AddToDataDefinition(
          hSimConnect, DEFINITION_PDR, "FUEL TANK RIGHT MAIN QUANTITY",
          "Gallons", SIMCONNECT_DATATYPE_FLOAT32, 1,
          DATA_FUEL_TANK_RIGHT_MAIN_QUANTITY);
    }
    if (cbFuelTankRightAuxQuantity) {
      hr = SimConnect_AddToDataDefinition(
          hSimConnect, DEFINITION_PDR, "FUEL TANK RIGHT AUX QUANTITY",
          "Gallons", SIMCONNECT_DATATYPE_FLOAT32, 1,
          DATA_FUEL_TANK_RIGHT_AUX_QUANTITY);
    }
    if (cbFuelTankRightTipQuantity) {
      hr = SimConnect_AddToDataDefinition(
          hSimConnect, DEFINITION_PDR, "FUEL TANK RIGHT TIP QUANTITY",
          "Gallons", SIMCONNECT_DATATYPE_FLOAT32, 1,
          DATA_FUEL_TANK_RIGHT_TIP_QUANTITY);
    }
    if (cbFuelTankExternal1Quantity) {
      hr = SimConnect_AddToDataDefinition(
          hSimConnect, DEFINITION_PDR, "FUEL TANK EXTERNAL1 QUANTITY",
          "Gallons", SIMCONNECT_DATATYPE_FLOAT32, 1,
          DATA_FUEL_TANK_EXTERNAL1_QUANTITY);
    }
    if (cbFuelTankExternal2Quantity) {
      hr = SimConnect_AddToDataDefinition(
          hSimConnect, DEFINITION_PDR, "FUEL TANK EXTERNAL2 QUANTITY",
          "Gallons", SIMCONNECT_DATATYPE_FLOAT32, 1,
          DATA_FUEL_TANK_EXTERNAL2_QUANTITY);
    }
    if (cbFuelLeftQuantity) {
      hr = SimConnect_AddToDataDefinition(
          hSimConnect, DEFINITION_PDR, "FUEL LEFT QUANTITY", "Gallons",
          SIMCONNECT_DATATYPE_FLOAT32, 1, DATA_FUEL_LEFT_QUANTITY);
    }
    if (cbFuelRightQuantity) {
      hr = SimConnect_AddToDataDefinition(
          hSimConnect, DEFINITION_PDR, "FUEL RIGHT QUANTITY", "Gallons",
          SIMCONNECT_DATATYPE_FLOAT32, 1, DATA_FUEL_RIGHT_QUANTITY);
    }
    if (cbFuelTotalQuantity) {
      hr = SimConnect_AddToDataDefinition(
          hSimConnect, DEFINITION_PDR, "FUEL TOTAL QUANTITY", "Gallons",
          SIMCONNECT_DATATYPE_FLOAT32, 1, DATA_FUEL_TOTAL_QUANTITY);
    }

    if (cbBrakeParkingIndicator) {
      hr = SimConnect_AddToDataDefinition(
          hSimConnect, DEFINITION_PDR, "BRAKE PARKING INDICATOR", "Bool",
          SIMCONNECT_DATATYPE_INT32, 1, DATA_BRAKE_PARKING_INDICATOR);
    }

    QString path =
        QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QSettings settings(path + "/" + "settings.ini", QSettings::IniFormat);
    settings.beginGroup("Coms");
    string val = settings.value("outputComActiveBase").toString().toStdString();
    const char *valPort = val.c_str();
    arduinoTest = new SerialPort(valPort);
    settings.endGroup();
    settings.sync();

    // Request an event when the simulation starts

    hr =
        SimConnect_SubscribeToSystemEvent(hSimConnect, EVENT_SIM_START, "1sec");

    printf("\nLaunch a flight.");

    while (abort != true) {
      if (connectionError) {
        if (lastConnectionState != connectionError) {
          emit updateLastStatusUI("Error connecting to Arduino");
          lastConnectionState = connectionError;
        }
      } else {
        emit updateLastStatusUI("Succesfully connected to your Arduino");
      }

      SimConnect_CallDispatch(hSimConnect, MyDispatchProcRD, nullptr);
      Sleep(1);
      quit();
    }
    arduinoTest->closeSerial();
    hr = SimConnect_Close(hSimConnect);
  }
}
ConnectWorker::~ConnectWorker() {
  lastConnectionState = false;
  connectionError = false;
  // arduinoTest->closeSerial();
  mutex.lock();
  abort = true;
  condition.wakeOne();
  mutex.unlock();
}
