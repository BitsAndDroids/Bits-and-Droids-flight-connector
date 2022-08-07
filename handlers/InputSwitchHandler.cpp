/*!
    \class InputSwitchHandler
    \brief The QVector3D class represents a vector or vertex in 3D space.
    \since 4.6

    Vectors are one of the main building blocks of 3D representation and
    drawing.  They consist of three coordinates, traditionally called
    x, y, and z.

    The QVector3D class can also be used to represent vertices in 3D space.
    We therefore do not need to provide a separate vertex class.

    \note By design values in the QVector3D instance are stored as \c float.
    This means that on platforms where the \c qreal arguments to QVector3D
    functions are represented by \c double values, it is possible to
    lose precision
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
#include <QJsonParseError>
#include <QJsonObject>
#include <QJsonArray>
#include <utility>

//#define Bcd2Dec(BcdNum) HornerScheme(BcdNum, 0x10, 10)
#define Dec2Bcd(DecNum) HornerScheme(DecNum, 10, 0x10)

using namespace std;

char *token, *next_token;
bool inputs[5];

int counter = 0;


float closedAxis = -16383.0;
float openAxis = 16383.0;

InputEnum inputDefinitions = InputEnum();

InputSwitchHandler::InputSwitchHandler() {}

InputSwitchHandler::InputSwitchHandler(std::map<int, Input> inputs, HANDLE connect) {
    this->inputs = std::move(inputs);
    this->connect = connect;
    mapInputs();
    setRanges();
}

void InputSwitchHandler::mapInputs() {
    HRESULT hr;
    for (auto &input: inputs) {

        hr = SimConnect_MapClientEventToSimEvent(connect,
                                                 input.second.getPrefix(),
                                                 input.second.getEvent().c_str());

        cout << input.second.getPrefix() << " EVENT " << input.second.getEvent() << " " << to_string(hr) << endl;
    }
}

//TODO unified set implementation

void InputSwitchHandler::setRanges() {
    InputEnum::DATA_DEFINE_ID_INPUT engineEvents[] = {
            inputDefinitions.DATA_EX_THROTTLE_1_AXIS,
            inputDefinitions.DATA_EX_THROTTLE_2_AXIS,
            inputDefinitions.DATA_EX_THROTTLE_3_AXIS,
            inputDefinitions.DATA_EX_THROTTLE_4_AXIS};

    InputEnum::DATA_DEFINE_ID_INPUT mixtureEvents[] = {
            inputDefinitions.DEFINITION_MIXTURE_LEVER_AXIS_1,
            inputDefinitions.DEFINITION_MIXTURE_LEVER_AXIS_2,
            inputDefinitions.DEFINITION_MIXTURE_LEVER_AXIS_3,
            inputDefinitions.DEFINITION_MIXTURE_LEVER_AXIS_4};

    InputEnum::DATA_DEFINE_ID_INPUT propEvents[] = {
            inputDefinitions.DEFINITION_PROP_LEVER_AXIS_1,
            inputDefinitions.DEFINITION_PROP_LEVER_AXIS_2,
            inputDefinitions.DEFINITION_PROP_LEVER_AXIS_3,
            inputDefinitions.DEFINITION_PROP_LEVER_AXIS_4};

    if (!settingsHandler.retrieveSetting("Ranges", "FlapsMin")->isNull()) {
        for (int i = 0; i < constants::supportedEngines; i++) {
            QString minStr = "Engine " + QString::number(i + 1) + "Reverse";

            int minRange = settingsHandler.retrieveSetting("Ranges", minStr)->toInt();
            cout << "MIN SET" << minRange << endl;

            QString idleStr = "Engine " + QString::number(i + 1) + "Idle cutoff";
            int idleCutoff =
                    settingsHandler.retrieveSetting("Ranges", idleStr)->toInt();

            QString maxStr = "Engine " + QString::number(i + 1) + "Max";
            int maxRange = settingsHandler.retrieveSetting("Ranges", maxStr)->toInt();

            enginelist[i] = new Engine(minRange, idleCutoff, maxRange, ENGINE, engineEvents[i]);
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

            mixtureRanges[i] = Axis(minRange, maxRange, mixtureEvents[i]);
        }
        for (int i = 0; i < constants::supportedPropellerLevers; i++) {
            QString minStr = "Propeller " + QString::number(i + 1) + "Min";
            int minRange = settingsHandler.retrieveSetting("Ranges", minStr)->toInt();

            QString idleStr = "Propeller " + QString::number(i + 1) + "Max";
            int maxRange =
                    settingsHandler.retrieveSetting("Ranges", idleStr)->toInt();

            propellerRanges[i] = Axis(minRange, maxRange, propEvents[i]);
        }
        int minFlaps =
                settingsHandler.retrieveSetting("Ranges", "FlapsMin")->toInt();
        int maxFlaps =
                settingsHandler.retrieveSetting("Ranges", "FlapsMax")->toInt();
        flapsRange = Axis(minFlaps, maxFlaps, inputDefinitions.DEFINITION_AXIS_FLAPS_SET);

    } else if (settingsHandler.retrieveSetting("Ranges", "FlapsMin")->isNull()) {
        for (int i = 0; i < constants::supportedEngines; i++) {
            enginelist[i] = new Engine(0, 0, 1023, ENGINE, engineEvents[i]);
        }
        for (int i = 0; i < constants::supportedMixtureLevers; i++) {
            mixtureRanges[i] = Axis(0, 1023, mixtureEvents[i]);
        }
        for (int i = 0; i < constants::supportedPropellerLevers; i++) {
            propellerRanges[i] = Axis(0, 1023, propEvents[i]);
        }

        flapsRange = Axis(0, 1023, inputDefinitions.DEFINITION_AXIS_FLAPS_SET);
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


void InputSwitchHandler::setCurve(QList<coordinates> curve, CurveTypeEnum curveType) {
    CurveAxis *toSet;
    switch (curveType) {
        case RUDDER:
            toSet = &rudderAxis;
            break;
        case LEFTBRAKE:
            toSet = &brakeAxis[0];
            break;
        case RIGHTBRAKE:
            toSet = &brakeAxis[1];
            break;
        case AILERON:
            toSet = &aileronAxis;
            break;
        case ELEVATOR:
            toSet = &elevatorAxis;
            break;
    }
    toSet->clearCurve();
    toSet->setCurve(curve);
}

void InputSwitchHandler::mapEngineValueToAxis(Engine *engine) const {
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
    if ((reversed && idle - min < 0 && (float) engine->getCurrentValue() >= idle) ||
        (!reversed && idle - min > 0 && (float) engine->getCurrentValue() <= idle)) {
        valueThrottle = (int) (reverseAxis + (closedAxis - reverseAxis) *
                                             (((float) engine->getCurrentValue() - min) / (idle - min)));
    } else {
        valueThrottle = (int) (closedAxis +
                               (openAxis - closedAxis) * (((float) engine->getCurrentValue() - idle) / (max - idle)));
    }
    if (valueThrottle > 16383) {
        valueThrottle = 16383;
    }
    engine->setMappedValue(valueThrottle);
}

void mapValueToAxis(Axis *axis) {
    axis->setMappedValue((int)
                                 (closedAxis + (openAxis - closedAxis) *
                                               (((float) axis->getCurrentValue() - axis->getMin()) /
                                                (axis->getMax() - axis->getMin()))));
}

std::vector<int> InputSwitchHandler::cutInputs(int amountOfPartsNeeded, int index) {
    std::vector<int> parts;

    try {
        token = strtok_s(receivedString[index], " ", &next_token);
        cout << receivedString[index] << endl;
        counter = 0;
        while (token != nullptr && counter < amountOfPartsNeeded + 1) {
            if (token != nullptr) {
                const auto incVal = strtod(token, nullptr);
                if (counter != 0) {
                    parts.push_back((int) incVal);
                }
                token = strtok_s(nullptr, " ", &next_token);
                counter++;
            }
        }
    }
    catch (const std::exception &e) {
        cout << "error in cutInputs()" << endl;
    }
    if (parts.size() == amountOfPartsNeeded) {
        return parts;
    }
    parts.clear();
    cout << "SIZE OF PARTS: " << parts.size() << endl;
    return parts;
}

void InputSwitchHandler::calibratedRange(CurveAxis *curveAxis) {
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
    } else if (static_cast<float>(value) <= curveAxis->getCoordinates(5).getX()) {
        axis = mapCoordinates(static_cast<float>(value), curveAxis->getCoordinates(4),
                              curveAxis->getCoordinates(5));
    } else if (static_cast<float>(value) <= curveAxis->getCoordinates(6).getX()) {
        axis = mapCoordinates(static_cast<float>(value), curveAxis->getCoordinates(5),
                              curveAxis->getCoordinates(6));
    }
    curveAxis->setMappedValue(axis);
}

void InputSwitchHandler::setAxisValue(Axis *axis) {
    int value = axis->getCurrentValue();
    int oldValue = axis->getOldValue();
    if (value < 10) {
        if (oldValue < 20) {
            axis->setOldValue(value);
        }
    } else {

        if (value == 98) {
            axis->setCurrentValue(100);
        }
        axis->setOldValue(value);

    }
    if (axis->getType() == ENGINE) {
        mapEngineValueToAxis((Engine *) (axis));
    } else {
        mapValueToAxis(axis);
    }

}

void InputSwitchHandler::setEngineValues(int index) {
    // Throttle control
    std::vector<int> engineBuffer = cutInputs(constants::supportedEngines, index);
    cout << "BUFFERSIZE: " << engineBuffer.size() << std::endl;
    if (engineBuffer.size() == constants::supportedEngines) {
        for (int i = 0; i < constants::supportedEngines; i++) {
            cout << "ENGINE " << i << " " << engineBuffer[i] << endl;
            enginelist[i]->setCurrentValue(engineBuffer.at(i));
            setAxisValue(enginelist[i]);
            sendBasicCommandValue(enginelist[i]->getEvent(), enginelist[i]->getMappedValue());
        }
    }
}

void InputSwitchHandler::setFlaps(int index) {
    std::vector<int> flapsBuffer = cutInputs(1, index);

    if (flapsBuffer.size() == 1) {
        flapsRange.setCurrentValue(flapsBuffer[0]);
        setAxisValue(&flapsRange);
        sendBasicCommandValue(flapsRange.getEvent(), flapsRange.getMappedValue());
    }
}


void InputSwitchHandler::controlYoke(int index) {
    std::vector<int> yokeBuffer = cutInputs(2, index);
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


void InputSwitchHandler::setMixtureValues(int index) {
    //TODO support 4 mixture handles
    std::vector<int> mixtureBuffer = cutInputs(2, index);
    if (mixtureBuffer.size() == 2) {
        for (int i = 0; i < 2; i++) {
            mixtureRanges[i].setCurrentValue(mixtureBuffer.at(i));
            setAxisValue(&mixtureRanges[i]);
            sendBasicCommandValue(mixtureRanges[i].getEvent(), mixtureRanges[i].getMappedValue());
            sendBasicCommandValue(mixtureRanges[i + 2].getEvent(), mixtureRanges[i].getMappedValue());
        }
    }
}


void InputSwitchHandler::set_prop_values(int index) {
    //TODO support 4 prop handles
    std::vector<int> propBuffer = cutInputs(2, index);
    if (propBuffer.size() == 2) {
        for (int i = 0; i < 2; i++) {
            propellerRanges[i].setCurrentValue(propBuffer.at(i));
            setAxisValue(&propellerRanges[i]);
            sendBasicCommandValue(propellerRanges[i].getEvent(), propellerRanges[i].getMappedValue());
            sendBasicCommandValue(propellerRanges[i + 2].getEvent(), propellerRanges[i].getMappedValue());
        }
    }
}

void InputSwitchHandler::setElevatorTrim(int index) {
    std::vector<int> elevatorTrimBuffer = cutInputs(1, index);
    if (elevatorTrimBuffer.size() == 1) {
        elevatorTrimAxis.setCurrentValue(elevatorTrimBuffer.at(0));
        int diff = std::abs(elevatorTrimAxis.getMappedValue() - elevatorTrimAxis.getOldMappedValue());
        if (diff < 5000 || elevatorTrimAxis.getOldMappedValue() == NULL) {
            sendBasicCommandValue(elevatorTrimAxis.getEvent(), elevatorTrimAxis.getMappedValue());
            elevatorTrimAxis.setOldMappedValue(elevatorAxis.getCurrentValue());
        }
    }
}


void InputSwitchHandler::setRudder(int index) {
    std::vector<int> rudderBuffer = cutInputs(1, index);
    if (rudderBuffer.size() == 1) {

        rudderAxis.setCurrentValue(rudderBuffer.at(0));
        calibratedRange(&rudderAxis);
        cout << "RUDDER: " << rudderAxis.getMappedValue() << endl;
        int diff = std::abs(rudderAxis.getMappedValue() - rudderAxis.getOldMappedValue());
        if (diff < 10000 || rudderAxis.getOldMappedValue() == NULL) {
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

void InputSwitchHandler::setBrakeAxis(int index) {
    std::vector<int> brakeBuffer = cutInputs(2, index);
    if (brakeBuffer.size() == 2) {
        for (auto &brakeAxi: brakeAxis) {
            //TODO support seperate axis calibration
            brakeAxi.setCurrentValue(brakeBuffer.at(0));
            calibratedRange(&brakeAxi);
            int diff = std::abs(brakeAxi.getMappedValue() - brakeAxi.getOldMappedValue());
            if (diff < 10000 || brakeAxi.getOldMappedValue() == NULL) {
                sendBasicCommandValue(brakeAxi.getEvent(), brakeAxi.getMappedValue());
                brakeAxi.setOldMappedValue(brakeAxi.getCurrentValue());
            }
        }

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
                        125, 0,
                        SIMCONNECT_GROUP_PRIORITY_HIGHEST,
                        SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
            }
            case 2: {
                SimConnect_TransmitClientEvent(
                        connect, SIMCONNECT_OBJECT_ID_USER,
                        121, 0,
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
    if (sizeTest.size() == 6 || sizeTest.size() == 5) {
        cout << "SENDING COMMAND" << inputs[eventID].getEvent() << endl;
        hr = SimConnect_TransmitClientEvent(
                connect, 0, (SIMCONNECT_CLIENT_EVENT_ID) eventID, 0, SIMCONNECT_GROUP_PRIORITY_HIGHEST,
                SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
    }
    cout << "HR " << hr << endl;
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
    Sleep(2);
    //

    if (strlen(receivedString[index]) > 2) {
        try {

            int prefixValue = stoi(std::string(&receivedString[index][0], &receivedString[index][4]));


            if (inputs.count(prefixValue) > 0) {
                Input input = inputs[prefixValue];
                emit logMessage(
                        "Received data: " + std::string(receivedString[index]) +" command: " + input.getEvent(),
                        LogLevel::DEBUGLOG);
                if (input.getType() == 4) {
                    sendBasicCommandOff(input.getPrefix());
                } else if (input.getType() == 5) {
                    sendBasicCommandOn(input.getPrefix());
                }
                sendBasicCommand((SIMCONNECT_CLIENT_EVENT_ID) input.getPrefix(), index);

            } else {
                    switch (prefixValue) {
                        case 198: {
                            set_prop_values(index);
                            break;
                        }

                        case 199: {
                            setEngineValues(index);
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
                                    100, setComs(index, 1),
                                    SIMCONNECT_GROUP_PRIORITY_HIGHEST,
                                    SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);

                            break;
                        }
                        case 102: {
                            int value = stoi(std::string(&receivedString[index][4],
                                                         &receivedString[index][10]));
                            SimConnect_TransmitClientEvent(
                                    connect, SIMCONNECT_OBJECT_ID_USER,
                                    102, Dec2Bcd(value / 10),
                                    SIMCONNECT_GROUP_PRIORITY_HIGHEST,
                                    SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
                            if (value % (value / 10) == 5 || value % (value / 10) == 85) {
                                SimConnect_TransmitClientEvent(
                                        connect, SIMCONNECT_OBJECT_ID_USER,
                                        121, 0,
                                        SIMCONNECT_GROUP_PRIORITY_HIGHEST,
                                        SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
                            }
                            break;
                        }


                        case 202: {
                            std::string value =
                                    std::string(&receivedString[index][4], &receivedString[index][8]);
                            SimConnect_TransmitClientEvent(
                                    connect, 0, 202,
                                    Dec2Bcd(stoi(value)), SIMCONNECT_GROUP_PRIORITY_HIGHEST,
                                    SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
                            break;
                        }

                            // TO DO FUNCTION ON OFF Battery
                        case 405: {
                            SimConnect_TransmitClientEvent(
                                    connect, 0, 405, 1,
                                    SIMCONNECT_GROUP_PRIORITY_HIGHEST,
                                    SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
                            break;
                        }
                        case 406: {
                            SimConnect_TransmitClientEvent(
                                    connect, 0, 406, 2,
                                    SIMCONNECT_GROUP_PRIORITY_HIGHEST,
                                    SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
                            break;
                        }

                        case 900: {
                            setElevatorTrim(index);
                            break;
                        }
                        case 901: {
                            cout << "901" << endl;
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

                            sendWASMCommand(prefixValue, value);
                            cout << value << "val" << endl;
                            break;
                        }

                    }
                }
        }
        catch (std::exception &e) {
            emit logMessage(
                    "Received data: " + std::string(receivedString[index]) ,
                    LogLevel::ERRORLOG);
            cout << e.what() << endl;
        }
    }
}



