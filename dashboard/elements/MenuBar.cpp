//
// Created by dave- on 19-9-2022.
//

#include "MenuBar.h"
#include "constants.h"
#include <QAction>
#include <QMenuBar>
#include "dashboard/handlers/WASMHandler.h"
#include "outputmenu/outputmenu.h"
#include "widgets/axismenu/calibrateaxismenu.h"
#include "widgets/settingsmenu/optionsmenu.h"
#include "widgets/eventeditor/eventwindow.h"
#include "widgets/librarygenerator/librarygeneratorwindow.h"
#include "widgets/codegenerator/CodeGeneratorWindow.h"
#include "logging/LogWindow.h"
#include "services/ServiceWorker.h"
#include "services/InstallationService.h"

MenuBar::MenuBar(QMainWindow *parent, ServiceWorker *serviceworker): QMenuBar(parent) {
    populateMenuBar(parent);
    this->serviceWorker = serviceworker;
    auto loggerService = serviceWorker->getLoggerService();
    connect(loggerService, &Logger::logReceived, logWindow, &LogWindow::addLogRow);
}

void MenuBar::openSettings() {
    if (!optionMenuOpen) {
        optionMenuOpen = true;
        auto * wdg = new OptionsMenu();
        connect(
                wdg,
                &OptionsMenu::closedOptionsMenu,
                this,
                [=](){this->optionMenuOpen = false;}
                );
        wdg->show();
    }
}

void MenuBar::openOutputMenu() {
    if (!outputMenuOpen) {
        auto * wdg = new OutputMenu(serviceWorker);
        connect(
                wdg,
                &OutputMenu::closedOutputMenu,
                this,
                [=](){this->outputMenuOpen = false;}
                );
        wdg->show();
    }
}

void MenuBar::openCalibrateAxis() {
    if (!calibrateAxisMenuOpen) {
        calibrateAxisMenuOpen = true;
        auto * wdg = new CalibrateAxisMenu();
        connect(
                wdg,
                &CalibrateAxisMenu::closedCalibrateAxisMenu,
                this,
                [=](){this->calibrateAxisMenuOpen = false;}
                );
        wdg->show();
    }
}

void MenuBar::openEditEventMenu() {
    if (!eventwindowOpen) {
        eventwindowOpen = true;
        auto * wdg = new EventWindow();
        connect(
                wdg,
                &EventWindow::closedEventWindow,
                this,
                [=](){this->eventwindowOpen = false;});
        wdg->show();
    }
}

void MenuBar::openGenerateLibraryMenu() {
    if (generateLibraryMenuOpen) {
        generateLibraryMenuOpen = true;
        QWidget * wdg = new LibraryGeneratowWindow;
        wdg->show();
    }
}

void MenuBar::openLogWindow() {
    if(!logWindow->isVisible()){
        logWindow->show();
    }
}

void MenuBar::openGenerateCodeMenu() {
    if (!generateCodeMenuOpen) {
        generateCodeMenuOpen = true;
        QWidget * wdg = new CodeGeneratorWindow;
        wdg->show();
    }
}

void MenuBar::addUpdateAvailable() {
    auto *updateAvailable = new QAction("&Update available", this);
    this->addAction(updateAvailable);
    connect(updateAvailable, &QAction::triggered, this, &MenuBar::updateButtonClicked);
}

void MenuBar::populateMenuBar(QMainWindow *parent) {

    this->parent = parent;
    // TOOLBAR MENU
    auto *openLogging = new QAction("&Logging", parent);
    auto *openSettings = new QAction("&Settings",parent);
    auto *generateCode = new QAction("&Code");
    auto *libraryGenerator = new QAction("&Generate library");
    auto *calibrateAxis = new QAction("&Calibrate axis");
    auto *openOutputMenu = new QAction("&Outputs");
    auto *openEditEventWindow = new QAction("&Edit events");
    auto *installWasm = new QAction("&Install WASM");
    auto *WasmUpdateEventFile = new QAction("&Update event file");
    auto *updateApplication = new QAction("Check for updates");

    //TODO ADD CODE GENERATION MENU
    //QMenu *codeMenu = menuBar()->addMenu("&Code");
    QMenu *Settings = this->addMenu("&Settings");
    //TODO ADD LIBRARY GENERATOR
    //QMenu *libraryMenu = this->addMenu("&Library");
    QMenu *OutputSettings = this->addMenu("&Outputs");
    QMenu *WasmInstall = this->addMenu("&WASM");

    WasmInstall->addAction(WasmUpdateEventFile);
    WasmInstall->addAction(installWasm);
    OutputSettings->addAction(openOutputMenu);
    WasmInstall->addAction(openEditEventWindow);
    Settings->addAction(openSettings);
    Settings->addAction(updateApplication);
    Settings->addAction(openLogging);
    //codeMenu->addAction(generateCode);
    Settings->addAction(calibrateAxis);

    Settings->addAction("Version " + QString(constants::VERSION));
    // libraryMenu->addAction(libraryGenerator);

    connect(WasmUpdateEventFile, &QAction::triggered, this,
            &MenuBar::localUpdateEventFile);
    connect(openEditEventWindow, &QAction::triggered, this,
            &MenuBar::openEditEventMenu);
    connect(calibrateAxis, &QAction::triggered, this,
            &MenuBar::openCalibrateAxis);
    connect(openOutputMenu, &QAction::triggered, this,
            &MenuBar::openOutputMenu);
    connect(updateApplication, &QAction::triggered, this,
            &MenuBar::checkForUpdates);
    connect(openSettings, &QAction::triggered, this, &MenuBar::openSettings);
    connect(installWasm, &QAction::triggered, this, &MenuBar::installWASM);
    connect(libraryGenerator, &QAction::triggered, this,
            &MenuBar::openGenerateLibraryMenu);
    connect(generateCode, &QAction::triggered, this,
            &MenuBar::openGenerateCodeMenu);

    QObject::connect(openLogging, &QAction::triggered, this, &MenuBar::openLogWindow);
    InstallationService installationService = InstallationService();
}

void MenuBar::installWASM() {
    Logger::getInstance()->logDebug("Installing WASM");

    auto wasmHandler = new WASMHandler();
    wasmHandler->installWasm();
}

void MenuBar::checkForUpdates() {
}

void MenuBar::localUpdateEventFile() {
    emit updateEventFile();
}

void MenuBar::updateButtonClicked() {
    auto *process = new QProcess(this);
    PathHandler pathHandler;
    process->startDetached(pathHandler.getMaintenanceToolPath());
    process->waitForFinished();
    //emit exitProgram();
}
