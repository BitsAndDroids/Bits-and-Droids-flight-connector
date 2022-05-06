//
// Created by Dave Riedel on 01-May-22.
//

#include <QVBoxLayout>
#include <QLabel>
#include <iostream>
#include "CodeGeneratorWindow.h"
#include <QPixmap>
CodeGeneratorWindow::CodeGeneratorWindow(QWidget *parent) : QWidget(parent){
    auto* layout = new QVBoxLayout();
    this->setLayout(layout);
    if (this->objectName().isEmpty())
        this->setObjectName(QString::fromUtf8("CodeGeneratorWindow"));
    this->resize(640, 480);
    this->setWindowTitle("Code generator");

    placeImage();

    auto test = new QLabel("test");
    layout->addWidget(test);
}

void CodeGeneratorWindow::placeImage(){
    auto boardImage = new QLabel();
    QPixmap pm(":/codegenerator/boards/boardimages/proMicroPinout.png");
    boardImage->setPixmap(pm);
    std::cout<<pm.isNull()<<std::endl;
    this->layout()->addWidget(boardImage);
}
