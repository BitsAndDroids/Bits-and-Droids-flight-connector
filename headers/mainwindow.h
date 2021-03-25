#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <headers/ConnectWorker.h>
#include <headers/InputWorker.h>
#include <qpushbutton.h>
#include <qstandardpaths.h>

#include <QCoreApplication>
#include <QFile>
#include <QMainWindow>
#include <QNetworkReply>
#include <QSettings>

#include "SerialPort.hpp"
const char defaultFileName[] = "indexDl.html";
const std::string version = "0.8.4";
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

 public slots:
  void onUpdateLastValUI(const QString &lastVal);
  void onUpdateLastStatusUI(const QString &lastStatus);

 signals:
  void updateLastValUI(QString lastVal);
  void updateLastStatusUI(QString lastVal);

 private slots:

  void on_startButton_clicked();
  void on_stopButton_clicked();
  void onfinish(QNetworkReply *rep);
  void on_updateButton_clicked();
  void on_switchButton_clicked();
  void on_simpleRBtn_toggled(bool checked);
  void on_advancedRBtn_clicked();
  void on_startInputButton_clicked();
  void on_stopInputButton_clicked();
  void on_inputRefreshBtn_clicked();
  void on_pushButton_clicked();
  void on_inputOptionsBtn_clicked();
  void on_addComInputBtn_clicked();

  void onClicked();

  void on_setOptionsBtn_clicked();

  void on_saveSetBtn_clicked();

  void addInputComRow(bool notInit, int index);

  std::string convertComPort(QString comText);

 private:
  QString path =
      QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
  QSettings *settings =
      new QSettings(path + "/" + "settings.ini", QSettings::IniFormat);

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
  ConnectWorker outputThread;
  InputWorker inputThread;

  Ui::MainWindow *ui;
  void openSettings();
  void untick();
  void loadComPortData();
};
#endif  // MAINWINDOW_H
