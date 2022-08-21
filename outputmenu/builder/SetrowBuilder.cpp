//
// Created by DaveRiedel on 17-8-2022.
//

#include <QLabel>
#include <QPushButton>
#include "SetrowBuilder.h"


QWidget *SetrowBuilder::buildSetrowContainer() {
    return this;
}

void SetrowBuilder::buildSetrow(const Set &setForRow) {
    auto setrow = new SetRow(setForRow);
    connect(setrow, SIGNAL(clickedSignal(QString)), SIGNAL(showSetDetailsSignal(QString)));
    connect(setrow, SIGNAL(editSetSignal(QString)), SIGNAL(editSetSignal(QString)));
    connect(setrow, SIGNAL(deleteSetSignal(QString)), SIGNAL(deleteSetSignal(QString)));
    setRows.push_back(setrow);
    this->layout()->addWidget(setrow);
}

void SetrowBuilder::createSet(QString id) {
    auto setHandler = SetHandler();
    buildSetrow(setHandler.getSetById(id));
}

SetrowBuilder::SetrowBuilder(QWidget *parent) : QWidget(parent) {
    auto setHandler = SetHandler();
    this->setObjectName("setRowBuilder");
    auto *foundSets = setHandler.getSets();

    this->setObjectName("SetListContainer");

    auto setContainerVLayout = new QVBoxLayout();
    this->setLayout(setContainerVLayout);
    setContainerVLayout->addItem(new QSpacerItem(0, 30, QSizePolicy::Fixed, QSizePolicy::Fixed));

    for (auto &i: *foundSets) {
        buildSetrow(i);
    }

    if (foundSets->empty()) {
        auto newSetButton = new QPushButton("Click here to create your first set");
        newSetButton->setObjectName("newSetButton");
        newSetButton->setStyleSheet("QPushButton#newSetButton {"
                                    "background-color: white;"
                                    "margin:10px;"
                                    "padding: 5px;"
                                    "border-radius:5px;"
                                    "font: bold 14px;"
                                    "}");
        setContainerVLayout->addWidget(newSetButton);
        connect(newSetButton, &QPushButton::clicked, this, &SetrowBuilder::createSetSignal);
    }

    setContainerVLayout->setAlignment(Qt::AlignTop);
    this->setStyleSheet(
            "QWidget#setRowBuilder{"
            "border-radius:5px;"
            "background-color:#487f94;"
            "}");
    this->setFixedWidth(375);

}

