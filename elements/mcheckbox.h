#ifndef MCHECKBOX_H
#define MCHECKBOX_H

#include <qcheckbox.h>

#include <QObject>

class mCheckBox {
 public:
  mCheckBox();
  mCheckBox(QString cbLabelText, QString objectName);
  mCheckBox(QString cbLabelTextSet, QString objectNameSet, bool setDefaultState);
  QCheckBox *generateCheckbox();

 private:
  QString cbLabelText;
  QString objectName;
  bool defaultState;
};

#endif  // MCHECKBOX_H
