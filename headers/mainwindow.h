#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <headers/ConnectWorker.h>
#include <headers/InputWorker.h>
#include <outputs/OutputWorker.h>
#include <outputs/sethandler.h>
#include <qpushbutton.h>
#include <qstandardpaths.h>

#include <QCoreApplication>
#include <QFile>
#include <QMainWindow>
#include <QNetworkReply>
#include <QSettings>

#include "SerialPort.hpp"
#include "constants.h"
#include "radioworker.h"
const char defaultFileName[] = "indexDl.html";
const std::string version = constants::VERSION;
QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
  Q_OBJECT

 public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

  void startInputs();

  void startOutputs();
  void on_btnSwitchNav1_clicked();
 public slots:
  void onUpdateLastValUI(const QString &lastVal);
  void onUpdateLastStatusUI(const QString &lastStatus);
  void startMode(int mode);
  void refreshComs(int mode);
  void stopMode(int mode);
  void addCom(int mode);

 signals:
  void updateLastValUI(QString lastVal);
  void updateLastStatusUI(QString lastVal);
  void updateActiveCom1(QList<QString> lastVal);
  void startPressed(int mode);
  void refreshPressed(int mode);
  void stopPressed(int mode);
  void addPressed(int mode);

 private slots:

  void onfinish(QNetworkReply *rep);

  std::string convertComPort(QString comText);

 private:
  SetHandler *setHandler = new SetHandler();
  SettingsHandler settingsHandler;
  void stopInput();
  void stopOutput();
  void stopDual();

  QString prevInputComInt;
  QList<QString> chopStrInput;

  QString prevRowComInt;
  QList<QString> chopStrRow;
  int amntSets = 0;
  bool loadedSet = false;

  QString prevOutputComInt;
  QList<QString> chopStrOutput;
  bool setBlockVisible = true;
  bool extraInputOptionsVisible = true;
  int inputComRowCounter = 1;
  QList<QString> availableComPorts;
  QString m_sSettingsFile;
  std::string url;
  std::string lastValueRec = "";
  void saveSettings();
  void loadSettings();
  const char *portNameLocal;
  QPushButton *updateButton;
  QPushButton *switchButton;
  RadioWorker radioThread;
  OutputWorker outputThread;
  InputWorker inputThread;
  QList<set> *availableSets;

  Ui::MainWindow *ui;
  void openSettings();
  void openOutputMenu();
  void untick();
  void loadComPortData();
  void on_btnSwitchComm1_clicked();
  void on_updateButton_clicked();
  void startDual();
};
#endif  // MAINWINDOW_H
