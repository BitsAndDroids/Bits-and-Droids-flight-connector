#ifndef DASHBOARD_H
#define DASHBOARD_H


#include "controller/ComPortWidgetController.h"
#include "workers/InputWorker.h"
#include "workers/dualworker.h"
#include "workers/outputworker.h"

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
#include "workers/serviceWorker.h"
#include "dashboard/controller/DashboardController.h"
#include "dashboard/Elements/MenuBar.h"
#include "dashboard/controller/ComPortWidgetController.h"

const std::string version = constants::VERSION;

class Dashboard : public QMainWindow {
Q_OBJECT

public:
    Dashboard(QWidget *parent = nullptr);

    ~Dashboard();


    void on_btnSwitchNav1_clicked();

    static int getComboxIndex(ModeIndexCombobox *comboBox, const QString &value);

public slots:

    void gameConnectionMade(int con);

    void boardConnectionMade(int con);

private:

    DashboardController controller = DashboardController(this);
    ServiceWorker serviceWorker = ServiceWorker();
    ComPortWidgetController comPortWidgetController = ComPortWidgetController(this, &serviceWorker);

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
    std::string lastValueRec = "";

    void loadSettings();


    DualWorker dualThread;
    OutputWorker outputThread;
    InputWorker inputThread;
    FormBuilder formbuilder;

    MenuBar* menuBar;

    void loadComPortData();


    void copyFolder(const QString &sourceFolder, const QString &destinationFolder);

    bool updateApplication();

    void unzip(QString zipfilename, QString filename);

    void checkForUpdates(bool silentCheck);

    void restoreStoredValuesComboBoxes(QWidget *widget, const QString &comGroupName,
                                       const QString &setGroupName, bool setsNeeded);


    void saveAutoRunStates(int mode);

    void loadAutoRunState();

    QList<ModeIndexCheckbox *> getCheckboxesByPattern(const QRegularExpression &pattern);
};

#endif
