//
// Created by dave- on 19-9-2022.
//

#ifndef BITSANDDROIDSCONNECTOR_MENUBAR_H
#define BITSANDDROIDSCONNECTOR_MENUBAR_H


#include <QMainWindow>
#include <QMenuBar>
#include "services/ServiceWorker.h"
#include "outputmenu/outputmenu.h"


class MenuBar : public QMenuBar {
Q_OBJECT
public:
    MenuBar(QMainWindow *parent, ServiceWorker *serviceWorker);

    void populateMenuBar(QMainWindow *parent);

public slots:

    void addUpdateAvailable();

signals:

    void logMessage(const QString &message, const QString &type);

    void updateEventFile();
private:
    QMainWindow *parent;

    void openSettings();

    void openOutputMenu();

    void openEditEventMenu();

    void openGenerateCodeMenu();

    void openLogWindow();


    void openCalibrateAxis();


    void openGenerateLibraryMenu();

    bool outputMenuOpen = false;
    bool eventwindowOpen = false;
    bool calibrateAxisMenuOpen = false;
    bool optionMenuOpen = false;
    bool generateLibraryMenuOpen = false;
    bool generateCodeMenuOpen = false;

    LogWindow *logWindow = new LogWindow();

    ServiceWorker *serviceWorker;

private slots:

    void installWASM();

    void checkForUpdates();

    void localUpdateEventFile();

    void updateButtonClicked();


};


#endif //BITSANDDROIDSCONNECTOR_MENUBAR_H
