#include "headers/mainwindow.h"

#include <qdesktopservices.h>
#include <qserialportinfo.h>
#include <qstandardpaths.h>
#include <string.h>
#include <windows.h>

#include <QDesktopServices>
#include <QDir>
#include <QFileInfo>
#include <QMessageBox>
#include <QNetworkAccessManager>
#include <QSettings>
#include <QStandardPaths>
#include <cstdio>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

#include "headers/optionsmenu.h"
#include "stdio.h"
#include "ui_mainwindow.h"

const char *portNameLocal;
void MainWindow::untick() {
  QList<QCheckBox *> allCheckBoxes = ui->tabWidget->findChildren<QCheckBox *>();

  for (int i = 0; i < allCheckBoxes.size(); ++i) {
    allCheckBoxes.at(i)->setChecked(false);
  }
}
void MainWindow::settings() {
  QWidget *wdg = new optionsMenu;
  wdg->show();
}
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  updateButton = ui->updateButton;
  ui->setupUi(this);
  ui->stopButton->setVisible(false);
  ui->stopInputButton->setVisible(false);
  foreach (const QSerialPortInfo &serialPortInfo,
           QSerialPortInfo::availablePorts()) {
    ui->comboBox->addItem(serialPortInfo.portName() + " | " +
                          serialPortInfo.description());
    ui->comBoxInput->addItem(serialPortInfo.portName() + " | " +
                             serialPortInfo.description());
  }

  loadSettings();
  // MENU WIP

  auto *settings = new QAction("&Settings", this);

  QMenu *Settings = menuBar()->addMenu("&Settings");
  Settings->addAction(settings);
  connect(settings, &QAction::triggered, this, &MainWindow::settings);
  auto *untick = new QAction("&Untick all", this);
  Settings->addAction(untick);
  connect(untick, &QAction::triggered, this, &MainWindow::untick);

  QNetworkAccessManager *mgr = new QNetworkAccessManager(this);
  connect(mgr, SIGNAL(finished(QNetworkReply *)), this,
          SLOT(onfinish(QNetworkReply *)));
  connect(mgr, SIGNAL(finished(QNetworkReply *)), mgr, SLOT(deleteLater()));
  mgr->get(
      QNetworkRequest(QUrl("https://www.bitsanddroids.com/assets/"
                           "downloads/connector/version.html")));
}

void MainWindow::loadSettings() {
  QString path =
      QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
  QSettings settings(path + "/" + "settings.ini", QSettings::IniFormat);
  settings.beginGroup("Checked");
  QList<QCheckBox *> allCheckBoxes = ui->tabWidget->findChildren<QCheckBox *>();
  QStringList keys = settings.childKeys();

  foreach (const QString &key, settings.childKeys()) {
    if (ui->tabWidget->findChild<QCheckBox *>(key)) {
      ui->tabWidget->findChild<QCheckBox *>(key)->setChecked(
          settings.value(key).toBool());
    }
  }

  settings.endGroup();
}

void MainWindow::saveSettings() {
  QString path =
      QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
  QSettings settings(path + "/" + "settings.ini", QSettings::IniFormat);
  settings.beginGroup("Checked");

  QList<QCheckBox *> allCheckBoxes = ui->tabWidget->findChildren<QCheckBox *>();
  for (int i = 0; i < allCheckBoxes.size(); i++) {
    QString name = allCheckBoxes.at(i)->objectName();
    settings.setValue(name, allCheckBoxes.at(i)->isChecked());
  }

  settings.endGroup();
  settings.sync();
}

void MainWindow::onfinish(QNetworkReply *rep) {
  try {
    if (rep->error() == QNetworkReply::NoError) {
      QByteArray bts = rep->readAll();

      QString str(bts);

      QStringList versionCut = str.split("~");

      if (versionCut[1].toStdString() != version) {
        ui->updateButton->setEnabled(true);
        std::string nr = versionCut[1].toStdString();
        std::string buttonText = "New update available: " + nr;
        ui->updateButton->setText(QString::fromStdString(buttonText));
        url = versionCut[3].toStdString();
      } else {
        ui->updateButton->setText("Up to date v: " + versionCut[1]);
      }
    }
  } catch (...) {
  }
}

MainWindow::~MainWindow() {
  saveSettings();
  delete ui;
}

void MainWindow::on_startButton_clicked() {
  ui->stopButton->setVisible(true);
  QString comText = ui->comboBox->currentText();
  std::string comNr =
      R"(\\.\COM)" + comText.toStdString().std::string::substr(3, 2);
  QString path =
      QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
  QSettings settings(path + "/" + "settings.ini", QSettings::IniFormat);
  settings.beginGroup("Coms");
  settings.setValue("comActive1", comNr.c_str());
  settings.endGroup();
  settings.sync();

  // Avionics
  outputThread.cbPlaneName = ui->cbPlaneName->isChecked();
  outputThread.cbAPAltitudeLock = ui->cbAPAltitudeLock->isChecked();
  outputThread.cbAPHeadingLock = ui->cbAPHeadingLock->isChecked();
  outputThread.cbAPVerticalLock = ui->cbAPVerticalLock->isChecked();
  outputThread.cbActiveCom1 = ui->cbActiveCom1->isChecked();
  outputThread.cbActiveCom2 = ui->cbActiveCom2->isChecked();
  outputThread.cbStandbyCom1 = ui->cbStandbyCom1->isChecked();
  outputThread.cbStandbyCom2 = ui->cbStandbyCom2->isChecked();
  outputThread.cbIndicatedAirspeed = ui->cbIndicatedAirspeed->isChecked();
  outputThread.cbIndicatedVerticalSpeed =
      ui->cbIndicatedVerticalSpeed->isChecked();
  outputThread.cbIndicatedAltitude = ui->cbIndicatedAltitude->isChecked();
  outputThread.cbIndicatedHeading = ui->cbIndicatedHeading->isChecked();
  outputThread.cbGPSGroundspeed = ui->cbGPSGroundspeed->isChecked();
  outputThread.cbKohlman = ui->cbKohlman->isChecked();
  outputThread.cbBarometerPressure = ui->cbBarometerPressure->isChecked();
  outputThread.cbSelectedQuantityPercent =
      ui->cbSelectedQuantityPercent->isChecked();

  // coms
  outputThread.cbNavActiveFrequency1 = ui->cbNavActiveFrequency1->isChecked();
  outputThread.cbNavStandbyFrequency1 = ui->cbNavStandbyFrequency1->isChecked();
  outputThread.cbNavActiveFrequency2 = ui->cbNavActiveFrequency2->isChecked();
  outputThread.cbNavStandbyFrequency2 = ui->cbNavStandbyFrequency2->isChecked();
  outputThread.cbNavRadialError1 = ui->cbNavRadialError1->isChecked();
  outputThread.cbNavVorLatlonalt1 = ui->cbNavVorLatlonalt1->isChecked();

  // DME
  outputThread.cbNavDme1 = ui->cbNavDme1->isChecked();
  outputThread.cbNavDmespeed1 = ui->cbNavDmespeed1->isChecked();
  outputThread.cbNavDme2 = ui->cbNavDme2->isChecked();
  outputThread.cbNavDmespeed2 = ui->cbNavDmespeed2->isChecked();

  // ADF
  outputThread.cbAdfActiveFrequency1 = ui->cbAdfActiveFrequency1->isChecked();
  outputThread.cbAdfStandbyFrequency1 = ui->cbAdfStandbyFrequency1->isChecked();
  outputThread.cbAdfRadial1 = ui->cbAdfRadial1->isChecked();
  outputThread.cbAdfSignal1 = ui->cbAdfSignal1->isChecked();

  outputThread.cbAdfActiveFrequency2 = ui->cbAdfActiveFrequency2->isChecked();
  outputThread.cbAdfStandbyFrequency2 = ui->cbAdfStandbyFrequency2->isChecked();
  outputThread.cbAdfRadial2 = ui->cbAdfRadial2->isChecked();
  outputThread.cbAdfSignal2 = ui->cbAdfSignal2->isChecked();

  // Transponder
  outputThread.cbTransponderCode1 = ui->cbTransponderCode1->isChecked();
  outputThread.cbTransponderCode2 = ui->cbTransponderCode2->isChecked();

  // lights
  outputThread.cbLightTaxiOn = ui->cbLightTaxiOn->isChecked();
  outputThread.cbLightStrobeOn = ui->cbLightStrobeOn->isChecked();
  outputThread.cbLightPanelOn = ui->cbLightPanelOn->isChecked();
  outputThread.cbLightRecognitionOn = ui->cbLightRecognitionOn->isChecked();
  outputThread.cbLightWingOn = ui->cbLightWingOn->isChecked();
  outputThread.cbLightLogoOn = ui->cbLightLogoOn->isChecked();
  outputThread.cbLightCabinOn = ui->cbLightCabinOn->isChecked();
  outputThread.cbLightHeadOn = ui->cbLightHeadOn->isChecked();
  outputThread.cbLightBrakeOn = ui->cbLightBrakeOn->isChecked();
  outputThread.cbLightNavOn = ui->cbLightNavOn->isChecked();
  outputThread.cbLightBeaconOn = ui->cbLightBeaconOn->isChecked();
  outputThread.cbLightLandingOn = ui->cbLightLandingOn->isChecked();

  // warnings
  outputThread.cbStallWarning = ui->cbStallWarning->isChecked();
  outputThread.cbOverspeedWarning = ui->cbOverspeedWarning->isChecked();

  // trim rudder
  outputThread.cbElevatorTrimPosition = ui->cbElevatorTrimPosition->isChecked();
  outputThread.cbElevatorTrimPct = ui->cbElevatorTrimPct->isChecked();
  outputThread.cbAileronTrim = ui->cbAileronTrim->isChecked();
  outputThread.cbAileronTrimPct = ui->cbAileronTrimPct->isChecked();
  outputThread.cbRudderTrim = ui->cbRudderTrim->isChecked();
  outputThread.cbRudderTrimPct = ui->cbRudderTrimPct->isChecked();

  // flaps
  outputThread.cbFlapsHandlePercent = ui->cbFlapsHandlePercent->isChecked();
  outputThread.cbFlapsHandleIndex = ui->cbFlapsHandleIndex->isChecked();
  outputThread.cbFlapsNumHandlePositions =
      ui->cbFlapsNumHandlePositions->isChecked();
  outputThread.cbTrailingEdgeFlapsLeftPercent =
      ui->cbTrailingEdgeFlapsLeftPercent->isChecked();
  outputThread.cbTrailingEdgeFlapsRightPercent =
      ui->cbTrailingEdgeFlapsRightPercent->isChecked();
  outputThread.cbTrailingEdgeFlapsLeftAngle =
      ui->cbTrailingEdgeFlapsLeftAngle->isChecked();
  outputThread.cbTrailingEdgeFlapsRightAngle =
      ui->cbTrailingEdgeFlapsRightAngle->isChecked();
  outputThread.cbLeadingEdgeFlapsLeftPercent =
      ui->cbLeadingEdgeFlapsLeftPercent->isChecked();
  outputThread.cbLeadingEdgeFlapsRightPercent =
      ui->cbLeadingEdgeFlapsRightPercent->isChecked();
  outputThread.cbLeadingEdgeFlapsLeftAngle =
      ui->cbLeadingEdgeFlapsLeftAngle->isChecked();
  outputThread.cbLeadingEdgeFlapsRightAngle =
      ui->cbLeadingEdgeFlapsRightAngle->isChecked();

  // gear
  outputThread.cbGearHandlePosition = ui->cbGearHandlePosition->isChecked();
  outputThread.cbGearHydraulicPressure =
      ui->cbGearHydraulicPressure->isChecked();
  outputThread.cbGearCenterPosition = ui->cbGearCenterPosition->isChecked();
  outputThread.cbGearLeftPosition = ui->cbGearLeftPosition->isChecked();
  outputThread.cbGearRightPosition = ui->cbGearRightPosition->isChecked();
  outputThread.cbGearTailPosition = ui->cbGearTailPosition->isChecked();
  outputThread.cbGearAuxPosition = ui->cbGearAuxPosition->isChecked();
  outputThread.cbGearTotalPctExtended = ui->cbGearTotalPctExtended->isChecked();

  // AP
  outputThread.cbAutopilotAvailable = ui->cbAutopilotAvailable->isChecked();
  outputThread.cbAutopilotMaster = ui->cbAutopilotMaster->isChecked();
  outputThread.cbAutopilotWingLeveler = ui->cbAutopilotWingLeveler->isChecked();
  outputThread.cbAutopilotNav1Lock = ui->cbAutopilotNav1Lock->isChecked();
  outputThread.cbAutopilotHeadingLock = ui->cbAutopilotHeadingLock->isChecked();
  outputThread.cbAutopilotAltitudeLock =
      ui->cbAutopilotAltitudeLock->isChecked();
  outputThread.cbAutopilotAttitudeHold =
      ui->cbAutopilotAttitudeHold->isChecked();
  outputThread.cbAutopilotGlideslopeHold =
      ui->cbAutopilotGlideslopeHold->isChecked();
  outputThread.cbAutopilotApproachHold =
      ui->cbAutopilotApproachHold->isChecked();
  outputThread.cbAutopilotBackcourseHold =
      ui->cbAutopilotBackcourseHold->isChecked();
  outputThread.cbAutopilotFlightDirectorActive =
      ui->cbAutopilotFlightDirectorActive->isChecked();
  outputThread.cbAutopilotAirspeedHold =
      ui->cbAutopilotAirspeedHold->isChecked();
  outputThread.cbAutopilotMachHold = ui->cbAutopilotMachHold->isChecked();
  outputThread.cbAutopilotYawDamper = ui->cbAutopilotYawDamper->isChecked();
  outputThread.cbAutopilotThrottleArm = ui->cbAutopilotThrottleArm->isChecked();
  outputThread.cbAutopilotTakeoffPowerActive =
      ui->cbAutopilotTakeoffPowerActive->isChecked();
  outputThread.cbAutothrottleActive = ui->cbAutothrottleActive->isChecked();
  outputThread.cbAutopilotNav1Lock = ui->cbAutopilotNav1Lock->isChecked();
  outputThread.cbAutopilotVerticalHold =
      ui->cbAutopilotVerticalHold->isChecked();
  outputThread.cbAutopilotRpmHold = ui->cbAutopilotRpmHold->isChecked();

  ui->startButton->setText("Running");

  outputThread.abort = false;
  connect(&outputThread, SIGNAL(updateLastValUI(QString)),
          SLOT(onUpdateLastValUI(QString)));

  outputThread.start();
  ui->startButton->setEnabled(false);
}
void MainWindow::onUpdateLastValUI(const QString &lastVal) {
  ui->labelLastVal->setText(lastVal);
}
void MainWindow::on_stopButton_clicked() {
  ui->startButton->setEnabled(true);
  ui->stopButton->setVisible(false);
  ui->startButton->setText("Start");
  //    connect(&outputThread, SIGNAL(finished()),
  //             &outputThread, SLOT(deleteLater()));
  outputThread.abort = true;
}

void MainWindow::on_updateButton_clicked() {
  QString qUrl = QString::fromStdString(url);
  QDesktopServices::openUrl(QUrl(qUrl));
}

void MainWindow::on_switchButton_clicked() {
  int curIndex = ui->stackedWidget->currentIndex();
  if (curIndex == 1) {
    ui->stackedWidget->setCurrentIndex(0);
  } else {
    ui->stackedWidget->setCurrentIndex(1);
  }
}

void MainWindow::on_simpleRBtn_toggled(bool checked) {
  ui->advancedRBtn->setChecked(false);
}

void MainWindow::on_advancedRBtn_clicked() {
  ui->simpleRBtn->setChecked(false);
}

void MainWindow::on_startInputButton_clicked() {
  QString comText = ui->comBoxInput->currentText();
  std::string comNr =
      R"(\\.\COM)" + comText.toStdString().std::string::substr(3, 2);
  QString path =
      QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
  QSettings settings(path + "/" + "settings.ini", QSettings::IniFormat);
  settings.beginGroup("Coms");
  settings.setValue("comActiveInput1", comNr.c_str());
  settings.endGroup();
  settings.sync();
  if (ui->advancedRBtn->isChecked()) {
    inputThread.advanced = true;
  } else {
    inputThread.advanced = false;
  }
  if (ui->cbProps->isChecked()) {
    inputThread.props = true;
  } else {
    inputThread.props = false;
  }
  inputThread.abortInput = false;
  inputThread.start();
  ui->startInputButton->setEnabled(false);
  ui->stopInputButton->setVisible(true);
  ui->startInputButton->setText("Running");
}

void MainWindow::on_stopInputButton_clicked() {
  ui->startInputButton->setEnabled(true);
  ui->stopInputButton->setVisible(false);
  ui->startInputButton->setText("Start");
  inputThread.abortInput = true;
}

void MainWindow::on_pushButton_clicked() {
  ui->comboBox->clear();

  foreach (const QSerialPortInfo &serialPortInfo,
           QSerialPortInfo::availablePorts()) {
    ui->comboBox->addItem(serialPortInfo.portName() + " | " +
                          serialPortInfo.description());
  }
}

void MainWindow::on_inputRefreshBtn_clicked() {
  ui->comBoxInput->clear();
  foreach (const QSerialPortInfo &serialPortInfo,
           QSerialPortInfo::availablePorts()) {
    ui->comBoxInput->addItem(serialPortInfo.portName() + " | " +
                             serialPortInfo.description());
  }
}
