//
// Created by DaveRiedel on 18-8-2022.
//

#include "MPushButton.h"

MPushButton::MPushButton(QString text, QLayout* layout): QPushButton(text) {
    layout->addWidget(this);

}
MPushButton::MPushButton(QLayout* layout): QPushButton() {
    layout->addWidget(this);

}

void MPushButton::setIconWithPath(QString path) {

    this->setIcon(QIcon(path));
    this->setMinimumSize(30,30);
    this->setMaximumSize(30,30);
    this->setStyleSheet(
            "border-image:url(:"+path+"); background-color:#fff;");
    this->show();
}
