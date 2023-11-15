//
// Created by DaveRiedel on 18-10-2022.
//

#include <QProcess>
#include <QMessageBox>
#include <QCoreApplication>
#include <iostream>
#include <QDir>
#include "InstallationService.h"
#include "handlers/pathhandler.h"
#include "logging/MessageCaster.h"

InstallationService::InstallationService() {

}

void InstallationService::installWASMModule() {

}

void InstallationService::writeToExeXMLMFS2020() {
    PathHandler pathHandler = PathHandler();

    const std::string communityFolderPath = pathHandler.getCommunityFolderPath().toStdString();
    //get path of exe
    std::string exePath = QCoreApplication::applicationDirPath().toStdString() + "";

    if (QDir dir(communityFolderPath.c_str()); dir.exists()) {
        dir.cdUp();
        dir.cdUp();
        QFile file(dir.path() + "/exe.xml");
        QString content = "";
        if (file.exists()){
            file.open(QIODevice::ReadWrite);
            QTextStream stream(&file);
            content = stream.readAll();
        }
        if(content.contains("Bits and Droids connector")){
            return;
        }
        const QString firstPart = content.split("</SimBase.Document>").at(0);

        const std::string autoRunEntryString = " <Launch.Addon>\n"
                                               "     <Name>Bits and Droids connector</Name>\n"
                                               "     <Disabled>False</Disabled>\n"
                                               "     <Path>" + pathHandler.getApplicationExecutablePath().toStdString() +
                                               "</Path>\n"
                                               "     <CommandLine>-src=MFS2020</CommandLine>\n"
                                               "  </Launch.Addon>\n";

        const QString newContent = firstPart + autoRunEntryString.c_str() + "</SimBase.Document>";
        qDebug() << newContent;
        file.resize(0);
        file.write(newContent.toStdString().c_str());
    }

}

bool InstallationService::getUpdatesAvailable() {
    try {
        auto *process = new QProcess();
        PathHandler pathHandler = PathHandler();
        process->start(pathHandler.getMaintenanceToolPath() + " ch");
        process->waitForFinished();
        QByteArray data = process->readAll();
        return data.contains("update name");
    }
    catch (...) {
        return false;
    }
}

QString InstallationService::getCurrentVersion() {
    try {
        auto *process = new QProcess();
        PathHandler pathHandler = PathHandler();
        process->start(pathHandler.getMaintenanceToolPath() + " ch");
        process->waitForFinished();
        QByteArray data = process->readAll();
        return (data);
    }
    catch (...) {
        return "";
    }
}

