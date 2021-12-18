#ifndef INPUTWORKER_H
#define INPUTWORKER_H

#include <headers/SimConnect.h>
#include <qmutex.h>
#include <qsettings.h>
#include <qstandardpaths.h>
#include <qthread.h>
#include <qwaitcondition.h>
#include <settings/settingshandler.h>
#include <tchar.h>
#include <windows.h>

#include <QObject>
#include <string>

#include <cstdio>
#include "InputMapper.h"
#include "InputSwitchHandler.h"

/*!
  \class InputWorker
  \brief The InputWorker class
 */
class InputWorker : public QThread {
Q_OBJECT

    void run() override { inputEvents(); }

signals:

    void updateLastValUI(QString lastVal);

    void updateLastStatusUI(QString lastStatus);

    void GameConnectionMade(int con, int mode);

    void BoardConnectionMade(int con, int mode);

public:
    InputWorker();

    ~InputWorker() override;

    QMutex mutex;
    QWaitCondition condition;

    bool connected;
    bool abortInput;
public slots:

    void updateEventFile();

private slots:

    // void switchHandling(int index);

private:
    QStringList curveStrings = {"Rudder", "Toe brakes", "Aileron","Elevator"};
    SettingsHandler settingsHandler;
    std::string lastVal;
    SIMCONNECT_OBJECT_ID objectID = SIMCONNECT_OBJECT_ID_USER;
    SIMCONNECT_CLIENT_DATA_ID ClientDataID = 1;
    std::string lastStatus;
    InputMapper mapper = InputMapper();
    InputSwitchHandler handler = InputSwitchHandler();
    QStringList keys = *settingsHandler.retrieveKeys("inputCom");
    std::string prefix;

    void inputEvents();

    UINT32 HornerScheme(UINT32 Num, UINT32 Divider, UINT32 Factor);

    static void MyDispatchProcInput(SIMCONNECT_RECV *pData, DWORD cbData,
                                    void *pContext);
};

#endif  // INPUTWORKER_H
