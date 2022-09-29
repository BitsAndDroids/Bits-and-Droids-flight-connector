//
// Created by DaveRiedel on 19-9-2022.
//

#include <QHBoxLayout>
#include <QPushButton>
#include "ComPortButtonRow.h"
#include "widgets/settingsmenu/builder/formbuilder.h"
#include "elements/MPushButton.h"


QWidget *ComPortButtonRow::generateElement() {
    auto *comControls = new QWidget();
    comControls->setObjectName("comControls");

    auto *comControlRow = new QHBoxLayout();
    comControls->setLayout(comControlRow);
    comControlRow->setAlignment(Qt::AlignLeft);

    QFont font;
    font.setPixelSize(13);
    font.setBold(true);

    // START BTN
    auto *startButton = new QPushButton("Start");
    startButton->setObjectName("startButton");
    startButton->setCheckable(true);
    startButton->setStyleSheet(
            "QPushButton#startButton { \
                      color:white;\
                      background-color:#509402;\
                    border-radius:4px;\
                  }   \
                  QPushButton#startButton:checked{\
                      background-color: #0F4C5C;\
                      border: none; \
                  }\
                  QPushButton#startButton:hover{  \
                      background-color: grey; \
                      border-style: outset;\
                  }");

    startButton->setMinimumSize(60, 35);
    startButton->setMaximumSize(60, 35);

    startButton->setFont(font);
    QObject::connect(startButton, &QAbstractButton::clicked, controller,
                     &ComPortWidgetController::start);
    comControlRow->addWidget(startButton);

    // STOP BTN
    auto *stopButton = new QPushButton("Stop");
    stopButton->setObjectName("stopButton");
    stopButton->setStyleSheet(
            "QPushButton#stopBtn { \
                      color:white;\
                      background-color:#0F4C5C;\
                    border-radius:4px;\
                  }   \
                  QPushButton#stopBtn:checked{\
                      background-color: #E20303;\
                      border: none; \
                    border-radius:4px;\
                  }\
                  QPushButton#stopBtn:hover{  \
                      background-color: grey; \
                      border-style: outset;\
                  }");
    stopButton->setMinimumSize(60, 35);
    stopButton->setMaximumSize(60, 35);

    stopButton->setFont(font);
    stopButton->setEnabled(false);
    QObject::connect(stopButton, &QAbstractButton::clicked, controller, &ComPortWidgetController::stop);
    comControlRow->addWidget(stopButton);

    // ADD BTN
    auto *addButton = new MPushButton(comControlRow);
    addButton->setIconWithPath(":/resources/images/add.svg");
    addButton->setObjectName("addBtn");
    addButton->setToolTip("Add new row");
    addButton->setMinimumSize(20, 20);
    addButton->setMaximumSize(20, 20);
    QObject::connect(addButton, &QAbstractButton::clicked, controller, &ComPortWidgetController::add);
    comControlRow->addWidget(addButton);

    // REFRESH BTN
    auto *refreshButton = new MPushButton(comControlRow);

    refreshButton->setIconWithPath(":/resources/images/reload.svg");
    refreshButton->setToolTip("Refresh com ports/sets");
    refreshButton->setObjectName("refreshBtn");
    refreshButton->setMinimumSize(20, 20);
    refreshButton->setMaximumSize(20, 20);

    QObject::connect(refreshButton, &QAbstractButton::clicked, controller,
                     &ComPortWidgetController::refresh);

    comControlRow->addWidget(refreshButton);

    comControlRow->setSpacing(8);
    comControlRow->setContentsMargins(0, 0, 10, 0);
    return comControls;
}

ComPortButtonRow::ComPortButtonRow(ComPortWidgetController *controller, int mode) {
    this->controller = controller;
    this->mode = mode;
}

