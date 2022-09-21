//
// Created by DaveRiedel on 21-9-2022.
//

#include "WASMHandler.h"
void WASMHandler::installWasm() {
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