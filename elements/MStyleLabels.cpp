//
// Created by DaveRiedel on 18-8-2022.
//

#include "MStyleLabels.h"

MStyleLabels::MStyleLabels(QString text, StyleEnum style): QLabel(text) {
    switch (style) {
        case H1_LIGHT:
            this->setStyleSheet("QLabel { color: #fff; font-size: 24px; font-weight: bold; }");
            break;
        case H1:
            this->setStyleSheet("QLabel { color: #1c1c1c; font-size: 24px; font-weight: bold; }");
        case H2:
            this->setStyleSheet("QLabel { color: #1c1c1c; font-size: 18px; font-weight: bold; }");
            break;
        case LABEL:
            this->setStyleSheet("QLabel { font-family: roboto; color: #1c1c1c; font-size: 14px;}");
            break;
        default:
            break;
    }
    QFont roboto("Roboto");
    this->setFont(roboto);

}


