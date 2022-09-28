#include "Dashboard.h"

#include <qserialportinfo.h>
#include <QDir>
#include <string>

#include "logging/MessageCaster.h"
#include "dashboard/Elements/MenuBar.h"
#include "dashboard/Elements/ComPortWidget.h"
#include "dashboard/controller/DashboardController.h"

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
    auto serviceWorker = new ServiceWorker();
    //UI ELEMENTS
    auto menuBar = MenuBar(this, serviceWorker);
    auto mainVLayout = new QVBoxLayout;

    this->setLayout(mainVLayout);

    qRegisterMetaType<QList<QString>>("QList<QString>");

    this->setStyleSheet("QMainWindow {background-color: #487f94;}");

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
    connect(dashboardController, &DashboardController::exitProgram, this, &Dashboard::exitProgram);
    connect(&comPortWidgetController, &ComPortWidgetController::gameConnectionMade,this, &Dashboard::gameConnectionMade);
    connect(&comPortWidgetController, &ComPortWidgetController::boardConnectionMade, this, &Dashboard::boardConnectionMade);

    auto updateButton = new QPushButton("Update");
    mainVLayout->addWidget(updateButton);
    connect(updateButton, &QPushButton::clicked, dashboardController, &DashboardController::updateButtonClicked);




    //ComPortWidget
    auto comPortWidget = ComPortWidget(this, &comPortWidgetController);
    mainVLayout->addWidget(comPortWidget.generateElement());


    //CONNECTION ICONS
    auto connectionRow = new QHBoxLayout();
    connectionRow->setAlignment(Qt::AlignLeft);
    auto radioConBoard = new QRadioButton();
    radioConBoard->setObjectName("BoardCon");
    radioConBoard->setChecked(true);
    radioConBoard->setEnabled(false);
    auto radioConGame = new QRadioButton();
    radioConGame->setObjectName("GameCon");
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
    mainVLayout->addLayout(connectionRow);

    //WINDOW SIZING
    this->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    this->setMinimumWidth(375);
    mainVLayout->setSizeConstraint(QLayout::SetMinimumSize);
    this->updateGeometry();
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

void Dashboard::gameConnectionMade(int con) {
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

void Dashboard::boardConnectionMade(int con) {
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