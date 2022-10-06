//
// Created by dave- on 19-9-2022.
//

#include <QFile>
#include <QApplication>
#include <QMessageBox>
#include <QProcess>
#include <QPushButton>
#include "DashboardController.h"

void DashboardController::updateEventFile() {
    try {
        QFile::remove(pathHandler.getCommunityFolderPath() +
                      "/BitsAndDroidsModule/modules/events.txt");
        QFile::copy(applicationEventsPath,
                    pathHandler.getCommunityFolderPath() +
                    "/BitsAndDroidsModule/modules/events.txt");
        connect(this, &DashboardController::sendWASMCommand, serviceWorker,
                &ServiceWorker::sendWASMData);

        emit sendWASMCommand("9999");
        MessageCaster::showCompleteMessage("Successfully updated the event file");
    }
        //This catches the specific error thrown when no output is running
        //Since we send a command over SimConnect to the WASM module we need to have a connection that is alive
    catch (const std::invalid_argument &i) {
        MessageCaster::showWarningMessage("Please start the connector before updating the remote event file");
    }
    catch (const std::exception &e) {
        MessageCaster::showWarningMessage("Something went wrong updating the event file");
    }
}

DashboardController::DashboardController(QMainWindow *parent) {
    this->parent = parent;
    connect(serviceWorker, &ServiceWorker::gameConnectionMade, this, &DashboardController::gameConnectionMade);
    serviceWorker->start();
    //TODO connect logger
    //QObject::connect(&dualWorker, &DualWorker::logMessage, &serviceWorker, &ServiceWorker::logMessage);
    SettingsHandler settingsHandler = SettingsHandler();
    settingsHandler.checkEventFilePresent();
}


void DashboardController::checkForUpdates(bool silentCheck) {
    auto *process = new QProcess(this);
    process->start(pathHandler.getMaintenanceToolPath() + " ch");
    process->waitForFinished();
    QByteArray data = process->readAll();
    qDebug() << data;
    auto mb = new QMessageBox();
    auto updateButton = this->findChild<QPushButton *>("updateButton");
    updateButton->setVisible(false);
    if (data.contains("no updates available") && !silentCheck) {
        mb->setText("No updates available");
        mb->exec();
    } else if (data.contains("Wait until it finishes")) {
        mb->setText(
                "Another instance of the maintenance tool is already running\n Please "
                "close it before trying again.");
        mb->exec();
    } else if (data.contains("update name")) {
        updateButton->setText("Update available");
        updateButton->setVisible(true);
    }
}

void DashboardController::updateButtonClicked() {
    auto *process = new QProcess(this);
    process->startDetached(pathHandler.getMaintenanceToolPath());
    process->waitForFinished();
    emit exitProgram();
}

QList<ModeIndexCheckbox *> DashboardController::getCheckboxesByPattern(const QRegularExpression &pattern){

}