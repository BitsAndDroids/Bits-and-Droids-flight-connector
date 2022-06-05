#ifndef OUTPUTWORKER_H
#define OUTPUTWORKER_H

#include <qmutex.h>
#include <qsettings.h>
#include <qstandardpaths.h>
#include "handlers/settingshandler.h"
#include <tchar.h>
#include <windows.h>

#include <QObject>
#include <QThread>
#include <QWaitCondition>
#include "serial/SerialPort.hpp"

#include "models/SimConnect.h"
#include "models/outputs/output.h"
#include "models/outputs/outputbundle.h"
#include "handlers/outputhandler.h"
#include "widgets/outputmenu/outputmapper.h"

class OutputWorker : public QThread {
  Q_OBJECT
  void run() override { testDataRequest(); }
 signals:
  void updateLastValUI(QString lastVal);
  void updateLastStatusUI(QString lastStatus);
  void GameConnectionMade(int con, int mode);
  void BoardConnectionMade(int con, int mode);

 public:
  OutputWorker();
  ~OutputWorker();
  QMutex mutex;
  QWaitCondition condition;

  std::string getLastVal() { return lastVal; }
  std::string getLastStatusVal() { return lastStatus; }
  void clearBundles();

  const char* portName;
  const char* valPort;
  bool abort = false;
  void setOutputsToMap(QList<Output*> list) { this->outputsToMap = list; };

  void addBundle(outputBundle* bundle);

 private:
  outputMapper* outputMapper = new class outputMapper();
  bool connected = false;
  QList<Output*> outputsToMap;
  QList<outputBundle*>* outputBundles = new QList<outputBundle*>();
  SettingsHandler settingsHandler;
  outputHandler outputHandler;
  QStringList* keys;
  QMap<int, Output*> availableSets;

  int updatePerXFrames = 15;
  std::string lastVal;
  std::string lastStatus;
  SerialPort* arduino;

  static void MyDispatchProcRD(SIMCONNECT_RECV* pData, DWORD cbData,
                               void* pContext);
  void testDataRequest();
};

#endif  // OUTPUTWORKER_H
