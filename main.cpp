#include <QApplication>
#include "dashboard/Dashboard.h"
#include "settings/settingshandler.h"
#include "rungaurd/RunGuard.h"
#include "services/InstallationService.h"
#include "logging/Logger.h"
#include <QtCore/QDataStream>

Logger* Logger::instance = nullptr;

int main(int argc, char *argv[]) {

    RunGuard guard("BitsAndDroidsConnector");
    if (!guard.tryToRun())
        return 0;

    QApplication a(argc, argv);

    QApplication::setApplicationName("BitsAndDroidsFlightConnector");
    QApplication::setOrganizationName("Bits and Droids");
    QApplication::setOrganizationDomain("www.bitsanddroids.com");

    bool launchedFromMFS2020 = false;
    for (int i = 0; i < argc; i++) {
        std::cout << "argv[" << i << "] = " << argv[i] << std::endl;
        if(strcmp(argv[i], "-src=MFS2020") == 0){
            launchedFromMFS2020 = true;
        }
    }

    InstallationService installationService = InstallationService();
    installationService.writeToExeXMLMFS2020();
    SettingsHandler settingsHandler = SettingsHandler();
    settingsHandler.migrate();

    Dashboard w;
    if(launchedFromMFS2020){
        w.initWithAutorun();
    } else{
        w.show();
    }

    return QApplication::exec();
}
