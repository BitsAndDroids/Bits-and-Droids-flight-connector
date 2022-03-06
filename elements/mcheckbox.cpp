#include "mcheckbox.h"

#include <qcheckbox.h>

#include <QHBoxLayout>

mCheckBox::mCheckBox() {}

mCheckBox::mCheckBox(QString cbLabelTextSet, QString objectNameSet) {
  this->cbLabelText = cbLabelTextSet;
  this->objectName = objectNameSet;
}

mCheckBox::mCheckBox(QString cbLabelTextSet, QString objectNameSet,
                     bool setDefaultState) {
  this->cbLabelText = cbLabelTextSet;
  this->objectName = objectNameSet;
  this->defaultState = setDefaultState;
}

QCheckBox* mCheckBox::generateCheckbox() {
  auto cb = new QCheckBox();
  cb->setText(cbLabelText);
  cb->setObjectName(objectName);
  if (defaultState) {
    cb->setChecked(defaultState);
  }
  return cb;
}
