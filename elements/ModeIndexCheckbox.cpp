//
// Created by Dave Riedel on 06-May-22.
//

#include "ModeIndexCheckbox.h"

ModeIndexCheckbox::ModeIndexCheckbox(const QString &label, int mode, int index) : QCheckBox(label) {
    this->index = index;
    this->mode = mode;
    this->prefix = label;
    QCheckBox::setText(label);
    QCheckBox::setObjectName(label + QString::number(mode) + QString::number(index));
}

int ModeIndexCheckbox::getMode() const {
    return mode;
}

int ModeIndexCheckbox::getIndex() const {
    return index;
}

const QString &ModeIndexCheckbox::getPrefix() const {
    return prefix;
}
