#ifndef DUALWORKER_H
#define DUALWORKER_H

#include "InputMapper.h"
#include "handlers/InputSwitchHandler.h"
#include "models/outputs/outputbundle.h"
#include "handlers/outputhandler.h"
#include "widgets/outputmenu/outputmapper.h"
#include <qsettings.h>
#include <qstandardpaths.h>
#include <qthread.h>
#include "handlers/settingshandler.h"
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

typedef QList<QString> ComsList;

class DualWorker : public QThread {
  Q_OBJECT

  void run() override { RadioEvents(); }

 signals:

  void updateActiveCom1(QList<QString> lastActiveCom);

  void GameConnectionMade(int con, int mode);

  void BoardConnectionMade(int con, int mode);

  void updateLastValUI(QString lastVal);

 private:
  // ...
  SettingsHandler settingsHandler;
  SIMCONNECT_OBJECT_ID objectID = SIMCONNECT_OBJECT_ID_USER;
  outputHandler outputHandler;
  QList<outputBundle *> *outputBundles = new QList<outputBundle *>();
  InputSwitchHandler *dualInputHandler = new class InputSwitchHandler();
  InputMapper dualInputMapper = InputMapper();
  outputMapper *dualOutputMapper = new outputMapper();

  static void MyDispatchProcInput(SIMCONNECT_RECV *pData, DWORD cbData,
                                  void *pContext);

  double dataF = 1.2;

  SIMCONNECT_CLIENT_DATA_ID ClientDataID = 1;
  InputEnum radioDefs = InputEnum();
  InputMapper radioMap = InputMapper();
  QList<Output *> outputsToMap;
  QStringList *keys = new QStringList();
  void lastReceived(QString value);

 public:
  void setOutputsToMap(QList<Output *> list) { this->outputsToMap = list; };

  void addBundle(outputBundle *bundle);

  bool abortDual;

  DualWorker();

  ~DualWorker();

  QMutex mutex;
  QWaitCondition condition;

  void RadioEvents();

  void clearBundles();

 public slots:

  void sendWASMCommand(char cmd);
};

#endif  // DUALWORKER_H
