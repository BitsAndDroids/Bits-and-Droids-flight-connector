//
// Created by DaveRiedel on 19-9-2022.
//

#include <QHBoxLayout>
#include <QPushButton>
#include "ComPortButtonRow.h"
#include "widgets/settingsmenu/builder/formbuilder.h"
#include "elements/MStyleLabels.h"

QWidget *ComPortButtonRow::generateElement() {
        auto *comControls = new QWidget();
        auto *comControlRow = new QHBoxLayout();
        comControls->setLayout(comControlRow);
        auto *header = new MStyleLabels(headerText, HEADER);
        comControlRow->addWidget(header);
        comControlRow->setAlignment(Qt::AlignLeft);
        // REFRESH BTN
        auto *refreshButton = new QPushButton();

        refreshButton->setIcon(QIcon("resources/images/refreshicon.png"));
        refreshButton->setStyleSheet(
                "border-image:url(:resources/images/refreshicon.png); background-color:#fff;");
        refreshButton->setObjectName(QString::number(mode) + "refreshBtn");

        refreshButton->setMinimumSize(15, 15);
        refreshButton->setMaximumSize(15, 15);

        QObject::connect(refreshButton, &QAbstractButton::clicked, controller,
                &ComPortWidgetController::refresh);

        comControlRow->addWidget(refreshButton);

        // START BTN
        auto *startButton = new QPushButton("Start");
        startButton->setCheckable(true);
        //#2DE3A3
        startButton->setStyleSheet(

                "QPushButton { \
                      color:white;\
                      background-color:#509402;\
                  }   \
                  QPushButton:checked{\
                      background-color: #0F4C5C;\
                      border: none; \
                  }\
                  QPushButton:hover{  \
                      background-color: grey; \
                      border-style: outset;\
                  }");

        startButton->setMinimumSize(50, 25);
        startButton->setMaximumSize(50, 25);
        startButton->setObjectName(QString::number(mode) + "startButton");
        QObject::connect(startButton, &QAbstractButton::clicked, controller,
                &ComPortWidgetController::start);
        comControlRow->addWidget(startButton);

        // STOP BTN
        auto *stopButton = new QPushButton("Stop");
        stopButton->setMinimumSize(50, 25);
        stopButton->setMaximumSize(50, 25);
        stopButton->setObjectName(QString::number(mode) + "stopBtn");
        QObject::connect(stopButton, &QAbstractButton::clicked, controller, &ComPortWidgetController::stop);
        comControlRow->addWidget(stopButton);

        // ADD BTN
        auto *addButton = new QPushButton("+");
        addButton->setMinimumSize(20, 20);
        addButton->setMaximumSize(20, 20);
        addButton->setObjectName(QString::number(mode) + "addBtn");
        QObject::connect(addButton, &QAbstractButton::clicked, controller, &ComPortWidgetController::add);
        comControlRow->addWidget(addButton);

        return comControls;
}

ComPortButtonRow::ComPortButtonRow(ComPortWidgetController *controller,int mode) {
    this->controller = controller;
    this->mode = mode;
}

