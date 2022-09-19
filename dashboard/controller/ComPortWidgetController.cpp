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

ComPortWidgetController::ComPortWidgetController(Modes mode) {
    this->mode = mode;
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

void ComPortWidgetController::refreshComs(int mode) {
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

void ComPortWidgetController::GameConnectionMade(int con) {
    qDebug() << "ConnectionReceived";
    auto gameRadioButton = new QRadioButton();
    gameRadioButton = ui->dualWidgetContainer->findChild<QRadioButton *>(
            "dualWidgetContainerGameCon");

    if (con == 0) {
        gameRadioButton->setStyleSheet(
                "QRadioButton::indicator{border: 1px solid darkgray; background-color: "
                "red; border-radius: 7px; height: 12px; width: 12px;}");
    }
    if (con == 1) {
        gameRadioButton->setStyleSheet(
                "QRadioButton::indicator{border: 1px solid darkgray; background-color: "
                "orange; border-radius: 7px;height: 12px; width: 12px;}");
    }
    if (con == 2) {
        gameRadioButton->setStyleSheet(
                "QRadioButton::indicator{border: 1px solid darkgray; background-color: "
                "green; border-radius: 7px;height: 12px; width: 12px;}");
    }
}

void ComPortWidgetController::BoardConnectionMade(int con) {
    auto boardRadioButton = new QRadioButton();

    boardRadioButton = ui->dualWidgetContainer->findChild<QRadioButton *>(
            "dualWidgetContainerBoardCon");

    if (con == 0) {
        boardRadioButton->setStyleSheet(
                "QRadioButton::indicator{border: 1px solid darkgray; background-color: "
                "red; border-radius: 7px; height: 12px; width: 12px;}");
    }
    if (con == 1) {
        boardRadioButton->setStyleSheet(
                "QRadioButton::indicator{border: 1px solid darkgray; background-color: "
                "orange; border-radius: 7px;height: 12px; width: 12px;}");
    }
    if (con == 2) {
        boardRadioButton->setStyleSheet(
                "QRadioButton::indicator{border: 1px solid darkgray; background-color: "
                "green; border-radius: 7px;height: 12px; width: 12px;}");
    }
}