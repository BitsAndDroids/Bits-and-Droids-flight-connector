//
// Created by DaveRiedel on 18-8-2022.
//

#include "MStyleLabels.h"

MStyleLabels::MStyleLabels(QString text, StyleEnum style): QLabel(text) {
    switch (style) {
        case HEADER:
            this->setStyleSheet("QLabel { color: #fff; font-size: 24px; font-weight: bold; }");
            break;
        default:
            break;
    }
}


