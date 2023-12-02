#include "Dashboard.h"
#include <qserialportinfo.h>
#include "dashboard/Elements/MenuBar.h"
#include "dashboard/Elements/ComPortWidget.h"
#include "dashboard/controller/DashboardController.h"
#include "elements/PresetWidget.h"


Dashboard::Dashboard(QWidget* parent): QMainWindow(parent) {
    auto centralWidget = new QWidget(this);
    centralWidget->setStyleSheet("#presetWidget {background-color: #487f94;}");
    this->setCentralWidget(centralWidget);

    centralWidget->setObjectName("centralWidget");

    //UI ELEMENTS
    menuBar = new MenuBar(this, &serviceWorker);

    this->setMenuBar(menuBar);
    auto mainHLayout = new QHBoxLayout();
    auto comportVLayout = new QVBoxLayout();
    comportVLayout->setAlignment(Qt::AlignTop);

    centralWidget->setLayout(mainHLayout);

    qRegisterMetaType<QList<QString>>("QList<QString>");

    centralWidget->setStyleSheet("QWidget#centralWidget {background-color: #487f94;}");


    //CONTROLLER
    auto presetWidget = new PresetWidget(this);
    mainHLayout->addWidget(presetWidget->generateElement(), Qt::AlignTop);

    //ComPortWidget
    auto comPortWidget = ComPortWidget(this, &comPortWidgetController);
    connect(&comPortWidgetController, &ComPortWidgetController::boardConnectionMade, this,
            &Dashboard::boardConnectionMade);
    comportVLayout->addWidget(comPortWidget.generateElement(), Qt::AlignTop);
    comPortWidgetController.initComRows();

    //CONNECTION ICONS
    auto connectionRow = new QHBoxLayout();
    connectionRow->setAlignment(Qt::AlignLeft);

    auto radioConBoard = new QRadioButton();
    radioConBoard->setObjectName("boardCon");
    radioConBoard->setChecked(true);
    radioConBoard->setEnabled(false);
    auto radioConGame = new QRadioButton();
    radioConGame->setObjectName("gameCon");
    radioConGame->setEnabled(false);
    radioConGame->setChecked(true);
    auto wasmCon = new QRadioButton();
    wasmCon->setObjectName("wasmCon");
    wasmCon->setEnabled(false);
    wasmCon->setChecked(true);
    QString labelStyle = "QLabel{color: #fff; font-size: 10px;}";
    auto boardLabel = new QLabel("Boards");
    boardLabel->setStyleSheet(labelStyle);
    auto gameLabel = new QLabel("MFS2020");
    gameLabel->setStyleSheet(labelStyle);
    auto wasmLabel = new QLabel("WASM");
    wasmLabel->setStyleSheet(labelStyle);

    connectionRow->addWidget(radioConBoard);
    connectionRow->addWidget(boardLabel);
    connectionRow->addWidget(radioConGame);
    connectionRow->addWidget(gameLabel);
    connectionRow->addWidget(wasmCon);
    connectionRow->addWidget(wasmLabel);

    comportVLayout->addLayout(connectionRow);
    this->layout()->setAlignment(Qt::AlignTop);

    //After every UI element is added, the controller can be initialized
    connect(&comPortWidgetController, &ComPortWidgetController::boardConnectionMade, this,
            &Dashboard::boardConnectionMade);
    connect(&controller, &DashboardController::exitProgram, this, &Dashboard::exitProgram);
    connect(&controller, &DashboardController::updateAvailable, menuBar, &MenuBar::addUpdateAvailable);
    connect(&controller, &DashboardController::gameConnectionMade, this, &Dashboard::gameConnectionMade);
    connect(&controller, &DashboardController::wasmConnectionMade, this, &Dashboard::wasmConnectionMade);

    controller.setServiceWorker(&serviceWorker);
    controller.initController();
    connect(menuBar, &MenuBar::updateEventFile, &controller, &DashboardController::updateEventFile);

    mainHLayout->addLayout(comportVLayout);
    setIcon();
}

void Dashboard::initWithAutorun() {
    setHandler->updateSets();
    comPortWidgetController.start();
}

void Dashboard::closeEvent(QCloseEvent* event) {
    if (settingsHandler.getSettingValue("Settings", "cbCloseToTray")->toBool()) {
        if (closing) {
            event->accept();
            exitProgram();
        }
        else {
            this->hide();
            event->ignore();
        }
    }
    else {
        exitProgram();
    }
}

void Dashboard::toggleOpen(QSystemTrayIcon::ActivationReason reason) {
    if (reason == QSystemTrayIcon::Trigger) {
        if (isVisible()) {
            hide();
        }
        else {
            show();
            activateWindow();
        }
    }
}

void Dashboard::gameConnectionMade(int con) {
    auto gameRadioButton = this->findChild<QRadioButton *>(
        "gameCon");

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

void Dashboard::boardConnectionMade(int con) {
    auto boardRadioButton = this->findChild<QRadioButton *>(
        "boardCon");

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

void Dashboard::wasmConnectionMade(int con) {
    auto wasmCon = this->findChild<QRadioButton *>(
        "wasmCon");

    if (con == 0) {
        wasmCon->setStyleSheet(
            "QRadioButton::indicator{border: 1px solid darkgray; background-color: "
            "red; border-radius: 7px; height: 12px; width: 12px;}");
    }
    if (con == 1) {
        wasmCon->setStyleSheet(
            "QRadioButton::indicator{border: 1px solid darkgray; background-color: "
            "orange; border-radius: 7px;height: 12px; width: 12px;}");
    }
    if (con == 2) {
        wasmCon->setStyleSheet(
            "QRadioButton::indicator{border: 1px solid darkgray; background-color: "
            "green; border-radius: 7px;height: 12px; width: 12px;}");
    }
}

Dashboard::~Dashboard() = default;

void Dashboard::setIcon() {
    //TODO IMPLEMENT CHECK IF ICON ALREADY EXISTS
    connect(icon, &QSystemTrayIcon::activated, this, &Dashboard::toggleOpen);
    auto* quit_action = new QAction("Exit", icon);
    connect(quit_action, &QAction::triggered, this, &Dashboard::exitProgram);
    auto* tray_icon_menu = new QMenu;
    tray_icon_menu->addAction(quit_action);
    icon->setContextMenu(tray_icon_menu);
    icon->show();
}

void Dashboard::exitProgram() {
    serviceWorker.setStopServiceWorker(true);
    serviceWorker.wait();
    delete icon;
    dualThread.abortDual = true;
    dualThread.wait();
    QCoreApplication::exit();
}
