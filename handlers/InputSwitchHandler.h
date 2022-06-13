#ifndef INPUTSWITCHHANDLER_H
#define INPUTSWITCHHANDLER_H

#include "models/aircraft/Engine.h"
#include "models/SimConnect.h"
#include "constants.h"
#include "models/settings/range.h"
#include <qmutex.h>
#include <qsettings.h>
#include <qstandardpaths.h>
#include <qwaitcondition.h>
#include "models/settings/coordinates.h"
#include "handlers/settingshandler.h"
#include "enums/CurveTypeEnum.h"
#include "models/aircraft/CurveAxis.h"
#include <tchar.h>
#include <windows.h>

#include <QObject>
#include <QThread>
#include <cstdio>
#include <string>

using namespace std;

class InputSwitchHandler {
public:
    InputSwitchHandler();

    void switchHandling(int index);

    char receivedString[10][255];
    HANDLE connect;
    SIMCONNECT_OBJECT_ID object;
    std::array<Axis *, constants::supportedEngines> enginelist;


    float reverseAxis = -23000.0;

    void setRanges();

    void setCurve(QList<coordinates> curve, CurveTypeEnum curveType);

private slots:
    SettingsHandler settingsHandler;

    void setEngineValues(int index);

    void setMixtureValues(int index);

    void set_prop_values(int index);

    int setComs(int index, int comNo);

    void sendBasicCommandOn(SIMCONNECT_CLIENT_EVENT_ID eventID);

    void sendBasicCommandOff(SIMCONNECT_CLIENT_EVENT_ID eventID);


private:
    std::string prefix;

    Axis elevatorTrimAxis = Axis(0, 1023, InputEnum::DEFINITION_ELEVATOR_TRIM_SET);
    Axis mixtureRanges[constants::supportedMixtureLevers];
    Axis propellerRanges[constants::supportedPropellerLevers];
    Axis flapsRange;

    QStringList curveStrings = {"Rudder", "Toe brakes", "Aileron", "Elevator"};

    CurveAxis rudderAxis = CurveAxis(InputEnum::DEFINITION_AXIS_RUDDER_SET);
    CurveAxis leftBrakeAxis = CurveAxis(InputEnum::DEFINITION_AXIS_LEFT_BRAKE_SET);
    CurveAxis rightBrakeAxis = CurveAxis(InputEnum::DEFINITION_AXIS_RIGHT_BRAKE_SET);
    CurveAxis brakeAxis[2] = {leftBrakeAxis, rightBrakeAxis};
    CurveAxis aileronAxis = CurveAxis(InputEnum::DEFINITION_AXIS_AILERONS_SET);
    CurveAxis elevatorAxis = CurveAxis(InputEnum::DEFINITION_AXIS_ELEVATOR_SET);

    int calibratedRange(int value, QList<coordinates> curve);

    void setElevatorTrim(int index);

    void setFlaps(int index);

    void setRudder(int index);

    void setBrakeAxis(int index);

    void sendBasicCommandValue(SIMCONNECT_CLIENT_EVENT_ID eventID, int value);

    void controlYoke(int index);

    void sendBasicCommand(SIMCONNECT_CLIENT_EVENT_ID eventID, int index);

    int mapCoordinates(coordinates toMap);

    int mapCoordinates(coordinates toMapMin, coordinates toMapMax);

    int mapCoordinates(int value, coordinates toMapMin, coordinates toMapMax);

    void sendWASMCommand(SIMCONNECT_CLIENT_EVENT_ID eventID, int index);

    void sendWASMCommand(int index, int value);

    int calibratedRange(int value, QList<coordinates> *curve);

    int calibratedRange(int value, int index);

    void setAxisValue(Axis *axis);

    void mapEngineValueToAxis(Engine *engine) const;

    vector<int> cutInputs(int amountOfPartsNeeded, int index);


    void calibratedRange(CurveAxis *curveAxis);
};

#endif  // INPUTSWITCHHANDLER_H
