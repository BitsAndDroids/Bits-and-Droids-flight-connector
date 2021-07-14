#include "headers/mainwindow.h"

#include <headers/inputenum.h>
#include <outputs/outputhandler.h>
#include <outputs/outputmapper.h>
#include <outputs/set.h>
#include <qdesktopservices.h>
#include <qserialportinfo.h>
#include <qstandardpaths.h>
#include <settings/formbuilder.h>
#include <settings/outputmenu.h>
#include <string.h>
#include <windows.h>

#include <QCheckBox>
#include <QComboBox>
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

FormBuilder formbuilder;
bool radioOn = false;
bool matchRow = false;
int matchRowIndex = 0;

bool matchOutput = false;
int matchOutputIndex = 0;

bool matchInput = false;
int matchInputIndex = 0;

const char *portNameLocal;
void MainWindow::untick() {
  //  QList<QCheckBox *> allCheckBoxes =
  //      ui->cbTabWidget->findChildren<QCheckBox *>();

  //  for (int i = 0; i < allCheckBoxes.size(); ++i) {
  //    allCheckBoxes.at(i)->setChecked(false);
  //  }
}
void MainWindow::openSettings() {
  QWidget *wdg = new optionsMenu;
  wdg->show();
}
void MainWindow::openOutputMenu() {
  QWidget *wdg = new OutputMenu;
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

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  updateButton = ui->updateButton;
  ui->setupUi(this);

  std::cout << inputComRowCounter << "Rows init" << std::endl;

  availableSets = formbuilder.getAvailableSets();
  // On bootup be assured of the latest available com ports
  loadComPortData();

  // This block itterates through the updated com ports list. This is to ensure
  // that the user doesn't have to re-select his previous com port Does this
  // only on bootup to avoid conflicts down the road If no com port found
  // defaults to blank to make clear to the user the previous com port is no
  // longer available

  //  foreach (const QString &comName, availableComPorts) {
  //    if (chopStrOutput.size() > 0 && comName.contains(chopStrOutput.at(1))) {
  //      matchOutput = true;
  //      matchOutputIndex = counter;
  //      std::cout << "Current match found" << comName.toStdString() <<
  //      std::endl;
  //    }
  //    if (chopStrInput.size() > 0 && comName.contains(chopStrInput.at(1))) {
  //      matchInput = true;
  //      matchInputIndex = counter;
  //      std::cout << "Current match found" << comName.toStdString() <<
  //      std::endl;
  //    }

  qRegisterMetaType<QList<QString>>("QList<QString>");
  // STYLE AFFECTING SECTION
  //-----------------------
  ui->messagesWidgetLayout->setAlignment(Qt::AlignBottom);

  // MENU WIP
  qDebug() << QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
  auto *openSettings = new QAction("&Settings", this);
  auto *openOutputMenu = new QAction("&Outputs", this);
  QMenu *Settings = menuBar()->addMenu("&Settings");
  QMenu *OutputSettings = menuBar()->addMenu("&Outputs");
  OutputSettings->addAction(openOutputMenu);
  Settings->addAction(openSettings);

  auto *untick = new QAction("&Untick all", this);
  Settings->addAction(untick);

  connect(untick, &QAction::triggered, this, &MainWindow::untick);
  connect(openOutputMenu, &QAction::triggered, this,
          &MainWindow::openOutputMenu);
  connect(openSettings, &QAction::triggered, this, &MainWindow::openSettings);
  connect(&formbuilder, &FormBuilder::addPressed, this, &MainWindow::addCom);
  connect(&formbuilder, &FormBuilder::stopPressed, this, &MainWindow::stopMode);
  connect(&formbuilder, &FormBuilder::startPressed, this,
          &MainWindow::startMode);
  connect(&formbuilder, &FormBuilder::refreshPressed, this,
          &MainWindow::refreshComs);
  connect(&inputThread, &InputWorker::updateLastValUI, this,
          &MainWindow::onUpdateLastValUI);
  connect(&inputThread, &InputWorker::updateLastStatusUI, this,
          &MainWindow::onUpdateLastStatusUI);
  connect(&outputThread, SIGNAL(updateLastStatusUI(QString)),
          SLOT(onUpdateLastStatusUI(QString)));

  // IMPORTANT IMPROV SECTION

  formbuilder.loadComPortData();

  // INPUTS
  QVBoxLayout *inContainer = ui->inLayoutContainer;

  inContainer->setAlignment(Qt::AlignTop);
  inContainer->addWidget(formbuilder.generateHeader("INPUTS"));
  inContainer->addWidget(formbuilder.generateComControls(1));
  inContainer->addWidget(formbuilder.generateComSelector(false, 1));
  inContainer->parentWidget()->setSizePolicy(QSizePolicy::Expanding,
                                             QSizePolicy::Expanding);

  // OUTPUTS
  QVBoxLayout *outContainer = ui->outLayoutContainer;

  outContainer->setAlignment(Qt::AlignTop);
  outContainer->addWidget(formbuilder.generateHeader("OUTPUTS"));
  outContainer->addWidget(formbuilder.generateComControls(2));
  outContainer->addWidget(formbuilder.generateComSelector(true, 2));
  outContainer->parentWidget()->setSizePolicy(QSizePolicy::Expanding,
                                              QSizePolicy::Expanding);

  // DUAL MODE
  QVBoxLayout *dualContainer = ui->dualLayoutContainer;

  dualContainer->setAlignment(Qt::AlignTop);
  dualContainer->addWidget(formbuilder.generateHeader("DUAL"));
  dualContainer->addWidget(formbuilder.generateComControls(3));
  dualContainer->addWidget(formbuilder.generateComSelector(true, 3));
  dualContainer->parentWidget()->setSizePolicy(QSizePolicy::Expanding,
                                               QSizePolicy::Expanding);

  QNetworkAccessManager *mgr = new QNetworkAccessManager(this);
  connect(mgr, &QNetworkAccessManager::finished, this, &MainWindow::onfinish);
  connect(mgr, &QNetworkAccessManager::finished, mgr, &QObject::deleteLater);
  mgr->get(
      QNetworkRequest(QUrl("https://www.bitsanddroids.com/assets/"
                           "downloads/connector/version.html")));

  this->adjustSize();
}

void MainWindow::saveSettings() {
  //  settings->beginGroup("Checked");

  //  QList<QCheckBox *> allCheckBoxes =
  //      ui->cbTabWidget->findChildren<QCheckBox *>();
  //  for (int i = 0; i < allCheckBoxes.size(); i++) {
  //    QString name = allCheckBoxes.at(i)->objectName();
  //    settings->setValue(name, allCheckBoxes.at(i)->isChecked());
  //  }
  //  settings->setValue("simpleInput", ui->simpleRBtn->isChecked());
  //  settings->setValue("advancedInput", ui->advancedRBtn->isChecked());
  //  settings->setValue("propInput", ui->cbProps->isChecked());
  //  settings->setValue("mixtureInput", ui->cbMixtureInput->isChecked());
  //  settings->endGroup();
  //  settings->sync();
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

// SLOTS
void MainWindow::onUpdateLastValUI(const QString &lastVal) {
  ui->labelLastVal_2->setText(lastVal);
}
void MainWindow::onUpdateLastStatusUI(const QString &lastVal) {
  ui->labelLastStatus->setText(lastVal);
}
void MainWindow::startMode(int mode) {
  switch (mode) {
    case 1:
      startInputs();
      break;
    case 2:
      startOutputs();
      break;
    case 3:
      startDual();
      break;
  }

  qDebug() << "start" << mode;
}
void MainWindow::startInputs() {
  QWidget *widget = new QWidget();

  widget = ui->inWidgetContainer;

  QRegularExpression search("comBox");
  QList<QComboBox *> comList = widget->findChildren<QComboBox *>(search);

  QString comboBoxName;
  QString key;

  for (int i = 0; i < comList.size(); i++) {
    key = "inputCom" + QString::number(i);
    comboBoxName = "comboBoxRow" + QString::number(i);

    QString keyValue = comList[i]->currentText();
    qDebug() << keyValue;
    std::cout << keyValue.toStdString() << std::endl;
    settingsHandler.storeValue("inputCom", key,
                               convertComPort(keyValue).c_str());
  }

  inputThread.abortInput = false;
  inputThread.start();
}

void MainWindow::startOutputs() {
  QWidget *widget = new QWidget();

  widget = ui->outWidgetContainer;
  settingsHandler.clearKeys("outputcoms");
  settingsHandler.clearKeys("outputComSet");

  QRegularExpression search("comBox");
  QList<QComboBox *> comList = widget->findChildren<QComboBox *>(search);

  QRegularExpression searchSets("setBox");
  QList<QComboBox *> setList = widget->findChildren<QComboBox *>(searchSets);

  QString key;
  QString setKey;
  QList<Output *> outputsToMap;
  qDebug() << "sets available" << comList.size();
  for (int i = 0; i < comList.size(); i++) {
    key = "outputCom" + QString::number(i);
    setKey = "outputSet" + QString::number(i);

    QString keyValue = comList[i]->currentText();
    QString setKeyValue = setList[i]->currentText();
    qDebug() << setKeyValue;
    std::cout << keyValue.toStdString() << std::endl;
    settingsHandler.storeValue("outputcoms", key,
                               convertComPort(keyValue).c_str());

    qDebug() << " SET INDEX " << setList.at(i)->currentIndex();
    int index = setList[i]->currentIndex();

    int id = availableSets->at(index).getID();

    outputBundle *bundle = new outputBundle();

    set active = setHandler->getSetById(QString::number(id));
    bundle->setSet(active);
    //    const char *spString = convertComPort(keyValue).c_str();
    //    SerialPort *port = new SerialPort(spString);
    //    bundle->setSerialPortString(&spString);
    //    bundle->setSerialPort(*port);

    QMap<int, Output *> *outputs = new QMap<int, Output *>();
    *outputs = active.getOutputs();
    QMap<int, Output *>::iterator j;
    for (j = outputs->begin(); j != outputs->end(); j++) {
      outputsToMap.append(j.value());
    }
    outputThread.setOutputsToMap(outputsToMap);
    bundle->setOutputsInSet(*outputs);
    outputThread.addBundle(bundle);
    settingsHandler.storeValue("outputComSet", setKey, id);
  }

  //    ui->stopButton->setVisible(true);
  //    QString comText = ui->outputComboBoxBase->currentText();
  // std::string comNr =
  //    R"(\\.\COM)" + comText.toStdString().std::string::substr(3, 2);

  outputThread.abort = false;
  outputThread.start();
}
void MainWindow::startDual() {
  QWidget *widget = new QWidget();

  widget = ui->dualWidgetContainer;

  QRegularExpression search("comBox");
  QList<QComboBox *> comList = widget->findChildren<QComboBox *>(search);

  QString comboBoxName;
  QString key;

  for (int i = 0; i < comList.size(); i++) {
    key = "dualCom" + QString::number(i);

    QString keyValue = comList[i]->currentText();
    qDebug() << keyValue;
    std::cout << keyValue.toStdString() << std::endl;
    settingsHandler.storeValue("dualCom", key,
                               convertComPort(keyValue).c_str());
  }
  radioThread.start();
  radioThread.abortRadio = false;

  std::cout << "clicked" << std::endl;
}
void MainWindow::refreshComs(int mode) {
  QWidget *widget;

  switch (mode) {
    case 1:
      widget = ui->inWidgetContainer;
      break;
    case 2:
      widget = ui->outWidgetContainer;
      break;
    case 3:
      widget = ui->dualWidgetContainer;
      break;
  }
  QRegularExpression search("comBox");
  QList<QComboBox *> comList = widget->findChildren<QComboBox *>(search);
  formbuilder.loadComPortData();
  QList<set> sets = *formbuilder.getAvailableSets();
  QList<QString> coms = formbuilder.getAvailableComPorts();

  for (int i = 0; i < comList.size(); i++) {
    qDebug() << "hit size:" << comList.size();

    comList[i]->clear();
    for (int j = 0; j < coms.size(); j++) {
      comList[i]->addItem(coms[j]);
      qDebug() << coms[j];
    }
  }
  qDebug() << "refresh" << mode;
}
void MainWindow::stopMode(int mode) {
  switch (mode) {
    case 1:
      stopInput();
      break;
    case 2:
      stopOutput();
      break;
    case 3:
      stopDual();
      break;
  }
}
void MainWindow::addCom(int mode) {
  QVBoxLayout *layout;
  bool set = false;
  switch (mode) {
    case 1:
      layout = ui->inLayoutContainer;
      break;
    case 2:
      layout = ui->outLayoutContainer;
      set = true;
      break;
    case 3:
      layout = ui->dualLayoutContainer;
      set = true;
      break;
  }

  layout->addWidget(formbuilder.generateComSelector(set, mode));
  qDebug() << "addCom" << mode;
}
void MainWindow::stopInput() {
  inputThread.abortInput = true;
  inputThread.quit();
}
void MainWindow::stopOutput() { outputThread.abort = true; }
void MainWindow::stopDual() {}
void MainWindow::on_updateButton_clicked() {
  QString qUrl = QString::fromStdString(url);
  QDesktopServices::openUrl(QUrl(qUrl));
}
void MainWindow::on_btnSwitchComm1_clicked() {
  if (radioThread.isRunning()) {
    radioThread.switchCom1();
  }
}
void MainWindow::on_btnSwitchNav1_clicked() {
  if (radioThread.isRunning()) {
    radioThread.switchNav1();
  }
}
MainWindow::~MainWindow() {
  saveSettings();
  delete ui;
}
