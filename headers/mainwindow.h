#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <Inputs/InputWorker.h>
#include <dual/dualworker.h>
#include <outputs/outputworker.h>
#include <outputs/sethandler.h>
#include <qcombobox.h>
#include <qpushbutton.h>
#include <qstandardpaths.h>
#include <settings/formbuilder.h>

#include <QCoreApplication>
#include <QFile>
#include <QMainWindow>
#include <QNetworkReply>
#include <QSettings>

#include "SerialPort.hpp"
#include "constants.h"

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

  int getComboxIndex(QComboBox *comboBox, QString value);

 public slots:

  void onUpdateLastValUI(const QString &lastVal);

  void onUpdateLastStatusUI(const QString &lastStatus);

  void startMode(int mode);

  void refreshComs(int mode);

  void stopMode(int mode);

  void addCom(int mode);

 signals:
  void updateEventFile(int cmd);

  void closedOutputMenu();

  void closedEventWindow();

  void closedOptionsMenu();
  void closedCalibrateAxisMenu();

  void updateLastValUI(QString lastVal);

  void updateLastStatusUI(QString lastVal);

  void updateActiveCom1(QList<QString> lastVal);

  void startPressed(int mode);

  void refreshPressed(int mode);

  void stopPressed(int mode);

  void addPressed(int mode);

 private slots:
  void localUpdateEventFile();

  void on_updateButton_clicked();

  void outputMenuClosed();
  void calibrateAxisMenuClosed();
  void optionMenuClosed();
  void eventWindowClosed();

  std::string convertComPort(QString comText);

  void openCalibrateAxis();
  void toggleAdvanced();
 public slots:

  void GameConnectionMade(int con, int mode);

  void BoardConnectionMade(int con, int mode);

 private:
  bool advancedMode = false;
  bool closing = false;
  void exitProgram();
  void toggleOpen(QSystemTrayIcon::ActivationReason reason);
  void closeEvent(QCloseEvent *event) override;
  PathHandler pathHandler;
  QString applicationEventsPath = pathHandler.getWritableEventPath();
  enum warnings { NOSET, NOCOMPORT };
  SetHandler *setHandler = new SetHandler();
  SettingsHandler settingsHandler;

  bool outputMenuOpen = false;
  bool eventwindowOpen = false;
  bool calibrateAxisMenuOpen = false;
  bool optionMenuOpen = false;

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
  std::string url = "https://www.bitsanddroids.com/downloads";
  std::string lastValueRec = "";

  void loadSettings();

  const char *portNameLocal;
  QPushButton *updateButton;
  QPushButton *switchButton;
  DualWorker dualThread;
  OutputWorker outputThread;
  InputWorker inputThread;
  QList<set> *availableSets;
  FormBuilder formbuilder;
  Ui::MainWindow *ui;

  void openSettings();

  void openOutputMenu();

  void openEditEventMenu();

  void installWasm();

  void untick();

  void loadComPortData();

  void on_btnSwitchComm1_clicked();

  void startDual();

  bool checkIfComboIsEmpty(QList<QComboBox *>);

  QLabel *returnWarningString(int warningType);

  void clearChildrenFromLayout(QLayout *);

  void copyFolder(QString sourceFolder, QString destinationFolder);
  bool updateApplication();
  void unzip(QString zipfilename, QString filename);
  void checkForUpdates();
};

#endif  // MAINWINDOW_H
