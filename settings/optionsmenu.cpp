#include "optionsmenu.h"

#include <qstandardpaths.h>

#include <iostream>
#include <string>

#include "ui_optionsmenu.h"

using namespace std;

void optionsMenu::closeEvent(QCloseEvent *event) {
  qDebug() << 'clEvent';
  delete this;
}

optionsMenu::optionsMenu(QWidget *parent)
    : QWidget(parent), uiOptions(new Ui::optionsMenu) {
  uiOptions->setupUi(this);

  QStringList keys = *settingsHandler.retrieveKeys("Settings");
  if (!keys.empty()) {
    foreach (const QString &key, keys) {
      if (uiOptions->formLayoutWidget->findChild<QLineEdit *>(key)) {
        uiOptions->formLayoutWidget->findChild<QLineEdit *>(key)->setText(
            settingsHandler.retrieveSetting("Settings", key)->toString());
      }
    }
    if (!settingsHandler.retrieveSetting("Settings", "CBR")->isNull()) {
      uiOptions->baudComboBox->setCurrentText(
          settingsHandler.retrieveSetting("Setting", "CBR")->toString());
    }
  }
  auto communityFolderPathLBL = new QLabel("Community folder path");
  auto communityFolderPathLE = new QLineEdit();
  communityFolderPathLE->setObjectName("communityFolderPathLE");
  uiOptions->vlOptions->addWidget(communityFolderPathLBL);
  uiOptions->vlOptions->addWidget(communityFolderPathLE);

  uiOptions->vlOptions->setAlignment(Qt::AlignTop);
  // uiOptions->vlEngineRange->addLayout(builder->createRudderRow());
  uiOptions->vlEngineRange->addLayout(builder->RangeBuilder());
  auto sensLayout = new QVBoxLayout();
  uiOptions->sensitivityWidget->setLayout(sensLayout);
  uiOptions->sensitivityWidget->layout()->setAlignment(Qt::AlignTop);

  if (!settingsHandler
           .retrieveSubSetting("rudderSeries", "sliders", "rudderDeadzone")
           ->isNull()) {
    uiOptions->sensitivityWidget->findChild<QSlider *>("rudderDeadzone")
        ->setValue(
            settingsHandler
                .retrieveSubSetting("rudderSeries", "sliders", "rudderDeadzone")
                ->toInt());
    uiOptions->sensitivityWidget->findChild<QSlider *>("rudderMinSensitivity")
        ->setValue(settingsHandler
                       .retrieveSubSetting("rudderSeries", "sliders",
                                           "rudderMinSensitivity")
                       ->toInt());

    uiOptions->sensitivityWidget->findChild<QSlider *>("rudderPlusSensitivity")
        ->setValue(settingsHandler
                       .retrieveSubSetting("rudderSeries", "sliders",
                                           "rudderPlusSensitivity")
                       ->toInt());
    // Range handling
  }
  uiOptions->sensitivityWidget->adjustSize();
  QStringList rangeKeys = *settingsHandler.retrieveKeys("Ranges");
  if (!rangeKeys.empty()) {
    foreach (const QString &key, rangeKeys) {
      if (uiOptions->widgetRanges->findChild<QLineEdit *>(key)) {
        uiOptions->widgetRanges->findChild<QLineEdit *>(key)->setText(
            settingsHandler.retrieveSetting("Ranges", key)->toString());
      }
    }
  }
  if (!settingsHandler.retrieveSetting("Ranges", "maxReverseId")->isNull()) {
    int value =
        settingsHandler.retrieveSetting("Ranges", "maxReverseId")->toInt();
    if (value != -1) {
      uiOptions->buttonGroup->button(value)->click();
    }
  }
}

optionsMenu::~optionsMenu() {
  emit closedOptionsMenu();
  delete uiOptions;
}

void optionsMenu::on_saveSettingsBtn_clicked() {
  QList<QLineEdit *> allLabels =
      uiOptions->formLayoutWidget->findChildren<QLineEdit *>();
  qDebug() << "size" << allLabels.size();
  for (auto allLabel : allLabels) {
    QString name = allLabel->objectName();
    qDebug() << "Clicked" << name;
    // Only if someone adds a custom path save the location
    if (name == "communityFolderPathLE") {
      QString pathText = allLabel->text();
      if (!pathText.isNull()) {
        settingsHandler.storeValue("Settings", name, pathText);
      }
    } else {
      settingsHandler.storeValue("Settings", name, allLabel->text());
    }
  }
  settingsHandler.storeValue("Settings", "CBR",
                             uiOptions->baudComboBox->currentText());

  QList<QLineEdit *> rangeLineEdits =
      uiOptions->widgetRanges->findChildren<QLineEdit *>();

  for (auto &rangeLineEdit : rangeLineEdits) {
    settingsHandler.storeValue("Ranges", rangeLineEdit->objectName(),
                               rangeLineEdit->text());
  }


  QString idleStr = "Engine " + QString::number(1) + "Min";
  int idleCutoff = settingsHandler.retrieveSetting("Ranges", idleStr)->toInt();
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
  settingsHandler.storeValue("Ranges", "maxReverseRange", value);
  settingsHandler.storeValue("Ranges", "maxReverseId", id);
}

void optionsMenu::on_checkBox_stateChanged(int checked) {
  QList<QLineEdit *> allLabels =
      uiOptions->formLayoutWidget->findChildren<QLineEdit *>();
  if (uiOptions->checkBox->isChecked()) {
    for (auto allLabel : allLabels) {
      allLabel->setEnabled(true);
    }
    uiOptions->baudComboBox->setEnabled(true);

  } else {
    for (auto allLabel : allLabels) {
      allLabel->setEnabled(false);
    }
    uiOptions->baudComboBox->setEnabled(false);
  }
}

void optionsMenu::on_baudComboBox_currentTextChanged(const QString &arg1) {}
