//
// Created by DaveRiedel on 19-9-2022.
//

#include "ComPortWidgetController.h"
#include <QRadioButton>
#include <QSerialPort>
#include <QPushButton>

void ComPortWidgetController::removeComPortRow() {
    QWidget * senderWidget = qobject_cast<QWidget *>(sender()->parent());
    QString index = sender()->objectName().right(1);
    try {
        settingsHandler.removeSetting("dualSets", "Set" + index);
        settingsHandler.removeSetting("dualARIndex", index);
        settingsHandler.removeSetting("dualComs", "com" + index);
        adjustIndexes(index.toInt());
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
            comPortModel.addComPort(serialPortInfo.portName() + " | " +
                                    serialPortInfo.description())
        }
}

ComPortWidgetController::ComPortWidgetController(QWidget *parent) {
    this->parent = parent;
}

void ComPortWidgetController::start() {
    auto *pressedBtn = qobject_cast<QPushButton *>(sender());
    pressedBtn->setEnabled(false);
    pressedBtn->setText("Running");
    auto *stopButton = pressedBtn->parent()->findChild<QPushButton *>("stopBtn");
    stopButton->setStyleSheet("background-color:#E20303");
    dualWorker.start();
}

void ComPortWidgetController::refresh() {
    auto *pressedBtn = qobject_cast<QPushButton *>(sender());
    comPortWidgetModel.
}

void ComPortWidgetController::stop() {
    auto *pressedBtn = qobject_cast<QPushButton *>(sender());
    int mode = pressedBtn->objectName().at(0).digitValue();
    auto *startBtn = pressedBtn->parent()->findChild<QPushButton *>("startButton");
    startBtn->setEnabled(true);
    startBtn->setChecked(false);
    startBtn->setText("Start");
    pressedBtn->setStyleSheet("background-color:#0F4C5C");
    stopDual();
    BoardConnectionMade(0);
    GameConnectionMade(0);
}

void ComPortWidgetController::add() {
    auto *pressedBtn = qobject_cast<QPushButton *>(sender());
    int mode = pressedBtn->objectName().at(0).digitValue();
    auto *layout = new QVBoxLayout();
    //TODO generalize this line
    layout = ui->dualLayoutContainer;
    auto indexList = getCheckboxesByPattern(QRegularExpression("auto" + QString::number(mode)));
    layout->addWidget(formbuilder.generateComSelector(true, mode, (int) indexList.size()));
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
    QString comGroupName;
    QString setGroupName;
    bool setsNeeded = true;

    widget = ui->dualWidgetContainer;
    setGroupName = "dualSets";
    comGroupName = "dualComs";


    QRegularExpression search("comBox");
    QList<ModeIndexCombobox *> comList = widget->findChildren<ModeIndexCombobox *>(search);
    formbuilder.loadComPortData();

    QList<QString> coms = formbuilder.getAvailableComPorts();

    for (auto &i: comList) {
        i->clear();
        for (auto &com: coms) {
            i->addItem(com);
        }
    }
    QRegularExpression searchSets("setBox");
    restoreStoredValuesComboBoxes(widget, comGroupName, setGroupName, setsNeeded);
    QList<ModeIndexCombobox *> setList = widget->findChildren<ModeIndexCombobox *>(searchSets);
    QList<Set> *sets = setHandler->loadSets();

    for (auto &i: setList) {
        i->clear();
        for (auto &key: *sets) {
            i->addItem(key.getSetName());
        }
    }

    restoreStoredValuesComboBoxes(widget, comGroupName, setGroupName, setsNeeded);

}



void ComPortWidgetController::startDual(bool autoStart) {
    setHandler->updateSets();
    auto *startButton =
            parent->findChild<QPushButton *>("startButton");
    auto *stopButton =
            parent->findChild<QPushButton *>("stopBtn");

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
        dualThread.clearBundles();

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
                if (!(setKeyValue == "No outputs")) {
                    int index = setList[i]->currentIndex() - 1;

                    int id = availableSets->at(index).getID();

                    auto *bundle = new outputBundle();

                    Set active = setHandler->getSetById(QString::number(id));
                    bundle->setSet(active);

                    auto *outputs = new QMap<int, Output *>();
                    *outputs = active.getOutputs();
                    QMap<int, Output *>::iterator j;
                    for (j = outputs->begin(); j != outputs->end(); j++) {
                        outputsToMap.append(j.value());
                    }
                    dualThread.setOutputsToMap(outputsToMap);
                    bundle->setOutputsInSet(*outputs);
                    dualThread.addBundle(bundle);
                    settingsHandler.storeValue("runningDualSets", setKey, id);
                    settingsHandler.storeValue("dualSets", setKey, id);
                } else {
                    settingsHandler.storeValue("dualSets", setKey, "na");
                }
            }
        }
        stopButton->setChecked(false);
        stopButton->setStyleSheet("background-color:#E20303");

        //    ui->stopButton->setVisible(true);
        //    QString comText = ui->outputComboBoxBase->currentText();
        // std::string comNr
        //    R"(\\.\COM)" + comText.toStdString().std::string::substr(3, 2);

        dualThread.abortDual = false;
        dualThread.start();
    } else {

        startButton->setChecked(false);
        startButton->setText("Start");
        startButton->setEnabled(true);


        stopButton->setChecked(true);
        stopButton->setStyleSheet("background-color:#0F4C5C");

        auto dualWarningBox =
                ui->dualWidgetContainer->findChild<QLayout *>("dualWarningBox");
        clearChildrenFromLayout(dualWarningBox);
        if (emptyDualCom) {
            dualWarningBox->addWidget(returnWarningString(NOCOMPORT));
        }
        if (emptyDualSet) {
            dualWarningBox->addWidget(returnWarningString(NOSET));
        }
    }
    saveAutoRunStates(DUALMODE);

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