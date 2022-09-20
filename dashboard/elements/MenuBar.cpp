//
// Created by dave- on 19-9-2022.
//

#include "MenuBar.h"
#include "constants.h"
#include <QAction>
#include <QMenuBar>
MenuBar::MenuBar(QMainWindow *parent) {

}

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
            &Dashboard::localUpdateEventFile);

    connect(calibrateAxis, &QAction::triggered, this,
            &Dashboard::openCalibrateAxis);
    connect(openOutputMenu, &QAction::triggered, this,
            &Dashboard::openOutputMenu);
    connect(updateApplication, &QAction::triggered, this,
            &Dashboard::checkForUpdates);
    connect(installWasm, &QAction::triggered, this, &Dashboard::installWasm);

    connect(&outputThread, &OutputWorker::BoardConnectionMade, this,
            &Dashboard::BoardConnectionMade);
    connect(&outputThread, &OutputWorker::GameConnectionMade, this,
            &Dashboard::GameConnectionMade);
    connect(libraryGenerator, &QAction::triggered, this,
            &Dashboard::openGenerateLibraryMenu);

    connect(generateCode, &QAction::triggered, this,
            &Dashboard::openGenerateCodeMenu);
    QObject::connect(openLogging, &QAction::triggered, &serviceworker, &ServiceWorker::openLogWindow);
}
