/*!
 * \defgroup MFS2020Interaction
 */

/*!
    \class InputSwitchHandler
    \ingroup MFS2020Interaction
    \brief The InputSwitchHandler class is responsible for handling the input commands.
    The input commands are sent from the MFSWorker class.
    It uses the received prefix from a microncontroller over serial port to determine which input command to handle.
*/

#include "InputSwitchHandler.h"

#include <qlist.h>
#include <windows.h>
#include <iostream>
#include <string>

#include "enums/inputenum.h"
#include "enums/CurveTypeEnum.h"
#include <models/aircraft/Axis.h>
#include <QFile>
#include <QJsonObject>
#include <utility>
#include <QDebug>

//#define Bcd2Dec(BcdNum) HornerScheme(BcdNum, 0x10, 10)
#define Dec2Bcd(DecNum) HornerScheme(DecNum, 10, 0x10)

using namespace std;

InputEnum inputDefinitions = InputEnum();

InputSwitchHandler::InputSwitchHandler() {
}

InputSwitchHandler::InputSwitchHandler(std::map<int, Input> inputs, HANDLE connect) {
    this->inputs = std::move(inputs);
    this->connect = connect;
    mapInputs();
    setRanges();
}

void InputSwitchHandler::mapInputs() {
    HRESULT hr;
    for (auto&input: inputs) {
        hr = SimConnect_MapClientEventToSimEvent(connect,
                                                 input.second.getPrefix(),
                                                 input.second.getEvent().c_str());
    }
}

//TODO unified Set implementation

void InputSwitchHandler::setRanges() {
    InputEnum::DATA_DEFINE_ID_INPUT engineEvents[] = {
        inputDefinitions.DATA_EX_THROTTLE_1_AXIS,
        inputDefinitions.DATA_EX_THROTTLE_2_AXIS,
        inputDefinitions.DATA_EX_THROTTLE_3_AXIS,
        inputDefinitions.DATA_EX_THROTTLE_4_AXIS
    };

    InputEnum::DATA_DEFINE_ID_INPUT mixtureEvents[] = {
        inputDefinitions.DEFINITION_MIXTURE_LEVER_AXIS_1,
        inputDefinitions.DEFINITION_MIXTURE_LEVER_AXIS_2,
        inputDefinitions.DEFINITION_MIXTURE_LEVER_AXIS_3,
        inputDefinitions.DEFINITION_MIXTURE_LEVER_AXIS_4
    };

    InputEnum::DATA_DEFINE_ID_INPUT propEvents[] = {
        inputDefinitions.DEFINITION_PROP_LEVER_AXIS_1,
        inputDefinitions.DEFINITION_PROP_LEVER_AXIS_2,
        inputDefinitions.DEFINITION_PROP_LEVER_AXIS_3,
        inputDefinitions.DEFINITION_PROP_LEVER_AXIS_4
    };

    if (!settingsHandler.getSettingValue("Ranges", "FlapsMin")->isNull()) {
        for (int i = 0; i < constants::supportedEngines; i++) {
            QString minStr = "Engine " + QString::number(i + 1) + "Reverse";

            int minRange = settingsHandler.getSettingValue("Ranges", minStr)->toInt();
            qDebug() << "MIN SET" << minRange;

            QString idleStr = "Engine " + QString::number(i + 1) + "Idle cutoff";
            int idleCutoff =
                    settingsHandler.getSettingValue("Ranges", idleStr)->toInt();

            QString maxStr = "Engine " + QString::number(i + 1) + "Max";
            int maxRange = settingsHandler.getSettingValue("Ranges", maxStr)->toInt();

            enginelist[i] = new Engine(minRange, idleCutoff, maxRange, ENGINE, engineEvents[i]);
        }

        if (!settingsHandler.getSettingValue("Ranges", "maxReverseRange")
            ->isNull()) {
            reverseAxis =
                    settingsHandler.getSettingValue("Ranges", "maxReverseRange")
                    ->toFloat();
        }

        for (int i = 0; i < constants::supportedMixtureLevers; i++) {
            QString minStr = "Mixture " + QString::number(i + 1) + "Min";
            int minRange = settingsHandler.getSettingValue("Ranges", minStr)->toInt();
            qDebug() << minRange;

            QString idleStr = "Mixture " + QString::number(i + 1) + "Max";
            int maxRange =
                    settingsHandler.getSettingValue("Ranges", idleStr)->toInt();

            mixtureRanges[i] = Axis(minRange, maxRange, mixtureEvents[i]);
        }
        for (int i = 0; i < constants::supportedPropellerLevers; i++) {
            QString minStr = "Propeller " + QString::number(i + 1) + "Min";
            int minRange = settingsHandler.getSettingValue("Ranges", minStr)->toInt();

            QString idleStr = "Propeller " + QString::number(i + 1) + "Max";
            int maxRange =
                    settingsHandler.getSettingValue("Ranges", idleStr)->toInt();

            propellerRanges[i] = Axis(minRange, maxRange, propEvents[i]);
        }
        int minFlaps =
                settingsHandler.getSettingValue("Ranges", "FlapsMin")->toInt();
        int maxFlaps =
                settingsHandler.getSettingValue("Ranges", "FlapsMax")->toInt();
        flapsRange = Axis(minFlaps, maxFlaps, inputDefinitions.DEFINITION_AXIS_FLAPS_SET);
    }
    else if (settingsHandler.getSettingValue("Ranges", "FlapsMin")->isNull()) {
        for (int i = 0; i < constants::supportedEngines; i++) {
            enginelist[i] = new Engine(0, 0, 1023, ENGINE, engineEvents[i]);
        }
        for (int i = 0; i < constants::supportedMixtureLevers; i++) {
            mixtureRanges[i] = Axis(0, 1023, mixtureEvents[i]);
        }
        for (int i = 0; i < constants::supportedPropellerLevers; i++) {
            propellerRanges[i] = Axis(0, 1023, propEvents[i]);
        }

        flapsRange = Axis(0, 1023, InputEnum::DEFINITION_AXIS_FLAPS_SET);
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

void InputSwitchHandler::setCurveBasedOnType(QList<coordinates> curve, CurveTypeEnum curveType) {
    CurveAxis* axisToSet = getAxisToSet(curveType);
    clearAndSetCurve(axisToSet, std::move(curve));
}

CurveAxis* InputSwitchHandler::getAxisToSet(CurveTypeEnum curveType) {
    switch (curveType) {
        case RUDDER: return &rudderAxis;
        case LEFTBRAKE: return &brakeAxis[0];
        case RIGHTBRAKE: return &brakeAxis[1];
        case AILERON: return &aileronAxis;
        case ELEVATOR: return &elevatorAxis;
        default: return nullptr;
    }
}

void InputSwitchHandler::clearAndSetCurve(CurveAxis* axis, QList<coordinates> curve) {
    axis->clearCurve();
    axis->setCurve(curve);
}

void InputSwitchHandler::mapEngineValueToAxis(Engine* engine) const {
    int valueThrottle;
    float idle = engine->getIdleIndex();
    float min = engine->getMin();
    float max = engine->getMax();
    bool reversed = max < idle;

    /*First we check the orientation by determing if max < idleCutoff
  If max < idleCutoff. IF max is smaller we know the potentiometer is mounted
  backwards This affects how our logic needs to operate We want to check if the
  idle cutoff - reverse < 0 This check tells us wether or not the user wants to
  utilize the reverse range or not visa versa for the second check */
    if ((reversed && idle - min < 0 && (float)engine->getCurrentValue() >= idle) ||
        (!reversed && idle - min > 0 && (float)engine->getCurrentValue() <= idle)) {
        valueThrottle = (int)(reverseAxis + (closedAxis - reverseAxis) *
                              (((float)engine->getCurrentValue() - min) / (idle - min)));
    }
    else {
        valueThrottle = (int)(closedAxis +
                              (openAxis - closedAxis) * (((float)engine->getCurrentValue() - idle) / (max - idle)));
    }
    if (valueThrottle > 16383) {
        valueThrottle = 16383;
    }
    engine->setMappedValue(valueThrottle);
}

void InputSwitchHandler::mapValueToAxis(Axis* axis) const {
    axis->setMappedValue((int)
    (closedAxis + (openAxis - closedAxis) *
     (((float)axis->getCurrentValue() - axis->getMin()) /
      (axis->getMax() - axis->getMin()))));
}

std::vector<int> InputSwitchHandler::cutInputs(int amountOfPartsNeeded, std::string string) {
    std::vector<int> parts;
    std::string delimiter = " ";
    try {
        int counter = 0;
        size_t pos = 0;
        std::string tokenFound;
        while ((pos = string.find(delimiter)) != std::string::npos) {
            tokenFound = string.substr(0, pos);
            if (counter != 0) {
                parts.push_back(std::stoi(tokenFound));
            }
            string.erase(0, pos + delimiter.length());
            counter++;
        }
    }
    catch (const std::exception&e) {
        qDebug() << "error in cutInputs()";
    }
    if (parts.size() == amountOfPartsNeeded) {
        return parts;
    }
    parts.clear();
    qDebug() << "SIZE OF PARTS: " << parts.size();
    return parts;
}

void InputSwitchHandler::calibratedRange(CurveAxis* curveAxis) {
    int axis;
    int value = curveAxis->getCurrentValue();
    if (static_cast<float>(value) <= curveAxis->getCoordinates(1).getX()) {
        axis = mapCoordinates(value, curveAxis->getCoordinates(0), curveAxis->getCoordinates(1));
    }
    // minCurve
    else if (static_cast<float>(value) < curveAxis->getCoordinates(2).getX()) {
        axis = mapCoordinates(value, curveAxis->getCoordinates(1), curveAxis->getCoordinates(2));
    }
    // deadzone
    else if (static_cast<float>(value) >= curveAxis->getCoordinates(2).getX() &&
             static_cast<float>(value) <= curveAxis->getCoordinates(4).getX()) {
        axis = 0;
    }
    else if (static_cast<float>(value) <= curveAxis->getCoordinates(5).getX()) {
        axis = mapCoordinates(static_cast<float>(value), curveAxis->getCoordinates(4),
                              curveAxis->getCoordinates(5));
    }
    else if (static_cast<float>(value) <= curveAxis->getCoordinates(6).getX()) {
        axis = mapCoordinates(static_cast<float>(value), curveAxis->getCoordinates(5),
                              curveAxis->getCoordinates(6));
    }
    curveAxis->setMappedValue(axis);
}

void InputSwitchHandler::setAxisValue(Axis* axis) {
    const int value = axis->getCurrentValue();
    const int oldValue = axis->getOldValue();
    if (value < 10) {
        if (oldValue < 20) {
            axis->setOldValue(value);
        }
    }
    else {
        if (value == 98) {
            axis->setCurrentValue(100);
        }
        axis->setOldValue(value);
    }
    if (axis->getType() == ENGINE) {
        mapEngineValueToAxis((Engine *)(axis));
    }
    else {
        mapValueToAxis(axis);
    }
}

void InputSwitchHandler::setEngineValues(std::string stringToSet) {
    // Throttle control
    std::vector<int> engineBuffer = cutInputs(constants::supportedEngines, std::move(stringToSet));
    if (engineBuffer.size() == constants::supportedEngines) {
        for (int i = 0; i < constants::supportedEngines; i++) {
            enginelist[i]->setCurrentValue(engineBuffer.at(i));
            setAxisValue(enginelist[i]);
            sendBasicCommandValue(enginelist[i]->getEvent(), enginelist[i]->getMappedValue());
        }
    }
}

void InputSwitchHandler::setFlaps(std::string stringToSet) {
    std::vector<int> flapsBuffer = cutInputs(1, std::move(stringToSet));

    if (flapsBuffer.size() == 1) {
        flapsRange.setCurrentValue(flapsBuffer[0]);
        setAxisValue(&flapsRange);
        sendBasicCommandValue(flapsRange.getEvent(), flapsRange.getMappedValue());
    }
}


void InputSwitchHandler::controlYoke(std::string stringToSet) {
    std::vector<int> yokeBuffer = cutInputs(2, std::move(stringToSet));
    if (yokeBuffer.size() == 2) {
        elevatorAxis.setCurrentValue(yokeBuffer[0]);
        aileronAxis.setCurrentValue(yokeBuffer[1]);
        calibratedRange(&elevatorAxis);
        calibratedRange(&aileronAxis);
        sendBasicCommandValue(elevatorAxis.getEvent(),
                              elevatorAxis.getMappedValue());
        sendBasicCommandValue(aileronAxis.getEvent(),
                              aileronAxis.getMappedValue());
    }
}


void InputSwitchHandler::setMixtureValues(std::string stringToSet) {
    //TODO support 4 mixture handles
    std::vector<int> mixtureBuffer = cutInputs(2, std::move(stringToSet));
    if (mixtureBuffer.size() == 2) {
        for (int i = 0; i < 2; i++) {
            mixtureRanges[i].setCurrentValue(mixtureBuffer.at(i));
            setAxisValue(&mixtureRanges[i]);
            sendBasicCommandValue(mixtureRanges[i].getEvent(), mixtureRanges[i].getMappedValue());
            sendBasicCommandValue(mixtureRanges[i + 2].getEvent(), mixtureRanges[i].getMappedValue());
        }
    }
}


void InputSwitchHandler::set_prop_values(std::string stringToSet) {
    //TODO support 4 prop handles
    std::vector<int> propBuffer = cutInputs(2, std::move(stringToSet));
    if (propBuffer.size() == 2) {
        for (int i = 0; i < 2; i++) {
            propellerRanges[i].setCurrentValue(propBuffer.at(i));
            setAxisValue(&propellerRanges[i]);
            sendBasicCommandValue(propellerRanges[i].getEvent(), propellerRanges[i].getMappedValue());
            sendBasicCommandValue(propellerRanges[i + 2].getEvent(), propellerRanges[i].getMappedValue());
        }
    }
}

void InputSwitchHandler::setElevatorTrim(std::string stringToSet) {
    std::vector<int> elevatorTrimBuffer = cutInputs(1, std::move(stringToSet));
    if (elevatorTrimBuffer.size() == 1) {
        elevatorTrimAxis.setCurrentValue(elevatorTrimBuffer.at(0));
        int diff = std::abs(elevatorTrimAxis.getMappedValue() - elevatorTrimAxis.getOldMappedValue());
        if (diff < 5000) {
            sendBasicCommandValue(elevatorTrimAxis.getEvent(), elevatorTrimAxis.getMappedValue());
            elevatorTrimAxis.setOldMappedValue(elevatorAxis.getCurrentValue());
        }
    }
}


void InputSwitchHandler::setRudder(std::string stringToSet) {
    std::vector<int> rudderBuffer = cutInputs(1, std::move(stringToSet));
    if (rudderBuffer.size() == 1) {
        rudderAxis.setCurrentValue(rudderBuffer.at(0));
        calibratedRange(&rudderAxis);
        qDebug() << "RUDDER: " << rudderAxis.getMappedValue();
        int diff = std::abs(rudderAxis.getMappedValue() - rudderAxis.getOldMappedValue());
        if (diff < 10000) {
            sendBasicCommandValue(rudderAxis.getEvent(), rudderAxis.getMappedValue());
            rudderAxis.setOldMappedValue(rudderAxis.getCurrentValue());
        }
    }
}

int InputSwitchHandler::mapCoordinates(int value, coordinates toMapMin,
                                       coordinates toMapMax) {
    return toMapMin.getY() +
           (toMapMax.getY() - toMapMin.getY()) *
           ((value - toMapMin.getX()) / (toMapMax.getX() - toMapMin.getX()));
}

void InputSwitchHandler::setBrakeAxis(std::string stringToSet) {
    std::vector<int> brakeBuffer = cutInputs(2, std::move(stringToSet));
    if (brakeBuffer.size() == 2) {
        for (auto&brakeAxi: brakeAxis) {
            //TODO support seperate axis calibration
            brakeAxi.setCurrentValue(brakeBuffer.at(0));
            calibratedRange(&brakeAxi);
            int diff = std::abs(brakeAxi.getMappedValue() - brakeAxi.getOldMappedValue());
            if (diff < 10000) {
                sendBasicCommandValue(brakeAxi.getEvent(), brakeAxi.getMappedValue());
                brakeAxi.setOldMappedValue(brakeAxi.getCurrentValue());
            }
        }
    }
}

//make this generic using the code below
//
void InputSwitchHandler::setComCase(std::string s, int command) {
    const std::map<int, int> uptickLookupTable = {
        {102, 121},
        {100, 122}
    };
    int value = stoi(s.substr(4, 10));
    emit logMessage("Converted data com 2: " + Dec2Bcd(value / 10), LogLevel::DEBUGLOG);
    transmitEvent(102, Dec2Bcd(value / 10));
    //Tick the value up ingame to match the current state
    //this is due to the way the game syncs the khz
    if (value % (value / 10) == 5 || value % (value / 10) == 85) {
        transmitEvent(121, 0);
    }
}

int InputSwitchHandler::setComs(const std::string&stringToSet, int comNo) const {
    int value =
            stoi(stringToSet.substr(4, 10));
    HRESULT result = 0;
    if (value % (value / 10) == 5 || value % (value / 10) == 85) {
        switch (comNo) {
            case 1: {
                result = SimConnect_TransmitClientEvent(
                    connect, SIMCONNECT_OBJECT_ID_USER,
                    125, 0,
                    SIMCONNECT_GROUP_PRIORITY_HIGHEST,
                    SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
            }
            case 2: {
                result = SimConnect_TransmitClientEvent(
                    connect, SIMCONNECT_OBJECT_ID_USER,
                    121, 0,
                    SIMCONNECT_GROUP_PRIORITY_HIGHEST,
                    SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
            }
            default: {
            }
        }
    }
    if (result != 0) { return Dec2Bcd(value / 10); }
}

void InputSwitchHandler::sendBasicCommand(SIMCONNECT_CLIENT_EVENT_ID eventID,
                                          const std::string&stringToSet) {
    HRESULT hr;

    if (stringToSet.size() == 6 || stringToSet.size() == 5) {
        emit logMessage(inputs[(int)eventID].getEvent(), LogLevel::DEBUGLOG);
        hr = SimConnect_TransmitClientEvent(
            connect, 0, (SIMCONNECT_CLIENT_EVENT_ID)eventID, 0, SIMCONNECT_GROUP_PRIORITY_HIGHEST,
            SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
    }
}

void InputSwitchHandler::sendWASMCommand(int prefixValue, int value) {
    std::string testString = std::to_string(prefixValue) + " " + std::to_string(value);

    char arrayTest[256];
    strcpy(arrayTest, testString.c_str());
    puts(arrayTest);
    emit logMessage("Sending data to WASM: " + std::string(arrayTest),
                    LogLevel::DEBUGLOG);
    qDebug() << arrayTest;

    SimConnect_SetClientData(connect, 1, 12,
                             SIMCONNECT_CLIENT_DATA_SET_FLAG_DEFAULT, 0, 256,
                             &arrayTest);
}

void InputSwitchHandler::sendBasicCommandOn(
    SIMCONNECT_CLIENT_EVENT_ID eventID) const {
    SimConnect_TransmitClientEvent(connect, 0, eventID, 1,
                                   SIMCONNECT_GROUP_PRIORITY_HIGHEST,
                                   SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
}

void InputSwitchHandler::sendBasicCommandOff(
    SIMCONNECT_CLIENT_EVENT_ID eventID) const {
    SimConnect_TransmitClientEvent(connect, 0, eventID, 0,
                                   SIMCONNECT_GROUP_PRIORITY_HIGHEST,
                                   SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
}

void InputSwitchHandler::sendBasicCommandValue(
    SIMCONNECT_CLIENT_EVENT_ID eventID, int value) {
    emit logMessage("Sending value: " + std::to_string(value), LogLevel::DEBUGLOG);
    SimConnect_TransmitClientEvent(connect, 0, eventID, value,
                                   SIMCONNECT_GROUP_PRIORITY_HIGHEST,
                                   SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
}

void InputSwitchHandler::transmitEvent(DWORD eventID, DWORD data) {
    SimConnect_TransmitClientEvent(
        connect,
        SIMCONNECT_OBJECT_ID_USER,
        eventID,
        data,
        SIMCONNECT_GROUP_PRIORITY_HIGHEST,
        SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
}



void InputSwitchHandler::standardCase(std::string s) {
    std::string value = s.substr(4, 8);
    transmitEvent(202, Dec2Bcd(stoi(value)));
}

void InputSwitchHandler::transmitEvent(DWORD eventID, DWORD data) const {
    SimConnect_TransmitClientEvent(
        connect,
        SIMCONNECT_OBJECT_ID_USER,
        eventID,
        data,
        SIMCONNECT_GROUP_PRIORITY_HIGHEST,
        SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
};

void InputSwitchHandler::simpleCase(int caseNumber, int data) {
    // determine the data value based on your logic or requirement
    transmitEvent(caseNumber, data);
}

/*!
 * \fn InputSwitchHandler::switchHandling(const char* stringToParse)
 * \brief this function triggers the appropriate input handling function
 * Depending on the incomming string this function will parse and fire the appropriate input function.
 * Input types are determined in the Input.json file. Types are defined in the InputTypeEnum.
 * \sa InputTypeEnum
 * @param stringToParse the string that has been sent by a microcontroller
 */
void InputSwitchHandler::switchHandling(const char* stringToParse) {
    // Default handler
    auto defaultFunc = [&](std::string s) {
        int value = 0;
        bool valFound = s.size() > 6;
        if (valFound) {
            value = stoi(s.substr(4));
        }
        sendWASMCommand(0, value);
    };

    Sleep(2);
    //
    std::string stringStd = stringToParse;

    if (strlen(stringToParse) > 2) {
        try {
            if (const int prefixValue = stoi(stringStd.substr(0, 4));
                prefixValue < 1000 && inputs.count(prefixValue) > 0) {
                const Input input = inputs[prefixValue];
                emit logMessage(
                    "Received data: " + std::string(stringToParse) + " command: " + input.getEvent(),
                    DEBUGLOG);
                switch (input.getType()) {
                    case BASICCOMMAND: {
                        sendBasicCommand(input.getPrefix(), stringStd);
                        break;
                    }
                    case INPUTOFF: {
                        sendBasicCommandOff(input.getPrefix());
                        break;
                    }
                    case INPUTON: {
                        sendBasicCommandOn(input.getPrefix());
                        break;
                    }
                    case SETCOMS: {
                        const int value = Dec2Bcd((stoi(stringStd.substr(4,10))) / 10);
                        sendBasicCommandValue(input.getPrefix(), value);
                        break;
                    }
                    case SETINT: {
                        sendBasicCommandValue(input.getPrefix(), stoi(stringStd.substr(4, 10)));
                        break;
                    }
                    default: {
                        sendBasicCommand(static_cast<SIMCONNECT_CLIENT_EVENT_ID>(input.getPrefix()), stringToParse);
                        break;
                    }
                }
            }
            else {
                if (funcMap.find(prefixValue) != funcMap.end()) {
                    funcMap[prefixValue](stringToParse);
                }
                else {
                    defaultFunc(stringToParse);
                }
            }
        }
        catch (std::exception&e) {
            emit logMessage(
                "Received data: " + std::string(stringStd) + " " + e.what(),
                LogLevel::ERRORLOG);
        }
    }


    // Use the function map
}
