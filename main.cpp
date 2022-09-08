#include <QApplication>

#include "dashboard/Dashboard.h"
#include "rungaurd/RunGuard.h"
#include "workers/ServiceWorker.h"


int main(int argc, char *argv[]) {

    RunGuard guard("BitsAndDroidsConnector");
    if(!guard.tryToRun())
        return 0;

    QApplication a(argc, argv);

    QApplication::setApplicationName("BitsAndDroidsFlightConnector");
    QApplication::setOrganizationName("Bits and Droids");
    QApplication::setOrganizationDomain("www.bitsanddroids.com");

    Dashboard w;
    w.show();

    return QApplication::exec();
}
