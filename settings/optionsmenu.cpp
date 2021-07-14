#include "optionsmenu.h"

#include <qsettings.h>
#include <qstandardpaths.h>
#include <settings/formbuilder.h>
#include <tchar.h>

#include <QDebug>
#include <string>

#include "stdio.h"
#include "ui_optionsmenu.h"

FormBuilder builder = FormBuilder();

using namespace std;

optionsMenu::optionsMenu(QWidget *parent)
    : QWidget(parent), uiOptions(new Ui::optionsMenu) {
  uiOptions->setupUi(this);

  QList<QLineEdit *> allLabels =
      uiOptions->formLayoutWidget->findChildren<QLineEdit *>();

  QStringList keys = *settingsHandler.retrieveKeys("Settings");
  if (keys.size() > 0) {
    foreach (const QString &key, keys) {
      if (uiOptions->formLayoutWidget->findChild<QLineEdit *>(key)) {
        uiOptions->formLayoutWidget->findChild<QLineEdit *>(key)->setText(
           settingsHandler.retrieveSetting("Settings", key)->toString());
      }
    }
    if (!settingsHandler.retrieveSetting("Settings","CBR")->isNull()) {
      uiOptions->baudComboBox->setCurrentText(settingsHandler.retrieveSetting("Setting","CBR")->toString());
    }
  }


  // Range handling
  uiOptions->vlOptions->setAlignment(Qt::AlignTop);
  uiOptions->vlEngineRange->addLayout(builder.RangeBuilder());


  QList<QLineEdit *> allRangeLabels =
      uiOptions->widgetRanges->findChildren<QLineEdit *>();
  QStringList rangeKeys = *settingsHandler.retrieveKeys("Ranges");
  if (rangeKeys.size() > 0) {
    foreach (const QString &key, rangeKeys) {
      if (uiOptions->widgetRanges->findChild<QLineEdit *>(key)) {
        uiOptions->widgetRanges->findChild<QLineEdit *>(key)->setText(
            settingsHandler.retrieveSetting("Ranges",key)->toString());
      }
    }
  }
  if (!settingsHandler.retrieveSetting("Ranges","maxReverseId")->isNull()) {
    int value = settingsHandler.retrieveSetting("Ranges","maxReverseId")->toInt();
    if (value != -1) {
      uiOptions->buttonGroup->button(value)->click();
    }
  }

}

optionsMenu::~optionsMenu() { delete uiOptions; }

void optionsMenu::on_saveSettingsBtn_clicked() {

  QList<QLineEdit *> allLabels =
      uiOptions->formLayoutWidget->findChildren<QLineEdit *>();
  qDebug() << "size" << allLabels.size();
  for (int i = 0; i < allLabels.size(); i++) {
    QString name = allLabels.at(i)->objectName();
    qDebug() << "Clicked" << name;
    settingsHandler.storeValue("Settings",name, allLabels.at(i)->text());
  }
  settingsHandler.storeValue("Settings","CBR", uiOptions->baudComboBox->currentText());



  QList<QLineEdit *> rangeLineEdits =
      uiOptions->widgetRanges->findChildren<QLineEdit *>();

  for (int i = 0; i < rangeLineEdits.size(); i++) {
    settingsHandler.storeValue("Ranges",rangeLineEdits[i]->objectName(),
                      rangeLineEdits[i]->text());
  }

  QString idleStr = "Engine " + QString::number(1) + "Min";
  int idleCutoff = settingsHandler.retrieveSetting("Ranges",idleStr)->toInt();
  qDebug() << "cut" << idleCutoff;

  int value;
  auto id = uiOptions->buttonGroup->checkedId();
  qDebug() << "id " << id;
  switch (id) {
    case -2:
      value = -21400;
      break;
    case -3:
      value = -23000;
      break;
    case -4:
      value = -25000;
      break;
    default:
      value = -23000;
      break;
  }
  settingsHandler.storeValue("Ranges","maxReverseRange", value);
  settingsHandler.storeValue("Ranges","maxReverseId", id);
}

void optionsMenu::on_checkBox_stateChanged(int checked) {
  QList<QLineEdit *> allLabels =
      uiOptions->formLayoutWidget->findChildren<QLineEdit *>();
  if (uiOptions->checkBox->isChecked()) {
    for (int i = 0; i < allLabels.size(); i++) {
      allLabels.at(i)->setEnabled(true);
    }
    uiOptions->baudComboBox->setEnabled(true);

  } else {
    for (int i = 0; i < allLabels.size(); i++) {
      allLabels.at(i)->setEnabled(false);
    }
    uiOptions->baudComboBox->setEnabled(false);
  }
}

void optionsMenu::on_baudComboBox_currentTextChanged(const QString &arg1) {}
