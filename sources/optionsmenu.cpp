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
    settings.beginGroup("Ranges");
    settings.beginGroup("Engines");
    if (!settings.value("minEng1").isNull()) {
    }
  }
  uiOptions->vlOptions->setAlignment(Qt::AlignTop);
  auto headerRowProp = new QHBoxLayout();

  auto propHeader = new QLabel();
  propHeader->setText("Propeller range");

  headerRowProp->addWidget(propHeader);

  uiOptions->vlEngineRange->addLayout(headerRowProp);

  auto minMaxLabels = new QHBoxLayout();

  auto minLabel = new QLabel();
  minLabel->setText("Min");

  auto maxLabel = new QLabel();
  maxLabel->setText("Max");

  minMaxLabels->addWidget(minLabel);
  minMaxLabels->addWidget(maxLabel);

  uiOptions->vlEngineRange->addLayout(minMaxLabels);

  auto propLineEditRows = new QHBoxLayout();

  auto leMaxProp = new QLineEdit();
  auto leMinProp = new QLineEdit();

  leMinProp->setObjectName("leMinProp");
  leMinProp->setText("0");
  leMaxProp->setObjectName("leMaxProp");
  leMaxProp->setText("1023");

  propLineEditRows->addWidget(leMinProp);
  propLineEditRows->addWidget(leMaxProp);

  uiOptions->vlEngineRange->addLayout(propLineEditRows);

  auto headerRowMixture = new QHBoxLayout();

  auto mixtureHeader = new QLabel();
  mixtureHeader->setText("Mixture range");

  headerRowMixture->addWidget(mixtureHeader);

  uiOptions->vlEngineRange->addLayout(headerRowMixture);

  uiOptions->vlEngineRange->addLayout(minMaxLabels);

  auto mixtureLineEditRows = new QHBoxLayout();

  auto leMaxMixture = new QLineEdit();
  auto leMinMixture = new QLineEdit();

  leMinMixture->setObjectName("leMinMixture");
  leMinMixture->setText("0");
  leMaxMixture->setObjectName("leMaxMixture");
  leMaxMixture->setText("1023");

  mixtureLineEditRows->addWidget(leMinMixture);
  mixtureLineEditRows->addWidget(leMaxMixture);

  uiOptions->vlEngineRange->addLayout(mixtureLineEditRows);

  for (int i = 0; i < supportedAmntEngines; i++) {
    auto headerRow = new QHBoxLayout();
    auto header = new QLabel();
    header->setText("Engine: " + QString::number(i + 1));
    headerRow->addWidget(header);
    auto labelRow = new QHBoxLayout();

    labelRow->setAlignment(Qt::AlignBottom);

    auto minLabel = new QLabel();
    auto idleLabel = new QLabel();
    auto maxLabel = new QLabel();

    minLabel->setText("Reverse");
    idleLabel->setText("Idle cutoff");
    maxLabel->setText("Max");

    labelRow->addWidget(minLabel);
    labelRow->addWidget(idleLabel);
    labelRow->addWidget(maxLabel);

    auto engineRow = new QHBoxLayout();

    auto minLe = new QLineEdit();
    auto idleLe = new QLineEdit();
    auto maxLe = new QLineEdit();

    QString leMinName = "leMinEng" + QString::number(i);
    QString leIdleName = "leIdleEng" + QString::number(i);
    QString leMaxName = "leMaxEng" + QString::number(i);

    minLe->setObjectName(leMinName);
    idleLe->setObjectName(leIdleName);
    maxLe->setObjectName(leMaxName);

    minLe->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Minimum);
    idleLe->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Minimum);

    maxLe->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Minimum);

    minLe->setText("0");
    idleLe->setText("0");
    maxLe->setText("1023");

    QString minSetting = "minEng" + QString::number(i);
    QString idleSetting = "idleEng" + QString::number(i);
    QString maxSetting = "maxEng" + QString::number(i);

    if (!settings.value(minSetting).isNull()) {
      minLe->setText(settings.value(minSetting).toString());
    }
    if (!settings.value(idleSetting).isNull()) {
      idleLe->setText(settings.value(idleSetting).toString());
    }
    if (!settings.value(maxSetting).isNull()) {
      maxLe->setText(settings.value(maxSetting).toString());
    }

    engineRow->addWidget(minLe);
    engineRow->addWidget(idleLe);
    engineRow->addWidget(maxLe);

    uiOptions->vlEngineRange->setAlignment(Qt::AlignTop);
    uiOptions->vlEngineRange->addLayout(headerRow);
    uiOptions->vlEngineRange->addLayout(labelRow);
    uiOptions->vlEngineRange->addLayout(engineRow);
  }
  if (!settings.value("maxReverseId").isNull()) {
    int value = settings.value("maxReverseId").toInt();
    if (value != -1) {
      uiOptions->buttonGroup->button(value)->click();
    }
  }

  settings.endGroup();

  settings.beginGroup("Mixture");
  if (!settings.value("leMinMixture").isNull()) {
    uiOptions->widgetRanges->findChild<QLineEdit *>("leMinMixture")
        ->setText(settings.value("leMinMixture").toString());
  }
  if (!settings.value("leMaxMixture").isNull()) {
    uiOptions->widgetRanges->findChild<QLineEdit *>("leMaxMixture")
        ->setText(settings.value("leMaxMixture").toString());
  }

  settings.endGroup();

  settings.beginGroup("Props");
  if (!settings.value("leMinProp").isNull()) {
    uiOptions->widgetRanges->findChild<QLineEdit *>("leMinProp")
        ->setText(settings.value("leMinProp").toString());
  }
  if (!settings.value("leMaxProp").isNull()) {
    uiOptions->widgetRanges->findChild<QLineEdit *>("leMaxProp")
        ->setText(settings.value("leMaxProp").toString());
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
  settings.beginGroup("Engines");
  for (int i = 0; i < supportedAmntEngines; i++) {
    QString leMinName = "leMinEng" + QString::number(i);
    QString leIdleName = "leIdleEng" + QString::number(i);
    QString leMaxName = "leMaxEng" + QString::number(i);

    QString minSetting = "minEng" + QString::number(i);
    QString idleSetting = "idleEng" + QString::number(i);
    QString maxSetting = "maxEng" + QString::number(i);

    settings.setValue(
        minSetting,
        uiOptions->widgetRanges->findChild<QLineEdit *>(leMinName)->text());
    settings.setValue(
        idleSetting,
        uiOptions->widgetRanges->findChild<QLineEdit *>(leIdleName)->text());
    settings.setValue(
        maxSetting,
        uiOptions->widgetRanges->findChild<QLineEdit *>(leMaxName)->text());
  }
  int value;
  auto id = uiOptions->buttonGroup->checkedId();
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
  settings.endGroup();

  settings.beginGroup("Mixture");
  settings.setValue(
      "leMinMixture",
      uiOptions->widgetRanges->findChild<QLineEdit *>("leMinMixture")->text());
  settings.setValue(
      "leMaxMixture",
      uiOptions->widgetRanges->findChild<QLineEdit *>("leMaxMixture")->text());
  settings.endGroup();
  settings.beginGroup("Props");
  settings.setValue(
      "leMinProp",
      uiOptions->widgetRanges->findChild<QLineEdit *>("leMinProp")->text());
  settings.setValue(
      "leMaxProp",
      uiOptions->widgetRanges->findChild<QLineEdit *>("leMaxProp")->text());
  settings.endGroup();
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
