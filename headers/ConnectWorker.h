#ifndef CONNECTWORKER_H
#define CONNECTWORKER_H

#include <qmutex.h>
#include <qsettings.h>
#include <qstandardpaths.h>
#include <qwaitcondition.h>
#include <tchar.h>
#include <windows.h>

#include <QObject>
#include <QThread>
#include <headers/SerialPort.hpp>
#include <string>

#include "SimConnect.h"
#include "stdio.h"

class ConnectWorker : public QThread {
  Q_OBJECT
  void run() override { testDataRequest(); }
 signals:
  void updateLastValUI(QString lastVal);
  void updateLastStatusUI(QString lastStatus);

 public:
  ConnectWorker();
  ~ConnectWorker();
  QMutex mutex;
  QWaitCondition condition;

  std::string getLastVal() { return lastVal; }
  std::string getLastStatusVal() { return lastStatus; }

  // Data
  bool cbPlaneAltAboveGround = false;
  bool cbSimOnGround = false;

  // Avionics
  bool cbPlaneName = false;
  bool cbAPAltitudeLock = false;
  bool cbAPHeadingLock = false;
  bool cbAPVerticalLock = false;
  bool cbIndicatedAirspeed = false;
  bool cbIndicatedAltitude = false;
  bool cbIndicatedHeading = false;
  bool cbIndicatedVerticalSpeed = false;
  bool cbGPSGroundspeed = false;
  bool cbKohlman = false;
  bool cbBarometerPressure = false;
  bool cbSelectedQuantityPercent = false;

  // GPS
  bool cbGpsCourseToSteer = false;

  // Coms
  bool cbActiveCom1 = false;
  bool cbActiveCom2 = false;
  bool cbStandbyCom1 = false;
  bool cbStandbyCom2 = false;
  bool cbNavActiveFrequency1 = false;
  bool cbNavStandbyFrequency1 = false;
  bool cbNavActiveFrequency2 = false;
  bool cbNavStandbyFrequency2 = false;
  bool cbNavRadialError1 = false;
  bool cbNavVorLatlonalt1 = false;

  // OBS
  bool cbNavObs1;
  bool cbNavObs2;

  // DME
  bool cbNavDme1 = false;
  bool cbNavDmespeed1 = false;
  bool cbNavDme2 = false;
  bool cbNavDmespeed2 = false;

  // ADF
  bool cbAdfActiveFrequency1 = false;
  bool cbAdfStandbyFrequency1 = false;
  bool cbAdfRadial1 = false;
  bool cbAdfSignal1 = false;

  bool cbAdfActiveFrequency2 = false;
  bool cbAdfStandbyFrequency2 = false;
  bool cbAdfRadial2 = false;
  bool cbAdfSignal2 = false;

  // Transponder
  bool cbTransponderCode1 = false;
  bool cbTransponderCode2 = false;

  // Lights
  bool cbLightTaxiOn = false;
  bool cbLightStrobeOn = false;
  bool cbLightPanelOn = false;
  bool cbLightRecognitionOn = false;
  bool cbLightWingOn = false;
  bool cbLightLogoOn = false;
  bool cbLightCabinOn = false;
  bool cbLightHeadOn = false;
  bool cbLightBrakeOn = false;
  bool cbLightNavOn = false;
  bool cbLightBeaconOn = false;
  bool cbLightLandingOn = false;

  // warnings
  bool cbStallWarning = false;
  bool cbOverspeedWarning = false;

  // Rudder/trim
  bool cbElevatorTrimPosition = false;
  bool cbElevatorTrimIndicator = false;
  bool cbElevatorTrimPct = false;
  bool cbAileronTrimPct = false;
  bool cbRudderTrimPct = false;
  bool cbAileronTrim = false;
  bool cbRudderTrim = false;

  // flaps
  bool cbFlapsHandlePercent = false;
  bool cbFlapsHandleIndex = false;
  bool cbFlapsNumHandlePositions = false;
  bool cbTrailingEdgeFlapsLeftPercent = false;
  bool cbTrailingEdgeFlapsRightPercent = false;
  bool cbTrailingEdgeFlapsLeftAngle = false;
  bool cbTrailingEdgeFlapsRightAngle = false;
  bool cbLeadingEdgeFlapsLeftPercent = false;
  bool cbLeadingEdgeFlapsRightPercent = false;
  bool cbLeadingEdgeFlapsLeftAngle = false;
  bool cbLeadingEdgeFlapsRightAngle = false;

  // gear
  bool cbGearHandlePosition = false;
  bool cbGearHydraulicPressure = false;
  bool cbTailwheelLockOn = false;
  bool cbGearCenterPosition = false;
  bool cbGearLeftPosition = false;
  bool cbGearRightPosition = false;
  bool cbGearTailPosition = false;
  bool cbGearAuxPosition = false;
  bool cbGearTotalPctExtended = false;

  // AP
  bool cbAutopilotAvailable = false;
  bool cbAutopilotMaster = false;
  bool cbAutopilotWingLeveler = false;
  bool cbAutopilotNav1Lock = false;
  bool cbAutopilotHeadingLock = false;
  bool cbAutopilotAltitudeLock = false;
  bool cbAutopilotAttitudeHold = false;
  bool cbAutopilotGlideslopeHold = false;
  bool cbAutopilotApproachHold = false;
  bool cbAutopilotBackcourseHold = false;
  bool cbAutopilotFlightDirectorActive = false;
  bool cbAutopilotAirspeedHold = false;
  bool cbAutopilotMachHold = false;
  bool cbAutopilotYawDamper = false;
  bool cbAutopilotThrottleArm = false;
  bool cbAutopilotTakeoffPowerActive = false;
  bool cbAutothrottleActive = false;
  bool cbAutopilotVerticalHold = false;
  bool cbAutopilotRpmHold = false;

  bool cbFuelTankCenterLevel = false;
  bool cbFuelTankCenter2Level = false;
  bool cbFuelTankCenter3Level = false;
  bool cbFuelTankLeftMainLevel = false;
  bool cbFuelTankLeftAuxLevel = false;
  bool cbFuelTankLeftTipLevel = false;
  bool cbFuelTankRightMainLevel = false;
  bool cbFuelTankRightAuxLevel = false;
  bool cbFuelTankRightTipLevel = false;
  bool cbFuelTankExternal1Level = false;
  bool cbFuelTankExternal2Level = false;
  bool cbFuelTankCenterCapacity = false;
  bool cbFuelTankCenter2Capacity = false;
  bool cbFuelTankCenter3Capacity = false;
  bool cbFuelTankLeftMainCapacity = false;
  bool cbFuelTankLeftAuxCapacity = false;
  bool cbFuelTankLeftTipCapacity = false;
  bool cbFuelTankRightMainCapacity = false;
  bool cbFuelTankRightAuxCapacity = false;
  bool cbFuelTankRightTipCapacity = false;
  bool cbFuelTankExternal1Capacity = false;
  bool cbFuelTankExternal2Capacity = false;
  bool cbFuelLeftCapacity = false;
  bool cbFuelRightCapacity = false;
  bool cbFuelTankCenterQuantity = false;
  bool cbFuelTankCenter2Quantity = false;
  bool cbFuelTankCenter3Quantity = false;
  bool cbFuelTankLeftMainQuantity = false;
  bool cbFuelTankLeftAuxQuantity = false;
  bool cbFuelTankLeftTipQuantity = false;
  bool cbFuelTankRightMainQuantity = false;
  bool cbFuelTankRightAuxQuantity = false;
  bool cbFuelTankRightTipQuantity = false;
  bool cbFuelTankExternal1Quantity = false;
  bool cbFuelTankExternal2Quantity = false;
  bool cbFuelLeftQuantity = false;
  bool cbFuelRightQuantity = false;
  bool cbFuelTotalQuantity = false;

  bool cbBrakeParkingIndicator = false;

  const char* portName;
  const char* valPort;
  bool abort = false;

 private:
  QString path =
      QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
  QSettings* settings =
      new QSettings(path + "/" + "settings.ini", QSettings::IniFormat);
  int updatePerXFrames = 15;
  std::string lastVal;
  std::string lastStatus;
  SerialPort* arduino;
  static void MyDispatchProcRD(SIMCONNECT_RECV* pData, DWORD cbData,
                               void* pContext);
  void testDataRequest();
};

#endif  // CONNECTWORKER_H
