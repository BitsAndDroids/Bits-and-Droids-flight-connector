//
// Created by dave- on 19-9-2022.
//

#ifndef BITSANDDROIDSCONNECTOR_MENUBAR_H
#define BITSANDDROIDSCONNECTOR_MENUBAR_H


#include <QMainWindow>

class MenuBar: public QObject{
    Q_OBJECT
public:
    MenuBar(QMainWindow *parent);

    populateMenuBar(QMainWindow *parent);
private:
    QMainWindow *parent;



    void openSettings();

    void openOutputMenu();

    void openEditEventMenu();

    void openGenerateCodeMenu();

    void openLoggingWindow();

    void openCalibrateAxis();


    void openGenerateLibraryMenu();

    void outputMenuClosed();

    void calibrateAxisMenuClosed();

    void optionMenuClosed();

    void eventWindowClosed();

    bool outputMenuOpen = false;
    bool eventwindowOpen = false;
    bool calibrateAxisMenuOpen = false;
    bool optionMenuOpen = false;
    bool generateLibraryMenuOpen = false;
    bool generateCodeMenuOpen = false;

};


#endif //BITSANDDROIDSCONNECTOR_MENUBAR_H
