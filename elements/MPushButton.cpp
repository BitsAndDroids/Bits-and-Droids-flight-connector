//
// Created by DaveRiedel on 18-8-2022.
//

#include "MPushButton.h"

#include <QGraphicsDropShadowEffect>

MPushButton::MPushButton(QString text, QLayout* layout, ButtonStyleEnum style = DEFAULT): QPushButton(text) {
    setStyle(style);
    this->setText(text);
    layout->addWidget(this);
    this->show();

}

MPushButton::MPushButton(QLayout* layout, ButtonStyleEnum style = DEFAULT): QPushButton() {
    setStyle(style);
    layout->addWidget(this);
    this->show();
}

void MPushButton::setStyle(ButtonStyleEnum style) {
    switch (style) {
    case DEFAULT:
        setDefaulStyle();
        break;
    case ANUNCIATOR:
        setAnunciatorStyle();
        break;
    default:
        setDefaulStyle();
        break;
    }
}

void MPushButton::setDefaulStyle() {
    auto* shadow = new QGraphicsDropShadowEffect();
    shadow->setBlurRadius(20);
    shadow->setOffset(2, 2);
    this->setGraphicsEffect(shadow);
    this->show();
}

void MPushButton::setAnunciatorStyle() {
    auto* shadow = new QGraphicsDropShadowEffect();
    shadow->setBlurRadius(10);
    shadow->setOffset(5, 5);
    shadow->setColor(QColor(140,225,107));
    this->setGraphicsEffect(shadow);
    this->show();
}


void MPushButton::setIconWithPath(const QString& path) {
    this->setIcon(QIcon(path));
    this->setMinimumSize(30, 30);
    this->setMaximumSize(30, 30);
    this->setStyleSheet(
        "QPushButton{border-image:url(:" + path + "); background-color:#fff; border-radius:5px; padding: 5px;}");
    this->show();
}
