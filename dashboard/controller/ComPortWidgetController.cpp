//
// Created by DaveRiedel on 19-9-2022.
//

#include "ComPortWidgetController.h"
#include "workers/ServiceWorker.h"
#include "utils/InputReader.h"
#include <QVBoxLayout>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QPushButton>

void ComPortWidgetController::removeComPortRow() {
    QWidget * senderWidget = qobject_cast<QWidget *>(sender()->parent());
    QString index = sender()->objectName().right(1);
    try {
        settingsHandler.removeSetting("dualSets", "Set" + index);
        settingsHandler.removeSetting("dualARIndex", index);
        settingsHandler.removeSetting("dualComs", "com" + index);
       // adjustIndexes(index.toInt());
    } catch (std::exception &e) {
        qDebug("%s", e.what());
    }
    delete senderWidget;
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

ComPortWidgetController::ComPortWidgetController(QWidget *parent) {
    this->parent = parent;
    //TODO connect loggers
    //QObject::connect(&dualWorker, &DualWorker::logMessage, &parent->getServiceWorker(), &ServiceWorker::logMessage);
    InputReader inputReader = InputReader();
    inputReader.readInputs();
    dualWorker.setInputs(inputReader.getInputs());
}

void ComPortWidgetController::start() {
    setHandler.updateSets();
    auto *startButton =
            parent->findChild<QPushButton *>("startButton");
    auto *stopButton =
            parent->findChild<QPushButton *>("stopButton");

    settingsHandler.clearKeys("runningDualComs");
    settingsHandler.clearKeys("runningDualSets");

    QRegularExpression search("comBox");
    QList<ModeIndexCombobox *> comList = parent->findChildren<ModeIndexCombobox *>(search);

    QRegularExpression searchSets("setBox");
    QList<ModeIndexCombobox *> setList = parent->findChildren<ModeIndexCombobox *>(searchSets);

    bool emptyDualSet = checkIfComboIsEmpty(setList);
    bool emptyDualCom = checkIfComboIsEmpty(comList);
    if (!emptyDualSet && !emptyDualCom) {
        QString key;
        QString setKey;
        QList<Output *> outputsToMap;
        startButton->setChecked(true);
        startButton->setText("Start");
        startButton->setEnabled(false);
        qDebug() << "sets available" << comList.size();
        dualWorker.clearBundles();

        for (int i = 0; i < comList.size(); i++) {
            if (!(comList[i]->currentText().contains("Not connected"))) {
                key = "com" + QString::number(i);
                setKey = "Set" + QString::number(i);

                QString keyValue = comList[i]->currentText();
                QString setKeyValue = setList[i]->currentText();
                qDebug() << setKeyValue;

                settingsHandler.storeValue("runningDualComs", key,
                                           convertComPort(keyValue).c_str());
                settingsHandler.storeValue("dualComs", key,
                                           convertComPort(keyValue).c_str());

                qDebug() << " SET INDEX " << setList.at(i)->currentIndex();
                auto availableSets = comPortModel->getAvailableSets();
                if (!(setKeyValue == "No outputs")) {
                    int index = setList[i]->currentIndex() - 1;

                    int id = availableSets->at(index).getID();

                    auto *bundle = new outputBundle();

                    Set active = setHandler.getSetById(QString::number(id));
                    bundle->setSet(active);

                    auto *outputs = new QMap<int, Output *>();
                    *outputs = active.getOutputs();
                    QMap<int, Output *>::iterator j;
                    for (j = outputs->begin(); j != outputs->end(); j++) {
                        outputsToMap.append(j.value());
                    }
                    dualWorker.setOutputsToMap(outputsToMap);
                    bundle->setOutputsInSet(*outputs);
                    dualWorker.addBundle(bundle);
                    //TODO REMEMBER THIS CHANGED IN WORKER AS WELL
                    settingsHandler.storeValue("runningSets", setKey, id);
                    settingsHandler.storeValue("sets", setKey, id);
                } else {
                    settingsHandler.storeValue("sets", setKey, "na");
                }
            }
        }
        stopButton->setChecked(false);
        dualWorker.abortDual = false;
        dualWorker.start();
    } else {

        startButton->setChecked(false);
        startButton->setText("Start");
        startButton->setEnabled(true);

        stopButton->setChecked(true);
    }
    saveAutoRunStates();

}

void ComPortWidgetController::refresh() {
    comPortModel->refresh();
    refreshComs();
}

void ComPortWidgetController::stop() {
    auto *pressedBtn = qobject_cast<QPushButton *>(sender());
    auto *startBtn = pressedBtn->parent()->findChild<QPushButton *>("startButton");
    startBtn->setEnabled(true);
    startBtn->setChecked(false);
    startBtn->setText("Start");
    pressedBtn->setStyleSheet("background-color:#0F4C5C");
    stopDual();
    emit boardConnectionMade(0);
    emit gameConnectionMade(0);
}

void ComPortWidgetController::addComRow() {

}

void ComPortWidgetController::add() {
    auto *pressedBtn = qobject_cast<QPushButton *>(sender());
    int mode = pressedBtn->objectName().at(0).digitValue();
    auto *layout = new QVBoxLayout();
    //TODO generalize this line
    //layout = ui->dualLayoutContainer;
    auto indexList = getCheckboxesByPattern(QRegularExpression("auto" + QString::number(mode)));
    //layout->addWidget(formbuilder.generateComSelector(true, mode, (int) indexList.size()));
}

void ComPortWidgetController::stopDual() {
    dualWorker.abortDual = true;
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

    for (auto &i: comList) {
        i->clear();
        for (auto &com: *coms) {
            i->addItem(com);
        }
    }
    QRegularExpression searchSets("setBox");

    QList<ModeIndexCombobox *> setList = widget->findChildren<ModeIndexCombobox *>(searchSets);
    QList<Set> *sets = comPortModel->getAvailableSets();

    for (auto &i: setList) {
        i->clear();
        for (auto &key: *sets) {
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
    for (auto &i: toCheck) {
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
                            .retrieveSetting("coms", QString::number(i))
                            ->toString()
                            .mid(4);
            if (getComboxIndex(comComboBox, lastComSaved) != -10) {
                comComboBox->setCurrentIndex(getComboxIndex(comComboBox, lastComSaved));
            } else {
                std::cout<<"HITJE"<<std::endl;
                comComboBox->addItem("Not connected");
                comComboBox->setCurrentIndex(
                        getComboxIndex(comComboBox, "Not connected"));
            }

            auto comboBox =
                    parent->findChild<ModeIndexCombobox *>("setBox" + QString::number(i));
            if (!settingsHandler
                    .retrieveSetting("sets", QString::number(i))
                    ->isNull()) {
                //If the saved setting is not Set to "no" which indicates No Set saved for inputs only in 'dual' mode
                if (settingsHandler
                            .retrieveSetting("sets", QString::number(i))
                            ->toString() != "na") {
                    auto lastSetId =
                            settingsHandler
                                    .retrieveSetting("sets", QString::number(i))
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
        case NOCOMPORT:
            warningLabel->setText("Please select a com port before pressing start");
            break;
        case NOSET:
            warningLabel->setText(
                    "Please create or select a Set before pressing start");
        default:
            break;
    }
    return warningLabel;
}

std::string ComPortWidgetController::convertComPort(QString comText) {
    std::string val =
            R"(\\.\COM)" + comText.toStdString().std::string::substr(3, 2);
    return val;
}

void ComPortWidgetController::saveAutoRunStates() {
    auto autoList = getCheckboxesByPattern(QRegularExpression("auto"));
    QString group;
    for (auto &cb: autoList) {
        QString index = QString::number(cb->getIndex());
        settingsHandler.storeValue("dualARIndex", index, cb->isChecked());
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