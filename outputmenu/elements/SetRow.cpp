//
// Created by DaveRiedel on 17-8-2022.
//
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <iostream>
#include <QFileDialog>
#include "SetRow.h"
#include "elements/MPushButton.h"

SetRow::SetRow(const Set &setForRow, QWidget * parent): QWidget(parent) {
    this->setObjectName(QString::number(setForRow.getID()));
    auto rowHLayout = new QHBoxLayout(this);
    rowHLayout->setObjectName("setRowLayout");

    this->setMinimumSize(350,50);
    this->setMaximumSize(350,50);

    rowHLayout->addWidget(new QLabel(setForRow.getSetName()));

    auto editButton = new MPushButton(this->layout(), ButtonStyleEnum::DEFAULT);
    editButton->setIconWithPath(":/resources/images/edit.svg");
    editButton->setToolTip("Edit set");
    connect(editButton, &QPushButton::clicked, this, &SetRow::editSet);

    auto exportButton = new MPushButton(this->layout(), ButtonStyleEnum::DEFAULT);
    exportButton->setIconWithPath(":/resources/images/export.svg");
    exportButton->setToolTip("Export set");
    connect(exportButton, &QPushButton::clicked, this, &SetRow::exportSet);

    auto deleteButton = new MPushButton(this->layout(), ButtonStyleEnum::DEFAULT);
    deleteButton->setIconWithPath(":/resources/images/trashcan.svg");
    deleteButton->setToolTip("Delete set");
    connect(deleteButton, &QPushButton::clicked, this, &SetRow::deleteSet);

    this->setStyleSheet("background-color: white; border-radius:5px; padding: 5px; font-weight:bold;");
}

void SetRow::deleteSet(){
    setHandler.removeSet(this->objectName());
    emit deleteSetSignal(this->objectName());
}

void SetRow::exportSet(){
    auto filePath = QFileDialog::getSaveFileName(this, "Select folder", "", "*.json");
    setHandler.exportSet(this->objectName(), filePath);
}

void SetRow::editSet(){
    emit clickedSignal(this->objectName());
    emit editSetSignal(this->objectName());
}

void SetRow::mousePressEvent(QMouseEvent *) {
    emit clickedSignal(this->objectName());
}
