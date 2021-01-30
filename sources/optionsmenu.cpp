#include "headers/optionsmenu.h"

#include <qsettings.h>
#include <qstandardpaths.h>
#include <tchar.h>
#include <windows.h>

#include <QDebug>
#include <string>

#include "stdio.h"
#include "ui_optionsmenu.h"

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
    settings.endGroup();
    settings.sync();
  }
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
