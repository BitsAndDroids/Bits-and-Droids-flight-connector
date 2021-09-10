#ifndef DUALWORKER_H
#define DUALWORKER_H

#include <Inputs/InputMapper.h>
#include <Inputs/InputSwitchHandler.h>
#include <outputs/outputbundle.h>
#include <outputs/outputhandler.h>
#include <outputs/outputmapper.h>
#include <qsettings.h>
#include <qstandardpaths.h>
#include <qthread.h>
#include <settings/settingshandler.h>
#include <tchar.h>
#include <windows.h>

#include <QMutex>
#include <QObject>
#include <QWaitCondition>
#include <cstdio>
#include <future>
#include <iostream>
#include <string>

#include "headers/SimConnect.h"

#include "strsafe.h"
typedef QList<QString> ComsList;
class DualWorker : public QThread {
  Q_OBJECT
  void run() override { RadioEvents(); }
 signals:
  void updateActiveCom1(QList<QString> lastActiveCom);

 private:
  // ...
  SettingsHandler settingsHandler;
  outputHandler outputHandler;
  QList<outputBundle *> *outputBundles = new QList<outputBundle *>();
  InputSwitchHandler *dualInputHandler = new class InputSwitchHandler();
  InputMapper dualInputMapper = InputMapper();
  outputMapper dualOutputMapper = outputMapper();

  static void MyDispatchProcInput(SIMCONNECT_RECV *pData, DWORD cbData,
                                  void *pContext);

  QList<Output *> outputsToMap;
  QStringList *keys = new QStringList();

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
};

#endif  // DUALWORKER_H
