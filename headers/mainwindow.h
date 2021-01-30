#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <headers/ConnectWorker.h>
#include <headers/InputWorker.h>
#include <qpushbutton.h>

#include <QCoreApplication>
#include <QFile>
#include <QMainWindow>
#include <QNetworkReply>
#include <QSettings>

#include "SerialPort.hpp"
const char defaultFileName[] = "indexDl.html";
const std::string version = "0.7.0";
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

 signals:
  void updateLastValUI(QString lastVal);

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

 private:
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
  void settings();
  void untick();
};
#endif  // MAINWINDOW_H
