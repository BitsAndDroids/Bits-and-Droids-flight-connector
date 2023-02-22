#ifndef DASHBOARD_H
#define DASHBOARD_H


#include "controller/ComPortWidgetController.h"
#include "dashboard/workers/MFSWorker.h"

#include "outputmenu/handlers/sethandler.h"
#include <qcombobox.h>
#include <qpushbutton.h>
#include <qstandardpaths.h>
#include "widgets/settingsmenu/builder/formbuilder.h"
#include "elements/ModeIndexCombobox.h"
#include "elements/ModeIndexCheckbox.h"
#include <QCoreApplication>
#include <QFile>
#include <QMainWindow>
#include <QNetworkReply>
#include <QSettings>

#include "serial/SerialPort.hpp"
#include "constants.h"
#include "services/ServiceWorker.h"
#include "dashboard/controller/DashboardController.h"
#include "dashboard/Elements/MenuBar.h"
#include "dashboard/controller/ComPortWidgetController.h"

const std::string version = constants::VERSION;

class Dashboard : public QMainWindow {
Q_OBJECT

public:
    Dashboard(QWidget *parent = nullptr);

    ~Dashboard();

public slots:

    void gameConnectionMade(int con);

    void boardConnectionMade(int con);

    void wasmConnectionMade(int con);

    signals:
    void updateEventFile();

private:

    DashboardController controller = DashboardController(this);
    ServiceWorker serviceWorker = ServiceWorker();
    ComPortWidgetController comPortWidgetController = ComPortWidgetController(this, &serviceWorker);

    void setIcon();

    bool closing = false;

    void exitProgram();

    void toggleOpen(QSystemTrayIcon::ActivationReason reason);

    void closeEvent(QCloseEvent *event) override;

    PathHandler pathHandler;


    SetHandler *setHandler = new SetHandler();
    SettingsHandler settingsHandler;

    QString prevInputComInt;
    QList<QString> chopStrInput;

    QString prevRowComInt;
    QList<QString> chopStrRow;

    QString prevOutputComInt;
    QList<QString> chopStrOutput;

    QList<QString> availableComPorts;
    QString m_sSettingsFile;
    std::string url = "https://www.bitsanddroids.com/downloads";
    std::string lastValueRec;

    QSystemTrayIcon *icon = new QSystemTrayIcon(QIcon(":/BitsAndDroidsLogo.ico"), this);

    MFSWorker dualThread;
    FormBuilder formbuilder;

    MenuBar* menuBar;

    void saveAutoRunStates(int mode);

    void loadAutoRunState();

    QList<ModeIndexCheckbox *> getCheckboxesByPattern(const QRegularExpression &pattern);
};

#endif
