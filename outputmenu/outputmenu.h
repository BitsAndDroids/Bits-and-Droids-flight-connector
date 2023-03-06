#ifndef OUTPUTMENU_H
#define OUTPUTMENU_H

#include "handlers/outputhandler.h"
#include "outputmenu/handlers/sethandler.h"

#include <QSettings>
#include <QStandardPaths>
#include <QWidget>

#include "widgets/settingsmenu/builder/formbuilder.h"
#include "settings/settingshandler.h"
#include "outputmenu/builder/OutputDetailsBuilder.h"
#include "outputmenu/builder/OutputTabBuilder.h"
#include "outputmenu/builder/SetrowBuilder.h"
#include "services/ServiceWorker.h"

class OutputMenu : public QWidget {
Q_OBJECT

public:
    explicit OutputMenu(ServiceWorker *pWorker);

    ~OutputMenu();

signals:

    void closedOutputMenu();

    void displaySetDetails(QString id);

    void editSetSignal(QString id);

    void deleteSetSignal(QString id);

    void showSetDetailsSignal(QString id);

private:

    bool open = false;
    int activeSet{};
    SettingsHandler settingsHandler;
    SetHandler setHandler;
    FormBuilder formBuilder;

    OutputDetailsBuilder setDetaisBuilder = OutputDetailsBuilder(this);
    OutputTabBuilder outputTabBuilder = OutputTabBuilder(this);
    SetrowBuilder setrowBuilder = SetrowBuilder(this);

    QString path =
            QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QSettings *settings = new QSettings(path + "/Bits and Droids/settings.ini",
                                        QSettings::IniFormat);

    void closeEvent(QCloseEvent *event) override;

    void addMenuBar();

private slots:
    void deleteSet(const QString& id);

    void showSetDetails(const QString& id);

    void editSet(const QString& id);

    void importSet();
    void createSet();
};

#endif  // OUTPUTMENU_H
