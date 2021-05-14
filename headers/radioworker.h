#ifndef RADIOWORKER_H
#define RADIOWORKER_H

#include <qsettings.h>
#include <qstandardpaths.h>
#include <qthread.h>
#include <tchar.h>
#include <windows.h>

#include <QMutex>
#include <QWaitCondition>
#include <cstdio>
#include <future>
#include <iostream>
#include <string>

#include "SimConnect.h"
#include "stdio.h"
#include "strsafe.h"
typedef QList<QString> ComsList;
class RadioWorker : public QThread {
  Q_OBJECT
  void run() override { RadioEvents(); }
 signals:
  void updateActiveCom1(QList<QString> lastActiveCom);

 private:
  // ...

  QString path =
      QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
  QSettings *settings =
      new QSettings(path + "/" + "settings.ini", QSettings::IniFormat);

  static void MyDispatchProcInput(SIMCONNECT_RECV *pData, DWORD cbData,
                                  void *pContext);

  QList<QString> coms = {"000.000", "000.000", "000.000",
                         "000.000", "000.000", "000.000"};

 public:
  bool abortRadio;
  RadioWorker();
  ~RadioWorker();
  QMutex mutex;
  QWaitCondition condition;
  void RadioEvents();
  void switchNav1();
 public slots:
  void switchCom1();
};

#endif  // RADIOWORKER_H
