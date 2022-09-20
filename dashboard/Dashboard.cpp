#include "Dashboard.h"

#include "widgets/eventeditor/eventwindow.h"
#include "widgets/librarygenerator/librarygeneratorwindow.h"
#include <qserialportinfo.h>
#include <qstandardpaths.h>
#include "widgets/axismenu/calibrateaxismenu.h"
#include "widgets/settingsmenu/optionsmenu.h"
#include "outputmenu/outputmenu.h"

#include <QDir>
#include <iostream>
#include <string>

#include "widgets/codegenerator/CodeGeneratorWindow.h"

#include "utils/InputReader.h"
#include "workers/ServiceWorker.h"
#include "logging/MessageCaster.h"
#include "enums/ModeEnum.h"
#include "elements/ModeIndexCheckbox.h"
#include "elements/ModeIndexCombobox.h"
#include "logging/LogWindow.h"
#include "dashboard/Elements/MenuBar.h"
#include "dashboard/Elements/ComPortWidget.h"
#include "dashboard/controller/DashboardController.h"

void Dashboard::untick() {}

void Dashboard::outputMenuClosed() { outputMenuOpen = false; }

void Dashboard::calibrateAxisMenuClosed() { calibrateAxisMenuOpen = false; }

void Dashboard::eventWindowClosed() { eventwindowOpen = false; }

void Dashboard::optionMenuClosed() { optionMenuOpen = false; }

void Dashboard::openSettings() {
    if (!optionMenuOpen) {
        optionMenuOpen = true;
        QWidget * wdg = new optionsMenu;
        connect(wdg, SIGNAL(closedOptionsMenu()), this,
                SIGNAL(closedOptionsMenu()));
        wdg->show();
    }
}

void Dashboard::openLoggingWindow() {
    auto *wdg = new LogWindow();
    wdg->openWindow();
}

void Dashboard::openOutputMenu() {
    if (!outputMenuOpen) {
        outputMenuOpen = true;
        QWidget * wdg = new OutputMenu;
        connect(wdg, SIGNAL(closedOutputMenu()), this, SIGNAL(closedOutputMenu()));
        wdg->show();
    }
}

void Dashboard::openCalibrateAxis() {
    if (!calibrateAxisMenuOpen) {
        calibrateAxisMenuOpen = true;
        QWidget * wdg = new CalibrateAxisMenu;
        connect(wdg, SIGNAL(closedCalibrateAxisMenu()), this,
                SIGNAL(closedCalibrateAxisMenu()));
        wdg->show();
    }
}

void Dashboard::openEditEventMenu() {
    if (!eventwindowOpen) {
        eventwindowOpen = true;
        QWidget * wdg = new EventWindow;
        connect(wdg, SIGNAL(closedEventWindow()), this,
                SIGNAL(closedEventWindow()));
        wdg->show();
    }
}

void Dashboard::openGenerateLibraryMenu() {
    if (generateLibraryMenuOpen) {
        generateLibraryMenuOpen = true;
        QWidget * wdg = new LibraryGeneratowWindow;
        wdg->show();
    }
}

void Dashboard::openGenerateCodeMenu() {
    std::cout << "hit" << std::endl;
    if (!generateCodeMenuOpen) {
        generateCodeMenuOpen = true;
        QWidget * wdg = new CodeGeneratorWindow;
        wdg->show();
    }
}


std::string Dashboard::convertComPort(QString comText) {
    std::string val =
            R"(\\.\COM)" + comText.toStdString().std::string::substr(3, 2);
    return val;
}

void Dashboard::loadComPortData() {
    availableComPorts.clear();
    availableComPorts.append("");
            foreach (const QSerialPortInfo &serialPortInfo,
                     QSerialPortInfo::availablePorts()) {
            availableComPorts.append(serialPortInfo.portName() + " | " +
                                     serialPortInfo.description());
        }
}

void Dashboard::installWasm() {
    try {
        bool customPathFound = pathHandler.getCommunityFolderPath() != nullptr;
        QString pathfound = "";
        QString sourceString =
                QCoreApplication::applicationDirPath() + "/BitsAndDroidsModule";
        cout << sourceString.toStdString() << endl;
        if (customPathFound) {
            pathfound = pathHandler.getCommunityFolderPath();
        } else {
            auto notFoundMessage = new QMessageBox();
            notFoundMessage->setInformativeText(
                    "Could not find the community folder");
            notFoundMessage->setStandardButtons(QMessageBox::Save |
                                                QMessageBox::Cancel);
            int ret = notFoundMessage->exec();

            if (ret == QMessageBox::Save) {
                QFileDialog dialog(this);
                dialog.setFileMode(QFileDialog::Directory);

                QString communityFolderPath = dialog.getExistingDirectory();
                settingsHandler.storeValue("Settings", "communityFolderPathLabel",
                                           communityFolderPath);
                pathfound = communityFolderPath;
            } else{
                if(pathfound == ""){
                    throw std::logic_error("No path saved");
                }
            }
        }

        QString destinationString = pathfound + "/BitsAndDroidsModule";
        copyFolder(sourceString, destinationString);
        MessageCaster::showCompleteMessage("WASM was sucesfully installed");
    }
    catch (...) {
        cout << "error" << endl;
        MessageCaster::showWarningMessage("Could not install WASM module");
    }
}

void Dashboard::copyFolder(const QString &sourceFolder, const QString &destinationFolder) {
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
    for (auto &dir: dirs) {
        QString sourceName = sourceFolder + "/" + dir;
        QString destinationName = destinationFolder + "/" + dir;
        copyFolder(sourceName, destinationName);
    }
}
Dashboard::Dashboard(QWidget *parent): QMainWindow(parent){
    //UI ELEMENTS
    auto menuBar = MenuBar(this);
    menuBar.populateMenuBar(this);
    auto mainVLayout = new QVBoxLayout;
    this->setLayout(mainVLayout);
    auto comPortWidget = ComPortWidget(this);
    mainVLayout->addWidget(&comPortWidget);

    //TRAY ICON
    auto icon = new QSystemTrayIcon(QIcon(":/BitsAndDroidsLogo.ico"), this);
    connect(icon, &QSystemTrayIcon::activated, this, &Dashboard::toggleOpen);
    auto *quit_action = new QAction("Exit", icon);
    connect(quit_action, &QAction::triggered, this, &Dashboard::exitProgram);
    auto *tray_icon_menu = new QMenu;
    tray_icon_menu->addAction(quit_action);
    icon->setContextMenu(tray_icon_menu);
    icon->show();

    //CONTROLLER
    auto dashboardController = new DashboardController(this);
    connect(this, &Dashboard::gameConnectionMade, comPortWidgetController, &ComPortWidgetController::gameConnectionMade);
    connect(this, &Dashboard::boardConnectionMade, comPortWidgetController, &ComPortWidgetController::boardConnectionMade);
}

Dashboard::Dashboard(QWidget *parent)
        : QMainWindow(parent){
    InputReader inputReader = InputReader();
    inputReader.readInputs();
    dualThread.setInputs(inputReader.getInputs());
    inputThread.setInputs(inputReader.getInputs());


    updateButton = ui->updateButton;

    availableSets = formbuilder.getAvailableSets();
    connect(this, &Dashboard::closedOutputMenu, this,
            &Dashboard::outputMenuClosed);
    connect(this, &Dashboard::closedOptionsMenu, this,
            &Dashboard::optionMenuClosed);
    connect(this, &Dashboard::closedEventWindow, this,
            &Dashboard::eventWindowClosed);
    connect(this, &Dashboard::closedCalibrateAxisMenu, this,
            &Dashboard::calibrateAxisMenuClosed);
    loadComPortData();

    qRegisterMetaType<QList<QString>>("QList<QString>");
    // STYLE AFFECTING SECTION
    //-----------------------
    ui->messagesWidgetLayout->setAlignment(Qt::AlignBottom);







    // SIGNALS + SLOTS


    serviceworker.start();




    connect(openEditEventWindow, &QAction::triggered, this,
            &Dashboard::openEditEventMenu);
    connect(openSettings, &QAction::triggered, this, &Dashboard::openSettings);
    connect(&formbuilder, &FormBuilder::addPressed, this, &Dashboard::addCom);
    connect(&formbuilder, &FormBuilder::stopPressed, this, &Dashboard::stopMode);
    connect(&formbuilder, &FormBuilder::startPressed, this,
            &Dashboard::startMode);
    connect(&formbuilder, &FormBuilder::refreshPressed, this,
            &Dashboard::refreshComs);


    // IMPORTANT IMPROV SECTION

    formbuilder.loadComPortData();

    // INPUTS
    QVBoxLayout *inContainer = ui->inLayoutContainer;

    auto *shadow = new QGraphicsDropShadowEffect();
    QWidget * inWidget = ui->inWidgetContainer;
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
    QWidget * outWidget = ui->outWidgetContainer;
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
    QWidget * dualWidget = ui->dualWidgetContainer;

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

    for (auto &widget: widgetsToSet) {
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
            QWidget * comSelector =
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
    checkForUpdates(true);
    loadAutoRunState();
    settingsHandler.checkEventFilePresent();

    this->adjustSize();
}

void Dashboard::restoreStoredValuesComboBoxes(QWidget *widget,
                                               const QString &comGroupName,
                                               const QString &setGroupName,
                                               bool setsNeeded) {
    auto comboBoxes = widget->findChildren<ModeIndexCombobox *>();
    int itterations = (int) comboBoxes.size();
    if (itterations > 1) {
        itterations = itterations / 2;
    }

    for (int i = 0; i < itterations; i++) {
        auto comComboBox =
                widget->findChild<ModeIndexCombobox *>("comBox" + QString::number(i));
        if (comComboBox != nullptr) {
            auto lastComSaved =
                    settingsHandler
                            .retrieveSetting(comGroupName, "com" + QString::number(i))
                            ->toString()
                            .mid(4);
            if (getComboxIndex(comComboBox, lastComSaved) != -10) {
                comComboBox->setCurrentIndex(getComboxIndex(comComboBox, lastComSaved));
            } else {
                comComboBox->addItem("Not connected");
                comComboBox->setCurrentIndex(
                        getComboxIndex(comComboBox, "Not connected"));
            }

            qDebug() << "LAST COM SAVED " << lastComSaved;

            if (setsNeeded) {
                auto comboBox =
                        widget->findChild<ModeIndexCombobox *>("setBox" + QString::number(i));
                if (!settingsHandler
                        .retrieveSetting(setGroupName, "Set" + QString::number(i))
                        ->isNull()) {
                    //If the saved setting is not Set to "no" which indicates No Set saved for inputs only in 'dual' mode
                    if (settingsHandler
                                .retrieveSetting(setGroupName, "Set" + QString::number(i))
                                ->toString() != "na") {
                        auto lastSetId =
                                settingsHandler
                                        .retrieveSetting(setGroupName, "Set" + QString::number(i))
                                        ->toString();
                        auto setFound = setHandler->getSetById(lastSetId);
                        auto setName = setFound.getSetName();

                        comboBox->setCurrentIndex(getComboxIndex(comboBox, setName));
                    } else {
                        comboBox->setCurrentIndex(0);
                    }
                }
            }
        }
    }
}



void Dashboard::saveAutoRunStates(int mode) {
    auto autoList = getCheckboxesByPattern(QRegularExpression("auto"));
    QString group;
    for (auto &cb: autoList) {
        QString index = QString::number(cb->getIndex());
        settingsHandler.storeValue("dualARIndex", index, cb->isChecked());
    }
}

void Dashboard::loadAutoRunState() {
    QRegularExpression searchAuto("auto");
    auto autoList = this->findChildren<ModeIndexCheckbox *>(searchAuto);
    QString group;
    for (auto &cb: autoList) {
        int mode = cb->getMode();
        QString index = QString::number(cb->getIndex());
        cb->setChecked(settingsHandler.retrieveSetting("dualARIndex", index)->toBool());
    }
}

void Dashboard::clearChildrenFromLayout(QLayout *toClear) {
    if (toClear != nullptr) {
        QLayoutItem *item;
        while ((item = toClear->takeAt(0)) != nullptr) {
            delete item->widget();
            delete item;
        }
    }
}

QLabel *Dashboard::returnWarningString(int warningType) {
    auto warningLabel = new QLabel();
    warningLabel->setStyleSheet("color:#B33A3A");
    switch (warningType) {
        case NOCOMPORT:
            warningLabel->setText("Please select a com port before pressing start");
            break;
        case NOSET:
            warningLabel->setText(
                    "Please create or select a Set before pressing start");
        default:
            break;
    }
    return warningLabel;
}





void Dashboard::on_updateButton_clicked() {
    auto *process = new QProcess(this);
    process->startDetached(pathHandler.getMaintenanceToolPath());
    process->waitForFinished();
    exitProgram();
}



void Dashboard::closeEvent(QCloseEvent *event) {
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



void Dashboard::toggleOpen(QSystemTrayIcon::ActivationReason reason) {
    if (reason == QSystemTrayIcon::Trigger) {
        if (isVisible()) {
            hide();
        } else {
            show();
            activateWindow();
        }
    }
}



void Dashboard::GameConnectionMade(int con) {
    auto gameRadioButton = new QRadioButton();
    gameRadioButton = this->findChild<QRadioButton *>(
            "dualWidgetContainerGameCon");

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

void Dashboard::BoardConnectionMade(int con) {
    auto boardRadioButton = new QRadioButton();

    boardRadioButton = this->findChild<QRadioButton *>(
            "dualWidgetContainerBoardCon");

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

Dashboard::~Dashboard() {
    delete this;
}

void Dashboard::exitProgram() {
    serviceworker.setStopServiceWorker(true);
    serviceworker.wait();
    dualThread.abortDual = true;
    dualThread.wait();
    delete this;
}