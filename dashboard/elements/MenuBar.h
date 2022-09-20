//
// Created by dave- on 19-9-2022.
//

#ifndef BITSANDDROIDSCONNECTOR_MENUBAR_H
#define BITSANDDROIDSCONNECTOR_MENUBAR_H


#include <QMainWindow>

class MenuBar {
public:
    MenuBar(QMainWindow *parent);
    populateMenuBar(QMainWindow *parent);
private:
    QMainWindow *parent;

};


#endif //BITSANDDROIDSCONNECTOR_MENUBAR_H
