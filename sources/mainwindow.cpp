#include "headers/mainwindow.h"

#include <events/eventwindow.h>
#include <qdesktopservices.h>
#include <qserialportinfo.h>
#include <qstandardpaths.h>
#include <settings/calibrateaxismenu.h>
#include <settings/optionsmenu.h>
#include <settings/outputmenu.h>

#include <QDir>
#include <QNetworkAccessManager>
#include <iostream>
#include <string>

#include "ui_mainwindow.h"

void MainWindow::untick() {}

void MainWindow::outputMenuClosed() { outputMenuOpen = false; }

void MainWindow::eventWindowClosed() { eventwindowOpen = false; }

void MainWindow::optionMenuClosed() { optionMenuOpen = false; }

void MainWindow::openSettings() {
  if (!optionMenuOpen) {
    optionMenuOpen = true;
    QWidget *wdg = new optionsMenu;
    connect(wdg, SIGNAL(closedOptionsMenu()), this,
            SIGNAL(closedOptionsMenu()));
    wdg->show();
  }
}

void MainWindow::openOutputMenu() {
  if (!outputMenuOpen) {
    outputMenuOpen = true;
    QWidget *wdg = new OutputMenu;
    connect(wdg, SIGNAL(closedOutputMenu()), this, SIGNAL(closedOutputMenu()));
    wdg->show();
  }
}
void MainWindow::openCalibrateAxis() {
  if (!calibrateAxisOpen) {
    calibrateAxisOpen = true;
    QWidget *wdg = new CalibrateAxisMenu;
    connect(wdg, SIGNAL(closedOutputMenu()), this, SIGNAL(closedOutputMenu()));
    wdg->show();
  }
}
void MainWindow::openEditEventMenu() {
  if (!eventwindowOpen) {
    eventwindowOpen = true;
    QWidget *wdg = new EventWindow;
    connect(wdg, SIGNAL(closedEventWindow()), this,
            SIGNAL(closedEventWindow()));
    wdg->show();
  }
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

void MainWindow::installWasm() {
  bool customPathFound =
      settingsHandler.retrieveSetting("Settings", "communityFolderPathLabel");
  QString pathfound;
  QString sourceString =
      QCoreApplication::applicationDirPath() + "/BitsAndDroidsModule";
  qDebug() << "start install";
  if (customPathFound) {
    qDebug() << "custom path found";
    pathfound =
        settingsHandler.retrieveSetting("Settings", "communityFolderPathLabel")
            ->toString();
  } else {
    QString windowsStorePath =
        QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation) +
        "/Packages/Microsoft.FlightSimulator_"
        "8wekyb3d8bbwe/LocalCache/packages/Community";
    bool windowsFound = QDir(windowsStorePath).exists();
    QString steamPath =
        QStandardPaths::writableLocation(QStandardPaths::RuntimeLocation) +
        "/AppData/Roaming/Microsoft Flight Simulator/Packages/Community";
    bool steamFound = QDir(steamPath).exists();

    if (windowsFound) {
      pathfound = windowsStorePath;
    } else if (steamFound) {
      pathfound = steamPath;
    } else {
      auto notFoundMessage = new QMessageBox();
      notFoundMessage->setInformativeText(
          "Could not automatically find the community folder");
      notFoundMessage->setStandardButtons(QMessageBox::Save |
                                          QMessageBox::Cancel);
      int ret = notFoundMessage->exec();

      switch (ret) {
        case QMessageBox::Save: {
          QFileDialog dialog(this);
          dialog.setFileMode(QFileDialog::Directory);

          QString communityFolderPath = dialog.getExistingDirectory();
          settingsHandler.storeValue("Settings", "communityFolderPathLabel",
                                     communityFolderPath);
          break;
        }
        case QMessageBox::Cancel:
          break;
      }
    }
  }

  QString destinationString = pathfound + "/BitsAndDroidsModule";
  std::cout << "destination: " << destinationString.toStdString() << std::endl;
  copyFolder(sourceString, destinationString);
}

void MainWindow::copyFolder(QString sourceFolder, QString destinationFolder) {
  qDebug() << "Dest path = " << destinationFolder;
  QDir sourceDir(sourceFolder);

  QDir destinationDir(destinationFolder);

  if (!sourceDir.exists()) {
    qDebug() << "dest not found";
    return;
  }

  if (!destinationDir.exists()) {
    destinationDir.mkdir(destinationFolder);
    qDebug() << "Dir not present";
  }

  QStringList files = sourceDir.entryList(QDir::Files);
  qDebug() << files.size() << " Files found";
  for (int i = 0; i < files.count(); i++) {
    QString sourceName = sourceFolder + "/" + files[i];
    QString destName = destinationFolder + "/" + files[i];
    if (QFile::exists(destName)) {
      if (files[i].contains("events")) {
        QString newPath = destName + "Old";
        QFile::copy(destName, newPath);
      }
      QFile::remove(destName);
    }
    QFile::copy(sourceName, destName);
  }
  QStringList dirs = sourceDir.entryList(QDir::AllDirs | QDir::NoDotAndDotDot);
  for (auto &dir : dirs) {
    QString sourceName = sourceFolder + "/" + dir;
    QString destinationName = destinationFolder + "/" + dir;
    copyFolder(sourceName, destinationName);
  }
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  updateButton = ui->updateButton;
  ui->setupUi(this);
  std::cout << "11111" << std::endl;
  std::cout << inputComRowCounter << "Rows init" << std::endl;

  availableSets = formbuilder.getAvailableSets();
  connect(this, &MainWindow::closedOutputMenu, this,
          &MainWindow::outputMenuClosed);
  connect(this, &MainWindow::closedOptionsMenu, this,
          &MainWindow::optionMenuClosed);
  connect(this, &MainWindow::closedEventWindow, this,
          &MainWindow::eventWindowClosed);
  loadComPortData();

  qRegisterMetaType<QList<QString>>("QList<QString>");
  // STYLE AFFECTING SECTION
  //-----------------------
  ui->messagesWidgetLayout->setAlignment(Qt::AlignBottom);

  // TOOLBAR MENU
  qDebug() << QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
  auto *openSettings = new QAction("&Settings", this);
  auto *calibrateAxis = new QAction("&Calibrate axis", this);
  auto *openOutputMenu = new QAction("&Outputs", this);
  auto *openEditEventWindow = new QAction("&Edit events", this);
  auto *installWasm = new QAction("&Install WASM", this);
  auto *WasmUpdateEventFile = new QAction("&Update event file", this);

  QMenu *Settings = menuBar()->addMenu("&Settings");
  QMenu *OutputSettings = menuBar()->addMenu("&Outputs");
  QMenu *WasmInstall = menuBar()->addMenu("&WASM");

  WasmInstall->addAction(WasmUpdateEventFile);
  WasmInstall->addAction(installWasm);
  OutputSettings->addAction(openOutputMenu);
  WasmInstall->addAction(openEditEventWindow);
  Settings->addAction(openSettings);
  Settings->addAction(calibrateAxis);

  Settings->addAction("Version " + QString(constants::VERSION));

  // SIGNALS + SLOTS
  connect(WasmUpdateEventFile, &QAction::triggered, this,
          &MainWindow::localUpdateEventFile);
  connect(WasmUpdateEventFile, &QAction::triggered, this,
          &MainWindow::localUpdateEventFile);
  connect(calibrateAxis, &QAction::triggered, this,
          &MainWindow::openCalibrateAxis);
  connect(openOutputMenu, &QAction::triggered, this,
          &MainWindow::openOutputMenu);
  connect(installWasm, &QAction::triggered, this, &MainWindow::installWasm);

  connect(&outputThread, &OutputWorker::BoardConnectionMade, this,
          &MainWindow::BoardConnectionMade);
  connect(&outputThread, &OutputWorker::GameConnectionMade, this,
          &MainWindow::GameConnectionMade);

  connect(&inputThread, &InputWorker::BoardConnectionMade, this,
          &MainWindow::BoardConnectionMade);
  connect(&inputThread, &InputWorker::GameConnectionMade, this,
          &MainWindow::GameConnectionMade);

  connect(&dualThread, &DualWorker::BoardConnectionMade, this,
          &MainWindow::BoardConnectionMade);
  connect(&dualThread, &DualWorker::GameConnectionMade, this,
          &MainWindow::GameConnectionMade);
  connect(openEditEventWindow, &QAction::triggered, this,
          &MainWindow::openEditEventMenu);
  connect(openSettings, &QAction::triggered, this, &MainWindow::openSettings);
  connect(&formbuilder, &FormBuilder::addPressed, this, &MainWindow::addCom);
  connect(&formbuilder, &FormBuilder::stopPressed, this, &MainWindow::stopMode);
  connect(&formbuilder, &FormBuilder::startPressed, this,
          &MainWindow::startMode);
  connect(&formbuilder, &FormBuilder::refreshPressed, this,
          &MainWindow::refreshComs);
  connect(&dualThread, &DualWorker::updateLastValUI, this,
          &MainWindow::onUpdateLastValUI);
  connect(&inputThread, &InputWorker::updateLastStatusUI, this,
          &MainWindow::onUpdateLastStatusUI);
  connect(&outputThread, SIGNAL(updateLastStatusUI(QString)),
          SLOT(onUpdateLastStatusUI(QString)));

  // IMPORTANT IMPROV SECTION

  formbuilder.loadComPortData();

  // INPUTS
  QVBoxLayout *inContainer = ui->inLayoutContainer;

  auto *shadow = new QGraphicsDropShadowEffect();
  QWidget *inWidget = ui->inWidgetContainer;
  shadow->setBlurRadius(20);
  shadow->setOffset(2, 2);
  inWidget->setGraphicsEffect(shadow);
  inContainer->setAlignment(Qt::AlignTop);

  inContainer->addWidget(formbuilder.generateComControls(1));
  inContainer->parentWidget()->setSizePolicy(QSizePolicy::Expanding,
                                             QSizePolicy::Expanding);

  auto inputWarningBox = new QVBoxLayout();
  inputWarningBox->setObjectName("inputWarningBox");
  inContainer->addLayout(inputWarningBox);

  // OUTPUTS
  QVBoxLayout *outContainer = ui->outLayoutContainer;
  QWidget *outWidget = ui->outWidgetContainer;
  auto *shadowOut = new QGraphicsDropShadowEffect();
  shadowOut->setBlurRadius(20);
  shadowOut->setOffset(2, 2);
  outWidget->setGraphicsEffect(shadowOut);
  outContainer->setAlignment(Qt::AlignTop);

  outContainer->addWidget(formbuilder.generateComControls(2));

  outContainer->parentWidget()->setSizePolicy(QSizePolicy::Expanding,
                                              QSizePolicy::Expanding);

  auto outputWarningBox = new QVBoxLayout();
  outputWarningBox->setObjectName("outputWarningBox");
  outContainer->addLayout(outputWarningBox);

  // DUAL MODE
  QVBoxLayout *dualContainer = ui->dualLayoutContainer;
  QWidget *dualWidget = ui->dualWidgetContainer;

  auto *shadowDual = new QGraphicsDropShadowEffect();
  shadowDual->setBlurRadius(20);
  shadowDual->setOffset(2, 2);
  dualWidget->setGraphicsEffect(shadowDual);
  dualContainer->setAlignment(Qt::AlignTop);

  dualContainer->addWidget(formbuilder.generateComControls(3));

  dualContainer->parentWidget()->setSizePolicy(QSizePolicy::Expanding,
                                               QSizePolicy::Expanding);

  auto dualWarningBox = new QVBoxLayout();
  dualWarningBox->setObjectName("dualWarningBox");
  dualContainer->addLayout(dualWarningBox);

  auto *mgr = new QNetworkAccessManager(this);
  connect(mgr, &QNetworkAccessManager::finished, this, &MainWindow::onfinish);
  connect(mgr, &QNetworkAccessManager::finished, mgr, &QObject::deleteLater);
  mgr->get(
      QNetworkRequest(QUrl("https://www.bitsanddroids.com/assets/"
                           "downloads/connector/version.html")));

  // THIS SECTION POPULATES THE COMBOBOXES
  auto widgetsToSet = QList<QWidget *>();
  widgetsToSet.append(dualWidget);
  widgetsToSet.append(outWidget);
  widgetsToSet.append(inWidget);

  for (auto &widget : widgetsToSet) {
    QString setGroupName;
    QString comGroupName;
    QString name = widget->objectName();
    int mode = 1;
    bool setsNeeded = true;
    if (name == "dualWidgetContainer") {
      setGroupName = "dualSets";
      comGroupName = "dualComs";
      mode = 3;
    } else if (name == "outWidgetContainer") {
      setGroupName = "outputSets";
      comGroupName = "outputComs";
      mode = 2;
    } else {
      comGroupName = "inputComs";
      setsNeeded = false;
    }

    QStringList *outputKeys = settingsHandler.retrieveKeys(comGroupName);

    for (int i = 0; i < outputKeys->size(); i++) {
      QWidget *comSelector =
          formbuilder.generateComSelector(setsNeeded, mode, i);
      widget->layout()->addWidget(comSelector);
    }

    if (outputKeys->empty()) {
      widget->layout()->addWidget(
          formbuilder.generateComSelector(true, mode, 0));
    }

    auto comboBoxes = widget->findChildren<QComboBox *>();
    int itterations = comboBoxes.size();
    if (itterations > 1) {
      itterations = itterations / 2;
    }

    for (int i = 0; i < itterations; i++) {
      auto comComboBox =
          widget->findChild<QComboBox *>("comBox" + QString::number(i));
      auto lastComSaved =
          settingsHandler
              .retrieveSetting(comGroupName, "com" + QString::number(i))
              ->toString()
              .mid(4);
      comComboBox->setCurrentIndex(getComboxIndex(comComboBox, lastComSaved));
      qDebug() << "LAST COM SAVED " << lastComSaved;

      if (setsNeeded) {
        auto comboBox =
            widget->findChild<QComboBox *>("setBox" + QString::number(i));

        auto lastSetId =
            settingsHandler
                .retrieveSetting(setGroupName, "set" + QString::number(i))
                ->toString();
        auto setFound = setHandler->getSetById(lastSetId);
        auto setName = setFound.getSetName();
        comboBox->setCurrentIndex(getComboxIndex(comboBox, setName));
      }
    }

    auto widgetContainer = new QWidget();
    auto connectionRow = new QHBoxLayout();
    connectionRow->setAlignment(Qt::AlignLeft);
    auto radioConBoard = new QRadioButton();
    radioConBoard->setObjectName(widget->objectName() + "BoardCon");
    radioConBoard->setChecked(true);
    radioConBoard->setEnabled(false);
    auto radioConGame = new QRadioButton();
    radioConGame->setObjectName(widget->objectName() + "GameCon");
    radioConGame->setEnabled(false);
    radioConGame->setChecked(true);
    QString labelStyle = "QLabel{color: #5c5c5c; font-size: 10px;}";
    auto boardLabel = new QLabel("Boards");
    boardLabel->setStyleSheet(labelStyle);
    auto gameLabel = new QLabel("MFS2020");
    gameLabel->setStyleSheet(labelStyle);
    connectionRow->addWidget(radioConBoard);
    connectionRow->addWidget(boardLabel);
    connectionRow->addWidget(radioConGame);
    connectionRow->addWidget(gameLabel);
    widgetContainer->setLayout(connectionRow);
    widget->layout()->addWidget(widgetContainer);
  }

  this->adjustSize();
}
void MainWindow::localUpdateEventFile() {
  if (dualThread.isRunning()) {
    connect(this, &MainWindow::updateEventFile, &dualThread,
            &DualWorker::updateEventFile);

    emit updateEventFile();
  } else if (inputThread.isRunning()) {
    connect(this, &MainWindow::updateEventFile, &inputThread,
            &InputWorker::updateEventFile);
    emit updateEventFile();
  }
}
void MainWindow::GameConnectionMade(int con, int mode) {
  qDebug() << "ConnectionReceived";
  auto gameRadioButton = new QRadioButton();
  switch (mode) {
    case 1:
      gameRadioButton = ui->inWidgetContainer->findChild<QRadioButton *>(
          "inWidgetContainerGameCon");
      break;
    case 2:
      gameRadioButton = ui->outWidgetContainer->findChild<QRadioButton *>(
          "outWidgetContainerGameCon");
      break;
    case 3:
      gameRadioButton = ui->dualWidgetContainer->findChild<QRadioButton *>(
          "dualWidgetContainerGameCon");
      break;
    default:
      break;
  }
  if (con == 0) {
    gameRadioButton->setStyleSheet(
        "QRadioButton::indicator{border: 1px solid darkgray; background-color: "
        "red; border-radius: 7px; height: 12px; width: 12px;}");
  }
  if (con == 1) {
    gameRadioButton->setStyleSheet(
        "QRadioButton::indicator{border: 1px solid darkgray; background-color: "
        "orange; border-radius: 7px;height: 12px; width: 12px;}");
  }
  if (con == 2) {
    gameRadioButton->setStyleSheet(
        "QRadioButton::indicator{border: 1px solid darkgray; background-color: "
        "green; border-radius: 7px;height: 12px; width: 12px;}");
  }
}

void MainWindow::BoardConnectionMade(int con, int mode) {
  qDebug() << "ConnectionReceived";
  auto boardRadioButton = new QRadioButton();
  switch (mode) {
    case 1:
      boardRadioButton = ui->inWidgetContainer->findChild<QRadioButton *>(
          "inWidgetContainerBoardCon");
      break;
    case 2:
      boardRadioButton = ui->outWidgetContainer->findChild<QRadioButton *>(
          "outWidgetContainerBoardCon");
      break;
    case 3:
      boardRadioButton = ui->dualWidgetContainer->findChild<QRadioButton *>(
          "dualWidgetContainerBoardCon");
      break;
    default:
      break;
  }
  if (con == 0) {
    boardRadioButton->setStyleSheet(
        "QRadioButton::indicator{border: 1px solid darkgray; background-color: "
        "red; border-radius: 7px; height: 12px; width: 12px;}");
  }
  if (con == 1) {
    boardRadioButton->setStyleSheet(
        "QRadioButton::indicator{border: 1px solid darkgray; background-color: "
        "orange; border-radius: 7px;height: 12px; width: 12px;}");
  }
  if (con == 2) {
    boardRadioButton->setStyleSheet(
        "QRadioButton::indicator{border: 1px solid darkgray; background-color: "
        "green; border-radius: 7px;height: 12px; width: 12px;}");
  }
}

void MainWindow::onfinish(QNetworkReply *rep) {
  try {
    if (rep->error() == QNetworkReply::NoError) {
      QByteArray bts = rep->readAll();

      QString str(bts);

      QStringList versionCut = str.split("~");

      if (versionCut[1].toStdString() != version) {
        std::string nr = versionCut[1].toStdString();
        std::string buttonText = "New update available: " + nr;
        ui->updateButton->setText(QString::fromStdString(buttonText));
        // url = versionCut[3].toStdString();
      } else {
        ui->updateButton->setVisible(false);
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
      setHandler->updateSets();
      startOutputs();
      break;
    case 3:
      setHandler->updateSets();
      startDual();
      break;
    default:
      break;
  }
}

void MainWindow::startInputs() {
  auto *widget = new QWidget();
  inputThread.abortInput = false;
  widget = ui->inWidgetContainer;
  settingsHandler.clearKeys("inputComs");
  QRegularExpression search("comBox");
  QList<QComboBox *> comList = widget->findChildren<QComboBox *>(search);
  bool emptyInputCom = checkIfComboIsEmpty(comList);
  if (!emptyInputCom) {
    QString comboBoxName;
    QString key;

    for (int i = 0; i < comList.size(); i++) {
      key = "com" + QString::number(i);
      comboBoxName = "comboBoxRow" + QString::number(i);

      QString keyValue = comList[i]->currentText();
      settingsHandler.storeValue("inputComs", key,
                                 convertComPort(keyValue).c_str());
    }

    inputThread.start();
  } else {
    auto *startButton =
        ui->inWidgetContainer->findChild<QPushButton *>("2startButton");
    startButton->setChecked(false);
    startButton->setText("Start");
    startButton->setEnabled(true);

    auto *stopButton =
        ui->inWidgetContainer->findChild<QPushButton *>("2stopBtn");
    stopButton->setChecked(true);
    stopButton->setStyleSheet("background-color:#0F4C5C");

    auto outWarningBox =
        ui->inWidgetContainer->findChild<QLayout *>("outputWarningBox");
    clearChildrenFromLayout(outWarningBox);

    // always true, check is in here for if I ever add more error messages
    if (emptyInputCom) {
      outWarningBox->addWidget(returnWarningString(NOCOMPORT));
    }
  }
}

void MainWindow::startOutputs() {
  auto *widget = new QWidget();

  widget = ui->outWidgetContainer;

  settingsHandler.clearKeys("outputComs");
  settingsHandler.clearKeys("outputSets");

  QRegularExpression search("comBox");
  QList<QComboBox *> comList = widget->findChildren<QComboBox *>(search);

  QRegularExpression searchSets("setBox");
  QList<QComboBox *> setList = widget->findChildren<QComboBox *>(searchSets);
  bool emptyDualSet = checkIfComboIsEmpty(setList);
  bool emptyDualCom = checkIfComboIsEmpty(comList);
  if (!emptyDualSet && !emptyDualCom) {
    QString key;
    QString setKey;
    QList<Output *> outputsToMap = QList<Output *>();
    outputThread.clearBundles();
    for (int i = 0; i < comList.size(); i++) {
      key = "com" + QString::number(i);
      setKey = "set" + QString::number(i);

      QString keyValue = comList[i]->currentText();
      QString setKeyValue = setList[i]->currentText();

      settingsHandler.storeValue("outputComs", key,
                                 convertComPort(keyValue).c_str());

      int index = setList[i]->currentIndex();

      int id = availableSets->at(index).getID();

      auto *bundle = new outputBundle();

      set active = setHandler->getSetById(QString::number(id));
      bundle->setSet(active);

      auto *outputs = new QMap<int, Output *>();
      *outputs = active.getOutputs();
      QMap<int, Output *>::iterator j;
      for (j = outputs->begin(); j != outputs->end(); j++) {
        outputsToMap.append(j.value());
      }
      outputThread.setOutputsToMap(outputsToMap);

      bundle->setOutputsInSet(*outputs);
      outputThread.addBundle(bundle);
      settingsHandler.storeValue("outputSets", setKey, id);
    }
    outputThread.abort = false;
    outputThread.start();
  } else {
    auto *startButton =
        ui->outWidgetContainer->findChild<QPushButton *>("2startButton");
    startButton->setChecked(false);
    startButton->setText("Start");
    startButton->setEnabled(true);

    auto *stopButton =
        ui->outWidgetContainer->findChild<QPushButton *>("2stopBtn");
    stopButton->setChecked(true);
    stopButton->setStyleSheet("background-color:#0F4C5C");

    auto outWarningBox =
        ui->outWidgetContainer->findChild<QLayout *>("outputWarningBox");
    clearChildrenFromLayout(outWarningBox);
    if (emptyDualCom) {
      outWarningBox->addWidget(returnWarningString(NOCOMPORT));
    }
    if (emptyDualSet) {
      outWarningBox->addWidget(returnWarningString(NOSET));
    }
  }
}

void MainWindow::startDual() {
  auto *widget = new QWidget();

  widget = ui->dualWidgetContainer;
  settingsHandler.clearKeys("dualComs");
  settingsHandler.clearKeys("dualSets");

  QRegularExpression search("comBox");
  QList<QComboBox *> comList = widget->findChildren<QComboBox *>(search);

  QRegularExpression searchSets("setBox");
  QList<QComboBox *> setList = widget->findChildren<QComboBox *>(searchSets);

  bool emptyDualSet = checkIfComboIsEmpty(setList);
  bool emptyDualCom = checkIfComboIsEmpty(comList);
  if (!emptyDualSet && !emptyDualCom) {
    QString key;
    QString setKey;
    QList<Output *> outputsToMap;
    qDebug() << "sets available" << comList.size();
    dualThread.clearBundles();
    for (int i = 0; i < comList.size(); i++) {
      key = "com" + QString::number(i);
      setKey = "set" + QString::number(i);

      QString keyValue = comList[i]->currentText();
      QString setKeyValue = setList[i]->currentText();
      qDebug() << setKeyValue;
      settingsHandler.storeValue("dualComs", key,
                                 convertComPort(keyValue).c_str());

      qDebug() << " SET INDEX " << setList.at(i)->currentIndex();
      int index = setList[i]->currentIndex();

      int id = availableSets->at(index).getID();

      auto *bundle = new outputBundle();

      set active = setHandler->getSetById(QString::number(id));
      bundle->setSet(active);

      auto *outputs = new QMap<int, Output *>();
      *outputs = active.getOutputs();
      QMap<int, Output *>::iterator j;
      for (j = outputs->begin(); j != outputs->end(); j++) {
        outputsToMap.append(j.value());
      }
      dualThread.setOutputsToMap(outputsToMap);
      bundle->setOutputsInSet(*outputs);
      dualThread.addBundle(bundle);
      settingsHandler.storeValue("dualSets", setKey, id);
    }

    //    ui->stopButton->setVisible(true);
    //    QString comText = ui->outputComboBoxBase->currentText();
    // std::string comNr
    //    R"(\\.\COM)" + comText.toStdString().std::string::substr(3, 2);

    dualThread.abortDual = false;
    dualThread.start();
  } else {
    auto *startButton =
        ui->dualWidgetContainer->findChild<QPushButton *>("3startButton");
    startButton->setChecked(false);
    startButton->setText("Start");
    startButton->setEnabled(true);

    auto *stopButton =
        ui->dualWidgetContainer->findChild<QPushButton *>("3stopBtn");
    stopButton->setChecked(true);
    stopButton->setStyleSheet("background-color:#0F4C5C");

    auto dualWarningBox =
        ui->dualWidgetContainer->findChild<QLayout *>("dualWarningBox");
    clearChildrenFromLayout(dualWarningBox);
    if (emptyDualCom) {
      dualWarningBox->addWidget(returnWarningString(NOCOMPORT));
    }
    if (emptyDualSet) {
      dualWarningBox->addWidget(returnWarningString(NOSET));
    }
  }
}

bool MainWindow::checkIfComboIsEmpty(QList<QComboBox *> toCheck) {
  for (auto &i : toCheck) {
    if (i->currentIndex() == -1) {
      return true;
    } else
      return false;
  }
}

void MainWindow::clearChildrenFromLayout(QLayout *toClear) {
  if (toClear != nullptr) {
    QLayoutItem *item;
    while ((item = toClear->takeAt(0)) != nullptr) {
      delete item->widget();
      delete item;
    }
  }
}

QLabel *MainWindow::returnWarningString(int warningType) {
  auto warningLabel = new QLabel();
  warningLabel->setStyleSheet("color:#B33A3A");
  switch (warningType) {
    case NOCOMPORT:
      warningLabel->setText("Please select a com port before pressing start");
      break;
    case NOSET:
      warningLabel->setText(
          "Please create or select a set before pressing start");
      break;
  }
  return warningLabel;
}

void MainWindow::refreshComs(int mode) {
  auto *widget = new QWidget();

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
    default:
      break;
  }
  QRegularExpression search("comBox");
  QList<QComboBox *> comList = widget->findChildren<QComboBox *>(search);
  formbuilder.loadComPortData();
  auto sets = *formbuilder.getAvailableSets();
  QList<QString> coms = formbuilder.getAvailableComPorts();

  for (int i = 0; i < comList.size(); i++) {
    qDebug() << "hit size:" << comList.size();

    comList[i]->clear();
    for (auto &com : coms) {
      comList[i]->addItem(com);
      qDebug() << com;
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
    default:
      break;
  }
  BoardConnectionMade(0, mode);
  GameConnectionMade(0, mode);
}

void MainWindow::addCom(int mode) {
  auto *layout = new QVBoxLayout();
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
    default:
      break;
  }

  layout->addWidget(formbuilder.generateComSelector(set, mode, 99));
  qDebug() << "addCom" << mode;
}

void MainWindow::stopInput() {
  inputThread.abortInput = true;

  inputThread.quit();
}

void MainWindow::stopOutput() {
  outputThread.abort = true;
  outputThread.quit();
}

void MainWindow::stopDual() { dualThread.abortDual = true; }

void MainWindow::on_updateButton_clicked() {
  qDebug() << "clicked";
  QString qUrl = QString::fromStdString(url);
  QDesktopServices::openUrl(QUrl(qUrl));
}

int MainWindow::getComboxIndex(QComboBox *comboBox, QString value) {
  int index = 0;
  qDebug() << value;
  for (int i = 0; i < comboBox->count(); i++) {
    QString text = comboBox->itemText(i);
    if (text.contains(value)) {
      index = i;
    }
  }
  return index;
}
void MainWindow::closeEvent(QCloseEvent *event) { delete ui; }
MainWindow::~MainWindow() { delete ui; }
