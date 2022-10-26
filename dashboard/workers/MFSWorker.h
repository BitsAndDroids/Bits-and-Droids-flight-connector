#ifndef DUALWORKER_H
#define DUALWORKER_H

#include "workers/InputMapper.h"
#include "handlers/InputSwitchHandler.h"
#include "handlers/outputhandler.h"
#include "outputmenu/outputmapper.h"
#include <qsettings.h>
#include <qstandardpaths.h>
#include <qthread.h>
#include "settings/settingshandler.h"
#include <tchar.h>
#include <windows.h>

#include <QMutex>
#include <QObject>
#include <QWaitCondition>
#include <cstdio>
#include <future>
#include <iostream>
#include <string>

#include "models/SimConnect.h"
#include "strsafe.h"
#include "enums/LogLevelEnum.h"
#include "utils/OutputConverters.h"
#include "dashboard/models/ComBundle.h"

typedef QList<QString> ComsList;

class MFSWorker : public QThread {
Q_OBJECT

    void run() override { eventLoop(); }

signals:

    void boardConnectionMade(int con);

    void logMessage(std::string message, LogLevel level);


private:
    bool connected = false;
    void setConnected(bool connectedToSim);
    SettingsHandler settingsHandler;
    SIMCONNECT_OBJECT_ID objectID = SIMCONNECT_OBJECT_ID_USER;
    outputHandler outputHandler;
    QList<ComBundle *> *comBundles = new QList<ComBundle *>();
    InputSwitchHandler *dualInputHandler;
    InputMapper dualInputMapper = InputMapper();
    outputMapper *dualOutputMapper = new outputMapper();

    static void MyDispatchProcInput(SIMCONNECT_RECV *pData, DWORD cbData,
                                    void *pContext);

    void loadRunningPortsAndSets();

    std::string formatOutgoingString(float received, std::string prefix, int mode);

    double dataF = 1.2;

    SIMCONNECT_CLIENT_DATA_ID ClientDataID = 1;
    InputEnum radioDefs = InputEnum();
    InputMapper radioMap = InputMapper();
    QList<Output *> outputsToMap;
    QStringList *keys = new QStringList();

    OutputConverters converter = OutputConverters();
    std::map<int, Input>inputs = std::map<int, Input>();
public:
    void setInputs(std::map<int, Input>inputsToSet);

    bool abortDual;

    MFSWorker();

    ~MFSWorker();

    QMutex mutex;
    QWaitCondition condition;

    void eventLoop();

    void sendToArduino(const std::string c_string, int index);
};

#endif  // DUALWORKER_H
