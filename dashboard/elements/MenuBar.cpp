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
MenuBar::MenuBar(QMainWindow *parent) {
    populateMenuBar(parent);
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

void MenuBar::openLoggingWindow() {
    auto *wdg = new LogWindow();
    wdg->openWindow();
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
    auto menuBar = new QMenuBar(parent);
    this->parent = parent;
    // TOOLBAR MENU
    auto *openLogging = new QAction("&Logging", parent);
    auto *openSettings = new QAction("&Settings", parent);
    auto *generateCode = new QAction("&Code", parent);
    auto *libraryGenerator = new QAction("&Generate library", parent);
    auto *calibrateAxis = new QAction("&Calibrate axis", parent);
    auto *openOutputMenu = new QAction("&Outputs", parent);
    auto *openEditEventWindow = new QAction("&Edit events", parent);
    auto *installWasm = new QAction("&Install WASM", parent);
    auto *WasmUpdateEventFile = new QAction("&Update event file", parent);
    auto *updateApplication = new QAction("Check for updates", parent);
    //QMenu *codeMenu = menuBar()->addMenu("&Code");
    QMenu *Settings = parent->menuBar()->addMenu("&Settings");
    QMenu *libraryMenu = parent->menuBar()->addMenu("&Library");
    QMenu *viewMenu = parent->menuBar()->addMenu("&View");
    QMenu *OutputSettings = parent->menuBar()->addMenu("&Outputs");
    QMenu *WasmInstall = parent->menuBar()->addMenu("&WASM");

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
    connect(installWasm, &QAction::triggered, this, &MenuBar::installWasm);
    connect(libraryGenerator, &QAction::triggered, this,
            &MenuBar::openGenerateLibraryMenu);
    connect(generateCode, &QAction::triggered, this,
            &MenuBar::openGenerateCodeMenu);
    //TODO add serviceWorker to class via constructor
    QObject::connect(openLogging, &QAction::triggered, &serviceworker, &ServiceWorker::openLogWindow);
}
