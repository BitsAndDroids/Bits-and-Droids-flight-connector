//
// Created by DaveRiedel on 17-8-2022.
//
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include "Setrow.h"
#include "elements/MPushButton.h"

Setrow::Setrow(const Set &setForRow): QWidget() {
    this->setObjectName(QString::number(setForRow.getID()));
    auto rowHLayout = new QHBoxLayout(this);
    rowHLayout->setObjectName("setRowLayout");

    this->setMinimumSize(350,50);
    this->setMaximumSize(350,50);

    rowHLayout->addWidget(new QLabel(setForRow.getSetName()));

    auto editButton = new MPushButton(this->layout());
    editButton->setIconWithPath(":/resources/images/edit.svg");
    editButton->setToolTip("Edit set");
    connect(editButton, &QPushButton::clicked, this, &Setrow::editSet);

    auto exportButton = new MPushButton(this->layout());
    exportButton->setIconWithPath(":/resources/images/export.svg");
    exportButton->setToolTip("Export set");
    connect(exportButton, &QPushButton::clicked, this, &Setrow::exportSet);

    auto deleteButton = new MPushButton(this->layout());
    deleteButton->setIconWithPath(":/resources/images/trashcan.svg");
    deleteButton->setToolTip("Delete set");
    connect(deleteButton, &QPushButton::clicked, this, &Setrow::deleteSet);

    this->setStyleSheet("background-color: white; border-radius:5px; padding: 5px; font-weight:bold;");
}

void Setrow::deleteSet(){
    setHandler.removeSet(this->objectName());
}

void Setrow::editSet() {
    emit editSetSignal(this->objectName());
}

void Setrow::exportSet(){
    setHandler.exportSet(this->objectName());
}
