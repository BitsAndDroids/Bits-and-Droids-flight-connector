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

 public:
  InputWorker();
  ~InputWorker() override;
  QMutex mutex;
  QWaitCondition condition;

  bool connected;
  bool abortInput;


 private slots:

  // void switchHandling(int index);

 private:
  SettingsHandler settingsHandler;
  std::string lastVal;
  std::string lastStatus;
  QStringList keys = *settingsHandler.retrieveKeys("inputCom");
  std::string prefix;
  void inputEvents();

  UINT32 HornerScheme(UINT32 Num, UINT32 Divider, UINT32 Factor);
  static void MyDispatchProcInput(SIMCONNECT_RECV* pData, DWORD cbData,
                                  void* pContext);
};

#endif  // INPUTWORKER_H
