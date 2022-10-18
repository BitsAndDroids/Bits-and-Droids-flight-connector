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
#include "settings/settingshandler.h"
#include "enums/CurveTypeEnum.h"
#include "models/aircraft/CurveAxis.h"
#include "models/commands/Input.h"
#include "enums/LogLevelEnum.h"
#include <tchar.h>
#include <windows.h>

#include <QObject>
#include <QThread>
#include <cstdio>
#include <string>

using namespace std;

class InputSwitchHandler: public QObject {
    Q_OBJECT
public:
    InputSwitchHandler();

    InputSwitchHandler(std::map<int, Input> inputs, HANDLE connect);

    void switchHandling(const char* stringToParse);

    HANDLE connect;
    SIMCONNECT_OBJECT_ID object;
    std::array<Axis *, constants::supportedEngines> enginelist;


    float reverseAxis = -23000.0;

    void setRanges();

    void setCurve(QList<coordinates> curve, CurveTypeEnum curveType);
signals:
    void logMessage(std::string message, LogLevel level);

private:
    float closedAxis = -16383.0;
    float openAxis = 16383.0;
    std::map<int, Input> inputs;
    std::string prefix;
    SettingsHandler settingsHandler;
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

    void setEngineValues(std::string stringToSet);

    void setMixtureValues(std::string stringToSet);

    void set_prop_values(std::string stringToSet);

    int setComs(const std::string& stringToSet, int comNo) const;

    void sendBasicCommandOn(SIMCONNECT_CLIENT_EVENT_ID eventID) const;

    void sendBasicCommandOff(SIMCONNECT_CLIENT_EVENT_ID eventID) const;


    void setElevatorTrim(std::string stringToSet);

    void setFlaps(std::string stringToSet);

    void setRudder(std::string stringToSet);

    void setBrakeAxis(std::string stringToSet);

    void sendBasicCommandValue(SIMCONNECT_CLIENT_EVENT_ID eventID, int value);

    void controlYoke(std::string stringToSet);

    void sendBasicCommand(SIMCONNECT_CLIENT_EVENT_ID eventID, const std::string& stringToSet);

    static int mapCoordinates(int value, coordinates toMapMin, coordinates toMapMax);

    void sendWASMCommand(int prefix, int value) const;

    void setAxisValue(Axis *axis);

    void mapEngineValueToAxis(Engine *engine) const;

    static vector<int> cutInputs(int amountOfPartsNeeded, std::string stringToSet);

    static void calibratedRange(CurveAxis *curveAxis);

    void mapInputs();

    void mapValueToAxis(Axis *axis) const;
};

#endif  // INPUTSWITCHHANDLER_H
