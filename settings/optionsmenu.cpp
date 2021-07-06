#include "headers/optionsmenu.h"

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
  QString path =
      QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
  QSettings settings(path + "/" + "settings.ini", QSettings::IniFormat);
  settings.beginGroup("Settings");
  QList<QLineEdit *> allLabels =
      uiOptions->formLayoutWidget->findChildren<QLineEdit *>();

  QStringList keys = settings.childKeys();
  if (keys.size() > 0) {
    foreach (const QString &key, settings.childKeys()) {
      if (uiOptions->formLayoutWidget->findChild<QLineEdit *>(key)) {
        uiOptions->formLayoutWidget->findChild<QLineEdit *>(key)->setText(
            settings.value(key).toString());
      }
    }
    if (!settings.value("CBR").isNull()) {
      uiOptions->baudComboBox->setCurrentText(settings.value("CBR").toString());
    }
  }
  settings.endGroup();
  settings.sync();

  // Range handling
  uiOptions->vlOptions->setAlignment(Qt::AlignTop);
  uiOptions->vlEngineRange->addLayout(builder.RangeBuilder());
  uiOptions->vlEngineRange->addWidget(builder.generateOutputTabs());
  // Loadin saved data from the Settings.ini file
  settings.beginGroup("Ranges");

  QList<QLineEdit *> allRangeLabels =
      uiOptions->widgetRanges->findChildren<QLineEdit *>();
  QStringList rangeKeys = settings.childKeys();
  if (rangeKeys.size() > 0) {
    foreach (const QString &key, rangeKeys) {
      if (uiOptions->widgetRanges->findChild<QLineEdit *>(key)) {
        uiOptions->widgetRanges->findChild<QLineEdit *>(key)->setText(
            settings.value(key).toString());
      }
    }
  }
  if (!settings.value("maxReverseId").isNull()) {
    int value = settings.value("maxReverseId").toInt();
    if (value != -1) {
      uiOptions->buttonGroup->button(value)->click();
    }
  }
  settings.endGroup();
}

optionsMenu::~optionsMenu() { delete uiOptions; }

void optionsMenu::on_saveSettingsBtn_clicked() {
  QString path =
      QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
  QSettings settings(path + "/" + "settings.ini", QSettings::IniFormat);
  settings.beginGroup("Settings");
  QList<QLineEdit *> allLabels =
      uiOptions->formLayoutWidget->findChildren<QLineEdit *>();
  qDebug() << "size" << allLabels.size();
  for (int i = 0; i < allLabels.size(); i++) {
    QString name = allLabels.at(i)->objectName();
    qDebug() << "Clicked" << name;
    settings.setValue(name, allLabels.at(i)->text());
  }
  settings.setValue("CBR", uiOptions->baudComboBox->currentText());

  settings.endGroup();
  settings.sync();

  settings.beginGroup("Ranges");

  QList<QLineEdit *> rangeLineEdits =
      uiOptions->widgetRanges->findChildren<QLineEdit *>();

  for (int i = 0; i < rangeLineEdits.size(); i++) {
    settings.setValue(rangeLineEdits[i]->objectName(),
                      rangeLineEdits[i]->text());
  }

  QString idleStr = "Engine " + QString::number(1) + "Min";
  int idleCutoff = settings.value(idleStr).toInt();
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
  settings.setValue("maxReverseRange", value);
  settings.setValue("maxReverseId", id);

  settings.sync();
  settings.endGroup();
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
