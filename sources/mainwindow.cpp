#include "headers/mainwindow.h"

#include <headers/Set.h>
#include <headers/inputenum.h>
#include <qdesktopservices.h>
#include <qserialportinfo.h>
#include <qstandardpaths.h>
#include <string.h>
#include <windows.h>

#include <QCheckBox>
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

bool matchRow = false;
int matchRowIndex = 0;

bool matchOutput = false;
int matchOutputIndex = 0;

bool matchInput = false;
int matchInputIndex = 0;

const char *portNameLocal;
void MainWindow::untick() {
  QList<QCheckBox *> allCheckBoxes =
      ui->cbTabWidget->findChildren<QCheckBox *>();

  for (int i = 0; i < allCheckBoxes.size(); ++i) {
    allCheckBoxes.at(i)->setChecked(false);
  }
}
void MainWindow::openSettings() {
  QWidget *wdg = new optionsMenu;
  wdg->show();
}
std::string MainWindow::convertComPort(QString comText) {
  std::string val =
      R"(\\.\COM)" + comText.toStdString().std::string::substr(3, 2);
  return val;
}
void MainWindow::loadComPortData() {
  availableComPorts.clear();
  availableComPorts.append("");
  foreach (const QSerialPortInfo &serialPortInfo,
           QSerialPortInfo::availablePorts()) {
    availableComPorts.append(serialPortInfo.portName() + " | " +
                             serialPortInfo.description());
  }
}
void MainWindow::addInputComRow(bool notInit, int index) {
  if (notInit) {
    inputComRowCounter++;
    std::cout << "ADDED" << inputComRowCounter << std::endl;
  }
  if (inputComRowCounter == 10) {
    ui->addComInputBtn->setEnabled(false);
  }
  // Creates new Row and ensures it has recognizable name
  auto newRow = new QHBoxLayout();
  QString rowName = "row" + QString::number(index);
  newRow->setObjectName(rowName);
  newRow->setAlignment(Qt::AlignLeft);

  auto newBox = new QComboBox();
  loadComPortData();
  foreach (const QString &comName, availableComPorts) {
    newBox->addItem(comName);
  }
  newBox->setMaximumWidth(150);
  newBox->setObjectName("comboBoxRow" + QString::number(index));
  auto newDeleteButton = new QPushButton();
  newDeleteButton->setText("-");
  newDeleteButton->setMinimumSize(20, 20);
  newDeleteButton->setMaximumSize(20, 20);
  // setName to recognize which button is pressed when dynamicly added
  QString deleteName = "deleteBtn-" + QString::number(index);
  newDeleteButton->setObjectName(deleteName);
  connect(newDeleteButton, SIGNAL(clicked()), this, SLOT(onClicked()));

  auto newLabel = new QLabel();
  newLabel->setText("Select the com port to be used");
  settings->beginGroup("inputCom");
  QString comString = "inputCom" + QString::number(index);
  if (!settings->value(comString).isNull()) {
    prevRowComInt = settings->value(comString).toString();
    chopStrRow = prevRowComInt.split('M');
    std::cout << "found" << std::endl;
  }
  int counter = 0;
  foreach (const QString &comName, availableComPorts) {
    if (chopStrRow.size() > 0 && comName.contains(chopStrRow.at(1))) {
      matchRow = true;
      matchRowIndex = counter;
      std::cout << "Current match found" << comName.toStdString() << std::endl;
    }
    counter++;
  }
  if (matchRow) {
    newBox->setCurrentIndex(matchRowIndex);
  }

  settings->endGroup();

  newRow->addWidget(newBox);
  newRow->addWidget(newLabel);
  newRow->addWidget(newDeleteButton);

  // Pushes horizontal layout in vertical layout which creates a custom grid
  std::cout << rowName.toStdString() << std::endl;
  ui->inputComListVBox->addLayout(newRow);
  //
  settings->beginGroup("inputCom");
  settings->setValue("inputCounter", inputComRowCounter);
  std::cout << inputComRowCounter << "Rows" << std::endl;
  settings->endGroup();
  settings->sync();
}
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  updateButton = ui->updateButton;
  ui->setupUi(this);

  settings->beginGroup("inputCom");
  inputComRowCounter = settings->value("inputCounter", 1).toInt();
  settings->endGroup();
  for (int i = 1; i < inputComRowCounter; i++) {
    addInputComRow(false, i);
  }
  std::cout << inputComRowCounter << "Rows init" << std::endl;

  // Initiate the sets
  settings->beginGroup("sets");
  if (settings->value("amntOfSets") != NULL) {
  }
  settings->endGroup();
  // On bootup be assured of the latest available com ports
  loadComPortData();

  // Counter is there to keep track of the index at which a possible match is
  // found. These work in cojunction with matchOutput (bool) and
  // matchInput(bool)
  int counter = 0;

  settings->beginGroup("Coms");

  // IMPORTANT leave this check in to assure program doesnt CTD because of
  // search for unexisting variable
  if (!settings->value("outputComActiveBase").isNull()) {
    prevOutputComInt = settings->value("outputComActiveBase").toString();
    chopStrOutput = prevOutputComInt.split('M');
  }
  settings->endGroup();
  settings->beginGroup("inputCom");
  if (!settings->value("inputComActiveBase").isNull()) {
    prevInputComInt = settings->value("inputComActiveBase").toString();
    chopStrInput = prevInputComInt.split('M');
  }
  settings->endGroup();

  // This block itterates through the updated com ports list. This is to ensure
  // that the user doesn't have to re-select his previous com port Does this
  // only on bootup to avoid conflicts down the road If no com port found
  // defaults to blank to make clear to the user the previous com port is no
  // longer available
  foreach (const QString &comName, availableComPorts) {
    if (chopStrOutput.size() > 0 && comName.contains(chopStrOutput.at(1))) {
      matchOutput = true;
      matchOutputIndex = counter;
      std::cout << "Current match found" << comName.toStdString() << std::endl;
    }
    if (chopStrInput.size() > 0 && comName.contains(chopStrInput.at(1))) {
      matchInput = true;
      matchInputIndex = counter;
      std::cout << "Current match found" << comName.toStdString() << std::endl;
    }

    ui->outputComboBoxBase->addItem(comName);
    ui->inputComboBoxBase->addItem(comName);

    counter++;
  }

  if (matchOutput) {
    ui->outputComboBoxBase->setCurrentIndex(matchOutputIndex);
  }
  if (matchInput) {
    ui->inputComboBoxBase->setCurrentIndex(matchInputIndex);
  }

  // If these lines aren't here the group stays open which leads to conflict
  // down the line
  settings->endGroup();
  settings->sync();

  // STYLE AFFECTING SECTION
  //-----------------------

  // Hide both stopbuttons to minimalize confusion if connector is active or not
  ui->stopButton->setVisible(false);
  ui->stopInputButton->setVisible(false);

  // Sets the strecht of the input QVBoxLayout to allow for dynamicly adding of
  ui->inputLayoutHBox->setAlignment(Qt::AlignTop);
  ui->btnsHBox->setAlignment(Qt::AlignLeft);
  ui->inputComListVBox->setAlignment(Qt::AlignTop);
  ui->comInputVBox->setAlignment(Qt::AlignTop);
  ui->setActionsGrid->setAlignment(Qt::AlignTop);
  ui->outputBtnRowHBox->setAlignment(Qt::AlignLeft);
  ui->outputLayoutVBox->setAlignment(Qt::AlignTop);
  ui->gpsVBox->setAlignment(Qt::AlignTop);
  // Load settings (checkboxes + menus)
  loadSettings();

  // MENU WIP

  auto *openSettings = new QAction("&Settings", this);

  QMenu *Settings = menuBar()->addMenu("&Settings");
  Settings->addAction(openSettings);
  connect(openSettings, &QAction::triggered, this, &MainWindow::openSettings);
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
  settings->beginGroup("Checked");
  QList<QCheckBox *> allCheckBoxes =
      ui->cbTabWidget->findChildren<QCheckBox *>();
  QStringList keys = settings->childKeys();

  foreach (const QString &key, settings->childKeys()) {
    if (ui->cbTabWidget->findChild<QCheckBox *>(key)) {
      ui->cbTabWidget->findChild<QCheckBox *>(key)->setChecked(
          settings->value(key).toBool());
    }
  }
  if (!settings->value("simpleInput").isNull()) {
    ui->simpleRBtn->setChecked(settings->value("simpleInput").toBool());
  }
  if (!settings->value("advancedInput").isNull()) {
    ui->advancedRBtn->setChecked(settings->value("advancedInput").toBool());
  }
  if (!settings->value("propInput").isNull()) {
    ui->cbProps->setChecked(settings->value("propInput").toBool());
  }
  if (!settings->value("mixtureInput").isNull()) {
    ui->cbMixtureInput->setChecked(settings->value("mixtureInput").toBool());
  }

  settings->endGroup();
}

void MainWindow::saveSettings() {
  settings->beginGroup("Checked");

  QList<QCheckBox *> allCheckBoxes =
      ui->cbTabWidget->findChildren<QCheckBox *>();
  for (int i = 0; i < allCheckBoxes.size(); i++) {
    QString name = allCheckBoxes.at(i)->objectName();
    settings->setValue(name, allCheckBoxes.at(i)->isChecked());
  }
  settings->setValue("simpleInput", ui->simpleRBtn->isChecked());
  settings->setValue("advancedInput", ui->advancedRBtn->isChecked());
  settings->setValue("propInput", ui->cbProps->isChecked());
  settings->setValue("mixtureInput", ui->cbMixtureInput->isChecked());
  settings->endGroup();
  settings->sync();
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
  QString comText = ui->outputComboBoxBase->currentText();
  std::string comNr =
      R"(\\.\COM)" + comText.toStdString().std::string::substr(3, 2);

  settings->beginGroup("Coms");
  settings->setValue("outputComActiveBase", comNr.c_str());
  settings->endGroup();
  settings->sync();

  // DATA
  outputThread.cbPlaneAltAboveGround = ui->cbPlaneAltAboveGround->isChecked();
  outputThread.cbSimOnGround = ui->cbSimOnGround->isChecked();

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

  // GPS
  outputThread.cbGpsCourseToSteer = ui->cbGpsCourseToSteer->isChecked();

  // coms
  outputThread.cbNavActiveFrequency1 = ui->cbNavActiveFrequency1->isChecked();
  outputThread.cbNavStandbyFrequency1 = ui->cbNavStandbyFrequency1->isChecked();
  outputThread.cbNavActiveFrequency2 = ui->cbNavActiveFrequency2->isChecked();
  outputThread.cbNavStandbyFrequency2 = ui->cbNavStandbyFrequency2->isChecked();
  outputThread.cbNavRadialError1 = ui->cbNavRadialError1->isChecked();
  outputThread.cbNavVorLatlonalt1 = ui->cbNavVorLatlonalt1->isChecked();

  // OBS
  outputThread.cbNavObs1 = ui->cbNavObs1->isChecked();
  outputThread.cbNavObs2 = ui->cbNavObs2->isChecked();

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
  connect(&outputThread, SIGNAL(updateLastStatusUI(QString)),
          SLOT(onUpdateLastStatusUI(QString)));

  outputThread.start();
  ui->startButton->setEnabled(false);
}

void MainWindow::onUpdateLastValUI(const QString &lastVal) {
  ui->labelLastVal_2->setText(lastVal);
}
void MainWindow::onUpdateLastStatusUI(const QString &lastVal) {
  ui->labelLastStatus->setText(lastVal);
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
  settings->beginGroup("inputCom");
  QString comText = ui->inputComboBoxBase->currentText();
  std::cout << comText.toStdString() << std::endl;
  settings->setValue("inputComActiveBase", convertComPort(comText).c_str());
  QString comboBoxName;
  QString key;
  std::cout << inputComRowCounter << "ROWS ON START" << std::endl;
  for (int i = 1; i < inputComRowCounter; i++) {
    key = "inputCom" + QString::number(i);
    comboBoxName = "comboBoxRow" + QString::number(i);
    std::cout << comboBoxName.toStdString() << std::endl;
    QComboBox *box =
        ui->inputComListWidget->findChild<QComboBox *>(comboBoxName);
    QString keyValue = box->currentText();
    std::cout << keyValue.toStdString() << std::endl;
    settings->setValue(key, convertComPort(keyValue).c_str());
  }
  std::cout << "why does it do this" << std::endl;
  settings->endGroup();
  settings->sync();
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
  connect(&inputThread, SIGNAL(updateLastValUI(QString)),
          SLOT(onUpdateLastValUI(QString)));
  connect(&inputThread, SIGNAL(updateLastStatusUI(QString)),
          SLOT(onUpdateLastStatusUI(QString)));

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
  inputThread.quit();
}

void MainWindow::on_pushButton_clicked() {
  ui->outputComboBoxBase->clear();
  loadComPortData();
  foreach (const QString &comName, availableComPorts) {
    ui->outputComboBoxBase->addItem(comName);
  }
}

void MainWindow::on_inputRefreshBtn_clicked() {
  ui->inputComboBoxBase->clear();
  loadComPortData();
  foreach (const QString &comName, availableComPorts) {
    ui->inputComboBoxBase->addItem(comName);
  }
}

void MainWindow::on_addComInputBtn_clicked() {
  addInputComRow(true, inputComRowCounter);
  ui->comInputVBox->update();
}

void MainWindow::onClicked() {
  std::cout << "clicked" << std::endl;
  QPushButton *btn = qobject_cast<QPushButton *>(sender());
  QString btnName = btn->objectName();
  QStringList splitString = btnName.split('-');

  QString nameOfRow = "row" + splitString[1];

  QHBoxLayout *selectedRow =
      ui->inputComListVBox->findChild<QHBoxLayout *>(nameOfRow);
  std::cout << selectedRow->objectName().toStdString() << std::endl;
  QLayoutItem *child;
  while ((child = selectedRow->takeAt(0)) != 0) {
    child->widget()->setParent(NULL);
    delete child;
  }
  delete selectedRow;
  ui->comInputVBox->update();
  inputComRowCounter--;
  settings->beginGroup("inputCom");
  settings->setValue("inputCounter", inputComRowCounter);
  settings->endGroup();
  settings->sync();
  if (inputComRowCounter < 10) {
    ui->addComInputBtn->setEnabled(true);
  }
}

void MainWindow::on_inputOptionsBtn_clicked() {
  QWidget *extraOptions = ui->inputExtraOptionsWidget;
  extraInputOptionsVisible = !extraInputOptionsVisible;
  if (extraInputOptionsVisible) {
    extraOptions->show();
  } else {
    extraOptions->hide();
  }
}

void MainWindow::on_setOptionsBtn_clicked() {
  QWidget *setBlock = ui->setCreationHBox;
  setBlockVisible = !setBlockVisible;
  if (setBlockVisible) {
    setBlock->show();
  } else {
    setBlock->hide();
  }
}

void MainWindow::on_saveSetBtn_clicked() {
  if (!loadedSet) {
    amntSets++;
  }
  QString name = ui->setNameTextInput->text();
  if (name.length() > 0) {
    Set setEdited = Set(amntSets, !loadedSet, name);

    QList<QCheckBox *> checkBoxes =
        ui->cbTabWidget->findChildren<QCheckBox *>();
    foreach (QCheckBox *cb, checkBoxes) {
      if (cb->isChecked()) {
        setEdited.addCheckBox(cb->objectName());
      }
    }
    std::cout << "send" << std::endl;
    setEdited.createSet();
  }
}
