//
// Created by dave- on 19-9-2022.
//

#include "MenuBar.h"
#include "constants.h"
#include <QAction>
#include <QMenuBar>
#include "widgets/axismenu/calibrateaxismenu.h"
#include "widgets/settingsmenu/optionsmenu.h"
#include "outputmenu/outputmenu.h"
#include "widgets/eventeditor/eventwindow.h"
#include "widgets/librarygenerator/librarygeneratorwindow.h"
#include "widgets/codegenerator/CodeGeneratorWindow.h"
#include "logging/LogWindow.h"
#include "workers/ServiceWorker.h"

MenuBar::MenuBar(QMainWindow *parent, ServiceWorker *serviceworker): QMenuBar(parent) {
    populateMenuBar(parent);
    this->serviceWorker = serviceworker;
    auto loggerService = serviceWorker->getLoggerService();
    connect(loggerService, &LoggerService::logReceived, logWindow, &LogWindow::addLogRow);
}

void MenuBar::openSettings() {
    if (!optionMenuOpen) {
        optionMenuOpen = true;
        QWidget * wdg = new OptionsMenu;
        QObject::connect(wdg, SIGNAL(closedOptionsMenu), this,
                         SLOT(optionMenuClosed));
        wdg->show();
    }
}

void MenuBar::openOutputMenu() {
    if (!outputMenuOpen) {
        outputMenuOpen = true;
        QWidget * wdg = new OutputMenu;
        connect(wdg, SIGNAL(closedOutputMenu()), this, SLOT(outputMenuClosed()));
        wdg->show();
    }
}

void MenuBar::openCalibrateAxis() {
    if (!calibrateAxisMenuOpen) {
        calibrateAxisMenuOpen = true;
        QWidget * wdg = new CalibrateAxisMenu;
        connect(wdg, SIGNAL(closedCalibrateAxisMenu()), this,
                SLOT(calibrateAxisMenuClosed()));
        wdg->show();
    }
}

void MenuBar::openEditEventMenu() {
    if (!eventwindowOpen) {
        eventwindowOpen = true;
        QWidget * wdg = new EventWindow;
        connect(wdg, SIGNAL(closedEventWindow()), this,
                SLOT(eventWindowClosed()));
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
    std::cout << "hit" << std::endl;
    if (!generateCodeMenuOpen) {
        generateCodeMenuOpen = true;
        QWidget * wdg = new CodeGeneratorWindow;
        wdg->show();
    }
}

void MenuBar::outputMenuClosed() { outputMenuOpen = false; }

void MenuBar::calibrateAxisMenuClosed() { calibrateAxisMenuOpen = false; }

void MenuBar::eventWindowClosed() { eventwindowOpen = false; }

void MenuBar::optionMenuClosed() { optionMenuOpen = false; }

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
}

void MenuBar::installWASM() {
    std::cout << "Installing WASM" << std::endl;
}

void MenuBar::checkForUpdates() {
    std::cout << "Checking for updates" << std::endl;
}

void MenuBar::localUpdateEventFile() {
    std::cout << "Updating event file" << std::endl;
}
