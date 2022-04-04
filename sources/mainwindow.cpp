#include "headers/mainwindow.h"

#include <events/eventwindow.h>
#include <library/librarygeneratorwindow.h>
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
void MainWindow::calibrateAxisMenuClosed() { calibrateAxisMenuOpen = false; }

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
  if (!calibrateAxisMenuOpen) {
    calibrateAxisMenuOpen = true;
    QWidget *wdg = new CalibrateAxisMenu;
    connect(wdg, SIGNAL(closedCalibrateAxisMenu()), this,
            SIGNAL(closedCalibrateAxisMenu()));
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
void MainWindow::openGenerateLibraryMenu() {
  if (generateLibraryMenuOpen) {
    generateLibraryMenuOpen = true;
    QWidget *wdg = new LibraryGeneratowWindow;
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
void MainWindow::toggleAdvanced() {
  auto inWidget = this->findChild<QWidget *>("inWidgetContainer");
  auto outWidget = this->findChild<QWidget *>("outWidgetContainer");
  auto dualWidget = this->findChild<QWidget *>("dualWidgetContainer");
  auto headerLabel = this->findChild<QWidget *>("headerDUAL");
  inWidget->setVisible(advancedMode);
  outWidget->setVisible(advancedMode);
  headerLabel->setVisible(advancedMode);
  if (!advancedMode) {
    dualWidget->resize(150, dualWidget->height());
    dualWidget->layout()->setSizeConstraint(QLayout::SetMinimumSize);
    dualWidget->adjustSize();

    ui->centralwidget->resize(250, this->height());
    ui->centralwidget->setMinimumWidth(250);
    ui->centralwidget->layout()->setSizeConstraint(QLayout::SetMinimumSize);
    ui->centralwidget->adjustSize();
    this->adjustSize();
  } else {
  }
  settingsHandler.storeValue("Settings", "advancedMode", advancedMode);
  advancedMode = !advancedMode;
  cout << "PRESSED" << endl;
}
void MainWindow::installWasm() {
  bool customPathFound = pathHandler.getCommunityFolderPath() != nullptr;
  QString pathfound;
  QString sourceString =
      QCoreApplication::applicationDirPath() + "/BitsAndDroidsModule";
  qDebug() << "start install";
  if (customPathFound) {
    qDebug() << "custom path found";
    pathfound = pathHandler.getCommunityFolderPath();
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

  copyFolder(sourceString, destinationString);
  qDebug() << applicationEventsPath;
}

void MainWindow::copyFolder(QString sourceFolder, QString destinationFolder) {
  qDebug() << "Dest path = " << destinationFolder;
  QDir sourceDir(sourceFolder);

  QDir destinationDir(destinationFolder);

  if (!sourceDir.exists()) {
    qDebug() << "dest not found" << sourceDir;
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
    if (!(files[i] == ("events.txt"))) {
      qDebug() << "Coppied " << files[i];
      QFile::copy(sourceName, destName);
    }
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
  ui->setupUi(this);

  updateButton = ui->updateButton;

  availableSets = formbuilder.getAvailableSets();
  connect(this, &MainWindow::closedOutputMenu, this,
          &MainWindow::outputMenuClosed);
  connect(this, &MainWindow::closedOptionsMenu, this,
          &MainWindow::optionMenuClosed);
  connect(this, &MainWindow::closedEventWindow, this,
          &MainWindow::eventWindowClosed);
  connect(this, &MainWindow::closedCalibrateAxisMenu, this,
          &MainWindow::calibrateAxisMenuClosed);
  loadComPortData();

  qRegisterMetaType<QList<QString>>("QList<QString>");
  // STYLE AFFECTING SECTION
  //-----------------------
  ui->messagesWidgetLayout->setAlignment(Qt::AlignBottom);

  // TOOLBAR MENU
  qDebug() << QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
  auto *openSettings = new QAction("&Settings", this);

  auto *toggleAdvancedAction = new QAction("&Toggle advanced mode", this);
  auto *libraryGenerator = new QAction("&Generate library", this);
  auto *calibrateAxis = new QAction("&Calibrate axis", this);
  auto *openOutputMenu = new QAction("&Outputs", this);
  auto *openEditEventWindow = new QAction("&Edit events", this);
  auto *installWasm = new QAction("&Install WASM", this);
  auto *WasmUpdateEventFile = new QAction("&Update event file", this);
  auto *updateApplication = new QAction("Check for updates", this);
  QMenu *Settings = menuBar()->addMenu("&Settings");
  QMenu *libraryMenu = menuBar()->addMenu("&Library");
  QMenu *viewMenu = menuBar()->addMenu("&View");
  QMenu *OutputSettings = menuBar()->addMenu("&Outputs");
  QMenu *WasmInstall = menuBar()->addMenu("&WASM");

  auto icon = new QSystemTrayIcon(QIcon(":/BitsAndDroidsLogo.ico"), this);

  connect(icon, &QSystemTrayIcon::activated, this, &MainWindow::toggleOpen);

  QAction *quit_action = new QAction("Exit", icon);
  connect(quit_action, &QAction::triggered, this, &MainWindow::exitProgram);

  QAction *hide_action = new QAction("Show/Hide", icon);

  QMenu *tray_icon_menu = new QMenu;
  // tray_icon_menu->addAction(hide_action);
  tray_icon_menu->addAction(quit_action);

  icon->setContextMenu(tray_icon_menu);

  icon->show();

  WasmInstall->addAction(WasmUpdateEventFile);
  WasmInstall->addAction(installWasm);
  viewMenu->addAction(toggleAdvancedAction);
  OutputSettings->addAction(openOutputMenu);
  WasmInstall->addAction(openEditEventWindow);
  Settings->addAction(openSettings);
  Settings->addAction(updateApplication);

  Settings->addAction(calibrateAxis);

  Settings->addAction("Version " + QString(constants::VERSION));
  // libraryMenu->addAction(libraryGenerator);

  // SIGNALS + SLOTS
  connect(WasmUpdateEventFile, &QAction::triggered, this,
          &MainWindow::localUpdateEventFile);
  connect(WasmUpdateEventFile, &QAction::triggered, this,
          &MainWindow::localUpdateEventFile);
  connect(calibrateAxis, &QAction::triggered, this,
          &MainWindow::openCalibrateAxis);
  connect(openOutputMenu, &QAction::triggered, this,
          &MainWindow::openOutputMenu);
  connect(updateApplication, &QAction::triggered, this,
          &MainWindow::checkForUpdates);
  connect(installWasm, &QAction::triggered, this, &MainWindow::installWasm);
  connect(toggleAdvancedAction, &QAction::triggered, this,
          &MainWindow::toggleAdvanced);
  connect(&outputThread, &OutputWorker::BoardConnectionMade, this,
          &MainWindow::BoardConnectionMade);
  connect(&outputThread, &OutputWorker::GameConnectionMade, this,
          &MainWindow::GameConnectionMade);
  connect(&libraryGenerator, &QAction::triggered, this,
          &MainWindow::openGenerateLibraryMenu);

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
    restoreStoredValuesComboBoxes(widget, comGroupName, setGroupName,
                                  setsNeeded);

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
  if (!settingsHandler.retrieveSetting("Settings", "advancedMode")->toBool()) {
    toggleAdvanced();
  }
  checkForUpdates(true);
  settingsHandler.checkEventFilePresent();
  this->adjustSize();
}
void MainWindow::restoreStoredValuesComboBoxes(QWidget *widget,
                                               QString comGroupName,
                                               QString setGroupName,
                                               bool setsNeeded) {
  auto comboBoxes = widget->findChildren<QComboBox *>();
  int itterations = comboBoxes.size();
  if (itterations > 1) {
    itterations = itterations / 2;
  }

  for (int i = 0; i < itterations; i++) {
    auto comComboBox =
        widget->findChild<QComboBox *>("comBox" + QString::number(i));
    if (!(comComboBox == nullptr)) {
      auto lastComSaved =
          settingsHandler
              .retrieveSetting(comGroupName, "com" + QString::number(i))
              ->toString()
              .mid(4);
      if (getComboxIndex(comComboBox, lastComSaved) != -10) {
        comComboBox->setCurrentIndex(getComboxIndex(comComboBox, lastComSaved));
      } else {
        qDebug() << "ELSE";
        comComboBox->addItem("Not connected");
        qDebug() << "ADDED NOT CONNECTED";
        comComboBox->setCurrentIndex(
            getComboxIndex(comComboBox, "Not connected"));
      }

      qDebug() << "LAST COM SAVED " << lastComSaved;

      if (setsNeeded) {
        auto comboBox =
            widget->findChild<QComboBox *>("setBox" + QString::number(i));
        if (!settingsHandler
                 .retrieveSetting(setGroupName, "set" + QString::number(i))
                 ->isNull()) {
          auto lastSetId =
              settingsHandler
                  .retrieveSetting(setGroupName, "set" + QString::number(i))
                  ->toString();
          auto setFound = setHandler->getSetById(lastSetId);
          auto setName = setFound.getSetName();
          comboBox->setCurrentIndex(getComboxIndex(comboBox, setName));
        }
      }
    }
  }
}
void MainWindow::checkForUpdates(bool silentCheck) {
  QProcess *process = new QProcess(this);
  process->start(pathHandler.getMaintenanceToolPath() + " ch");
  process->waitForFinished();
  QByteArray data = process->readAll();
  qDebug() << data;
  auto mb = new QMessageBox();
  ui->updateButton->setVisible(false);
  if (data.contains("no updates available") && !silentCheck) {
    mb->setText("No updates available");
    mb->exec();
  } else if (data.contains("Wait until it finishes")) {
    auto mb = new QMessageBox();
    mb->setText(
        "Another instance of the maintenance tool is already running\n Please "
        "close it before trying again.");
    mb->exec();
  } else if (data.contains("update name")) {
    ui->updateButton->setText("Update available");
    ui->updateButton->setVisible(true);
  }
}

void MainWindow::localUpdateEventFile() {
  QFile::remove(pathHandler.getCommunityFolderPath() +
                "/BitsAndDroidsModule/modules/events.txt");
  QFile::copy(applicationEventsPath,
              pathHandler.getCommunityFolderPath() +
                  "/BitsAndDroidsModule/modules/events.txt");
  if (dualThread.isRunning()) {
    connect(this, &MainWindow::updateEventFile, &dualThread,
            &DualWorker::sendWASMCommand);

    emit updateEventFile('9');
  } else if (inputThread.isRunning()) {
    connect(this, &MainWindow::updateEventFile, &inputThread,
            &InputWorker::sendWASMCommand);
    emit updateEventFile('9');
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
  settingsHandler.clearKeys("runningInputComs");
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
      settingsHandler.storeValue("runningInputComs", key,
                                 convertComPort(keyValue).c_str());
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

  settingsHandler.clearKeys("runningOutputComs");
  settingsHandler.clearKeys("runningOutputSets");

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

      settingsHandler.storeValue("runningOutputComs", key,
                                 convertComPort(keyValue).c_str());
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
      settingsHandler.storeValue("runningOutputSets", setKey, id);
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
  settingsHandler.clearKeys("runningDualComs");
  settingsHandler.clearKeys("runningDualSets");

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
      if (!(comList[i]->currentText().contains("Not connected"))) {
        key = "com" + QString::number(i);
        setKey = "set" + QString::number(i);

        QString keyValue = comList[i]->currentText();
        QString setKeyValue = setList[i]->currentText();
        qDebug() << setKeyValue;

        settingsHandler.storeValue("runningDualComs", key,
                                   convertComPort(keyValue).c_str());
        settingsHandler.storeValue("dualComs", key,
                                   convertComPort(keyValue).c_str());

        qDebug() << " SET INDEX " << setList.at(i)->currentIndex();
        if (!(setKeyValue == "No outputs")) {
          int index = setList[i]->currentIndex() - 1;

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
          settingsHandler.storeValue("runningDualSets", setKey, id);
          settingsHandler.storeValue("dualSets", setKey, id);
        }
      }
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
  return true;
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
  QString comGroupName;
  QString setGroupName;
  bool setsNeeded = true;

  switch (mode) {
    case 1:
      widget = ui->inWidgetContainer;
      comGroupName = "inputComs";
      setsNeeded = false;
      break;
    case 2:
      widget = ui->outWidgetContainer;
      setGroupName = "outputSets";
      comGroupName = "outputComs";
      break;
    case 3:
      widget = ui->dualWidgetContainer;
      setGroupName = "dualSets";
      comGroupName = "dualComs";
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
    comList[i]->clear();
    for (auto &com : coms) {
      comList[i]->addItem(com);
    }
  }
  restoreStoredValuesComboBoxes(widget, comGroupName, setGroupName, setsNeeded);
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
  QProcess *process = new QProcess(this);
  process->startDetached(pathHandler.getMaintenanceToolPath());
  process->waitForFinished();
  exitProgram();
}

int MainWindow::getComboxIndex(QComboBox *comboBox, QString value) {
  int index = -10;
  qDebug() << value;
  if (!value.isNull()) {
    for (int i = 0; i < comboBox->count(); i++) {
      QString text = comboBox->itemText(i);
      if (text.contains(value)) {
        index = i;
      }
    }
  }
  qDebug() << "INDEX " << index;
  return index;
}
void MainWindow::closeEvent(QCloseEvent *event) {
  if (settingsHandler.retrieveSetting("Settings", "cbCloseToTray")->toBool()) {
    if (closing) {
      event->accept();
    } else {
      this->hide();
      event->ignore();
    }
  } else {
    exitProgram();
  }
}
MainWindow::~MainWindow() { delete ui; }

void MainWindow::toggleOpen(QSystemTrayIcon::ActivationReason reason) {
  if (reason == QSystemTrayIcon::Trigger) {
    if (isVisible()) {
      hide();
    } else {
      show();
      activateWindow();
    }
  }
}
void MainWindow::exitProgram() { delete this; }
