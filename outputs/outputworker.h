#ifndef OUTPUTWORKER_H
#define OUTPUTWORKER_H

#include <QObject>
#include <QThread>
#include <QWaitCondition>
#include <qmutex.h>
#include <qsettings.h>
#include <qstandardpaths.h>
#include <tchar.h>
#include <windows.h>
#include <headers/SerialPort.hpp>
#include "headers/SimConnect.h"
#include "output.h"

class OutputWorker: public QThread
{
  Q_OBJECT
  void run() override { testDataRequest(); }
 signals:
  void updateLastValUI(QString lastVal);
  void updateLastStatusUI(QString lastStatus);

 public:
  OutputWorker();
  ~OutputWorker();
  QMutex mutex;
  QWaitCondition condition;

  std::string getLastVal() { return lastVal; }
  std::string getLastStatusVal() { return lastStatus; }

  const char* portName;
  const char* valPort;
  bool abort = false;

 private:
  QMap<int,Output*> availableSets;
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

#endif // OUTPUTWORKER_H
