//
// Created by DaveRiedel on 19-9-2022.
//

#include "ComPortWidgetController.h"
#include "utils/InputReader.h"
#include <QVBoxLayout>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QPushButton>
#include "dashboard/elements/ComPortRow.h"

ComPortWidgetController::ComPortWidgetController(QWidget *parent, ServiceWorker *serviceWorker) {
    this->parent = parent;
    this->serviceWorker = serviceWorker;
    connect(&dualWorker, &DualWorker::logMessage, serviceWorker, &ServiceWorker::logMessage);
    connect(&dualWorker, &DualWorker::boardConnectionMade, this, &ComPortWidgetController::boardConnectionMade);

    InputReader inputReader = InputReader();
    inputReader.readInputs();
    dualWorker.setInputs(inputReader.getInputs());
}

void ComPortWidgetController::initComRows(){
    auto foundComports = settingsHandler.retrieveKeys("comPorts");
    for (int i = 0; i < foundComports->size(); ++i) {
        add();
    }
    restoreStoredValuesComboBoxes();
}

void ComPortWidgetController::removeComPortRow() {
    QWidget * senderWidget = qobject_cast<QWidget *>(sender()->parent());
    QString index = sender()->objectName();
    try {
        settingsHandler.removeSetting("comSets", index);
        settingsHandler.removeSetting("autoRunIndex", index);
        settingsHandler.removeSetting("comPorts", index);
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


void ComPortWidgetController::start() {
    settingsHandler.clearKeys("runningComPorts");
    settingsHandler.clearKeys("runningComSets");

    QRegularExpression search("comBox");
    QList<ModeIndexCombobox *> comList = parent->findChildren<ModeIndexCombobox *>(search);

    QRegularExpression searchSets("setBox");
    QList<ModeIndexCombobox *> setList = parent->findChildren<ModeIndexCombobox *>(searchSets);

    QList<Output *> outputsToMap;

    toggleStartStopButtonState();

    dualWorker.clearBundles();
    for (int i = 0; i < comList.size(); i++) {
        if (!(comList[i]->currentText().contains("Not connected"))) {
            QString key = QString::number(i);

            QString keyValue = comList[i]->currentText();
            QString setKeyValue = setList[i]->currentText();

            settingsHandler.storeValue("runningComPorts", key,
                                       convertComPort(keyValue).c_str());
            settingsHandler.storeValue("comPorts", key,
                                       convertComPort(keyValue).c_str());
            //TODO move this action to the comModel
            auto availableSets = setHandler.loadSets();
            std::cout<<"available sets size: "<<availableSets->size()<<std::endl;
            if (!(setKeyValue == "No outputs")) {
                //The 0 index is the "no outputs" entry that is why we have to deduct 1
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
                settingsHandler.storeValue("runningComSets", key, id);
                settingsHandler.storeValue("comSets", key, id);
            } else {
                settingsHandler.storeValue("comSets", key, "na");
            }
        }
    }

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
    auto comportRow = new ComPortRow(this,rows);
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
                            .retrieveSetting("comPorts", QString::number(i))
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
                    .retrieveSetting("comSets", QString::number(i))
                    ->isNull()) {
                //If the saved setting is not Set to "no" which indicates No Set saved for inputs only in 'dual' mode
                if (settingsHandler
                            .retrieveSetting("comSets", QString::number(i))
                            ->toString() != "na") {
                    auto lastSetId =
                            settingsHandler
                                    .retrieveSetting("comSets", QString::number(i))
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