//
// Created by Dave Riedel on 01-May-22.
//

#ifndef BITSANDDROIDSGUI_CODEGENERATORWINDOW_H
#define BITSANDDROIDSGUI_CODEGENERATORWINDOW_H

#include <QApplication>
#include <QWidget>

class CodeGeneratorWindow : public QWidget {
Q_OBJECT
private:


public:
    explicit CodeGeneratorWindow(QWidget *parent = nullptr);

    void placeImage();
};


#endif //BITSANDDROIDSGUI_CODEGENERATORWINDOW_H
