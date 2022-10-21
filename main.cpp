#include <QApplication>
#include "dashboard/Dashboard.h"
#include "settings/settingshandler.h"
#include "rungaurd/RunGuard.h"
#include "services/InstallationService.h"

#include <QtCore/QDataStream>


int main(int argc, char *argv[]) {

    RunGuard guard("BitsAndDroidsConnector");
    if (!guard.tryToRun())
        return 0;

    QApplication a(argc, argv);

    QApplication::setApplicationName("BitsAndDroidsFlightConnector");
    QApplication::setOrganizationName("Bits and Droids");
    QApplication::setOrganizationDomain("www.bitsanddroids.com");

    InstallationService installationService = InstallationService();
    installationService.getUpdatesAvailable();

    SettingsHandler settingsHandler = SettingsHandler();
    settingsHandler.migrate();

    Dashboard w;
    w.show();

    return QApplication::exec();
}
