//
// Created by Dave Riedel on 06-May-22.
//

#include "ModeIndexCombobox.h"

ModeIndexCombobox::ModeIndexCombobox(QString prefix, int mode, int index): QComboBox() {
    this->prefix = prefix;
    this->mode = mode;
    this->index = index;
    QComboBox::setObjectName(prefix + QString::number(mode) + QString::number(index));
}
ModeIndexCombobox::ModeIndexCombobox(QString prefix, int index): QComboBox() {
    this->prefix = prefix;
    this->index = index;
    QComboBox::setObjectName(prefix  + QString::number(index));
}
