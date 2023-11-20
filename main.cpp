#include <QApplication>
#include <QDebug>
#include "dashboard/Dashboard.h"
#include "settings/settingshandler.h"
#include "rungaurd/RunGuard.h"
#include "logging/Logger.h"


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
    for (int i = 1; i < argc; i++) {
        if(argv[i] == nullptr) {
            qDebug()<< "argv[" << i << "] = nullptr";
        } else {
            qDebug() << "argv[" << i << "] = " << argv[i];
            if(strcmp(argv[i], "-src=MFS2020") == 0){
                launchedFromMFS2020 = true;
            }
        }
    }

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
