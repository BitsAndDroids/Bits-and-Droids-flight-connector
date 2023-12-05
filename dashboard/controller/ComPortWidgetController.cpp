//
// Created by DaveRiedel on 19-9-2022.
//

#include "ComPortWidgetController.h"
#include <QVBoxLayout>
#include <QSerialPortInfo>
#include <QPushButton>
#include "settings/ComSettingsHandler.h"
#include "dashboard/elements/ComPortRow.h"

ComPortWidgetController::ComPortWidgetController(QWidget *parent, ServiceWorker *serviceWorker) {
  this->parent = parent;
  this->serviceWorker = serviceWorker;

  ComSettingsHandler comSettingsHandler;
  comSettingsHandler.adjustIndexes();

  connect(&dualWorker, &MFSWorker::logMessage, serviceWorker, &ServiceWorker::logMessage);
  connect(&dualWorker, &MFSWorker::boardConnectionMade, this, &ComPortWidgetController::boardConnectionMade);
}

void ComPortWidgetController::initComRows() {
  auto foundComports = settingsHandler.retrieveKeys("comPorts");
  for (int i = 0; i < foundComports->size(); ++i) {
    add();
  }
  restoreStoredValuesComboBoxes();
}

void ComPortWidgetController::removeComPortRow() {
  QWidget *senderWidget = qobject_cast<QWidget *>(sender()->parent());
  QString index = sender()->objectName();
  ComSettingsHandler comSettingsHandler;
  try {
    comSettingsHandler.removeComRowSettings(index);
  } catch (std::exception &e) {
    qDebug("%s", e.what());
  }
  delete senderWidget;
}

void ComPortWidgetController::updatePreset(Preset preset) {
  qDebug() << "ComPortWidgetController::updatePreset()";
}

void ComPortWidgetController::loadComPortData() {
  comPortModel->clearComPortData();
      foreach(
      const QSerialPortInfo &serialPortInfo,
      QSerialPortInfo::availablePorts()) {
      comPortModel->addComPort(serialPortInfo.portName() + " | " +
          serialPortInfo.description());
    }
}

void ComPortWidgetController::start() {
  settingsHandler.clearKeys("runningComPorts");
  settingsHandler.clearKeys("runningComSets");

  QRegularExpression search("comBox");
  QList<ModeIndexCombobox *> comList = parent->findChildren<ModeIndexCombobox *>(search);

  QRegularExpression searchSets("setBox");
  QList<ModeIndexCombobox *> setList = parent->findChildren<ModeIndexCombobox *>(searchSets);

  toggleStartStopButtonState();
  //TODO move storage to model
  QList<Set> *sets = setHandler.getSets();
  QList<QPair<QString, int>> coms = QList<QPair<QString, int>>();
  for (int i = 0; i < comList.size(); i++) {
    int setID = -1;
    if (!setList.empty() && !sets->empty()) {
      if (setList.at(i)->currentIndex() != 0) {
        setID = setList.at(i)->currentIndex() - 1;
      }
    }
    QString com = convertComPort(comList.at(i)->currentText());
    QPair<QString, int> settingsPair = QPair<QString, int>(com,
                                                           (setID == -1) ? -1 : sets->at(setID).getID());
    coms.append(settingsPair);
  }

  ComSettingsHandler comSettingsHandler = ComSettingsHandler();
  comSettingsHandler.saveComs(coms);

  dualWorker.abortDual = false;
  dualWorker.start();

  saveAutoRunStates();
}

void ComPortWidgetController::refresh() {
  comPortModel->refresh();
  refreshComs();
}

void ComPortWidgetController::stop() {
  toggleStartStopButtonState();
  dualWorker.abortDual = true;
}

void ComPortWidgetController::toggleStartStopButtonState() {
  running = !running;
  auto *startBtn = parent->findChild<QPushButton *>("startButton");
  startBtn->setEnabled(!running);
  startBtn->setChecked(!running);
  startBtn->setText("Start");
  auto *stopButton =
      parent->findChild<QPushButton *>("stopButton");
  stopButton->setChecked(running);
  stopButton->setEnabled(running);
}

void ComPortWidgetController::addComRow() {

}

void ComPortWidgetController::add() {
  auto layout = parent->findChild<QVBoxLayout *>("comRowsContainer");
  auto comportRow = new ComPortRow(this, rows);
  layout->addWidget(comportRow->generateElement());
  rows++;
}

int ComPortWidgetController::getComboxIndex(ModeIndexCombobox *comboBox, const QString &value) {
  int index = -10;
  if (!value.isNull()) {
    for (int i = 0; i < comboBox->count(); i++) {
      QString text = comboBox->itemText(i);
      if (text.contains(value)) {
        index = i;
      }
    }
  }
  return index;
}

void ComPortWidgetController::refreshComs() {
  auto *widget = new QWidget();

  QRegularExpression search("comBox");
  QList<ModeIndexCombobox *> comList = parent->findChildren<ModeIndexCombobox *>(search);

  QList<QString> *coms = comPortModel->getAvailableComPorts();

  for (auto &i : comList) {
    i->clear();
    for (auto &com : *coms) {
      i->addItem(com);
    }
  }

  QRegularExpression searchSets("setBox");

  QList<ModeIndexCombobox *> setList = widget->findChildren<ModeIndexCombobox *>(searchSets);
  //TODO move storage to model
  QList<Set> *sets = setHandler.getSets();

  for (auto &i : setList) {
    i->clear();
    for (auto &key : *sets) {
      i->addItem(key.getSetName());
    }
  }

  restoreStoredValuesComboBoxes();
}

void ComPortWidgetController::clearChildrenFromLayout(QLayout *toClear) {
  if (toClear != nullptr) {
    QLayoutItem *item;
    while ((item = toClear->takeAt(0)) != nullptr) {
      delete item->widget();
      delete item;
    }
  }
}

bool ComPortWidgetController::checkIfComboIsEmpty(const QList<ModeIndexCombobox *> &toCheck) {
  for (auto &i : toCheck) {
    if (i->currentIndex() == -1) {
      return true;
    } else
      return false;
  }
  return true;
}

void ComPortWidgetController::restoreStoredValuesComboBoxes() {
  auto comboBoxes = parent->findChildren<ModeIndexCombobox *>();
  int itterations = (int) comboBoxes.size();
  if (itterations > 1) {
    itterations = itterations / 2;
  }

  for (int i = 0; i < itterations; i++) {
    auto comComboBox =
        parent->findChild<ModeIndexCombobox *>("comBox" + QString::number(i));
    if (comComboBox != nullptr) {
      auto lastComSaved =
          settingsHandler
              .getSettingValue("comPorts", QString::number(i))
              ->toString()
              .mid(4);
      if (getComboxIndex(comComboBox, lastComSaved) != -10) {
        comComboBox->setCurrentIndex(getComboxIndex(comComboBox, lastComSaved));
      } else {
        comComboBox->addItem("Not connected");
        comComboBox->setCurrentIndex(
            getComboxIndex(comComboBox, "Not connected"));
      }

      auto comboBox =
          parent->findChild<ModeIndexCombobox *>("setBox" + QString::number(i));
      if (!settingsHandler
          .getSettingValue("comSets", QString::number(i))
          ->isNull()) {
        //If the saved setting is not Set to "no" which indicates No Set saved for inputs only in 'dual' mode
        if (settingsHandler
            .getSettingValue("comSets", QString::number(i))
            ->toString() != "-1") {
          auto lastSetId =
              settingsHandler
                  .getSettingValue("comSets", QString::number(i))
                  ->toString();
          auto setFound = setHandler.getSetById(lastSetId);
          auto setName = setFound.getSetName();

          comboBox->setCurrentIndex(getComboxIndex(comboBox, setName));
        } else {
          comboBox->setCurrentIndex(0);
        }
      }
    }
  }
}

QLabel *ComPortWidgetController::returnWarningString(int warningType) {
  auto warningLabel = new QLabel();
  warningLabel->setStyleSheet("color:#B33A3A");
  switch (warningType) {
    case NOCOMPORT:warningLabel->setText("Please select a com port before pressing start");
      break;
    case NOSET:
      warningLabel->setText(
          "Please create or select a Set before pressing start");
    default:break;
  }
  return warningLabel;
}

QString ComPortWidgetController::convertComPort(const QString &comText) {
    int index = comText.indexOf(" ");
  QString val =
      R"(\\.\COM)" + comText.mid(3, index - 3);
  return val;
}

void ComPortWidgetController::saveAutoRunStates() {
  auto autoList = getCheckboxesByPattern(QRegularExpression("auto"));
  for (auto &cb : autoList) {
    QString index = QString::number(cb->getIndex());
    settingsHandler.storeSettingValue("dualARIndex", index, cb->isChecked());
  }
}

//TODO CHECK IF THIS NEEDS TO BE MOVED
//void ComPortWidgetController::loadAutoRunState() {
//    QRegularExpression searchAuto("auto");
//    auto autoList = this->findChildren<ModeIndexCheckbox *>(searchAuto);
//    QString group;
//    for (auto &cb: autoList) {
//        int mode = cb->getMode();
//        QString index = QString::number(cb->getIndex());
//        cb->setChecked(settingsHandler.retrieveSetting("dualARIndex", index)->toBool());
//    }
//}

QList<ModeIndexCheckbox *> ComPortWidgetController::getCheckboxesByPattern(const QRegularExpression &pattern) {
  return this->findChildren<ModeIndexCheckbox *>(pattern);
}