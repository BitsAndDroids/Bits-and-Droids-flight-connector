#include "headers/mainwindow.h"

#include <qdesktopservices.h>
#include <qserialportinfo.h>
#include <qstandardpaths.h>
#include <settings/formbuilder.h>
#include <settings/optionsmenu.h>
#include <settings/outputmenu.h>

#include <QGraphicsDropShadowEffect>
#include <QNetworkAccessManager>
#include <iostream>
#include <string>

#include "ui_mainwindow.h"


void MainWindow::untick() {}

void MainWindow::openSettings() {
    QWidget *wdg = new optionsMenu;
    wdg->show();
}

void MainWindow::openOutputMenu() {
    QWidget *wdg = new OutputMenu;
    wdg->show();
}

std::string MainWindow::convertComPort(QString comText) {
    std::string val =
            R"(\\.\COM)" + comText.toStdString().std::string::substr(3, 2);
    return val;
}

void MainWindow::loadComPortData() {
    availableComPorts.clear();
    availableComPorts.append("");
            foreach (const QSerialPortInfo &serialPortInfo,
                     QSerialPortInfo::availablePorts()) {
            availableComPorts.append(serialPortInfo.portName() + " | " +
                                     serialPortInfo.description());
        }
}

MainWindow::MainWindow(QWidget *parent)
        : QMainWindow(parent), ui(new Ui::MainWindow) {
    updateButton = ui->updateButton;
    ui->setupUi(this);
    std::cout << "11111" << std::endl;
    std::cout << inputComRowCounter << "Rows init" << std::endl;

    availableSets = formbuilder.getAvailableSets();

    loadComPortData();


    // This block itterates through the updated com ports list. This is to ensure
    // that the user doesn't have to re-select his previous com port Does this
    // only on bootup to avoid conflicts down the road If no com port found
    // defaults to blank to make clear to the user the previous com port is no
    // longer available

    qRegisterMetaType<QList<QString>>("QList<QString>");
    // STYLE AFFECTING SECTION
    //-----------------------
    ui->messagesWidgetLayout->setAlignment(Qt::AlignBottom);

    // TOOLBAR MENU
    qDebug() << QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    auto *openSettings = new QAction("&Settings", this);
    auto *openOutputMenu = new QAction("&Outputs", this);
    QMenu *Settings = menuBar()->addMenu("&Settings");
    QMenu *OutputSettings = menuBar()->addMenu("&Outputs");
    OutputSettings->addAction(openOutputMenu);
    Settings->addAction(openSettings);

    Settings->addAction("Version " + QString(constants::VERSION));

    // SIGNALS + SLOTS
    connect(openOutputMenu, &QAction::triggered, this,
            &MainWindow::openOutputMenu);
    connect(openSettings, &QAction::triggered, this, &MainWindow::openSettings);
    connect(&formbuilder, &FormBuilder::addPressed, this, &MainWindow::addCom);
    connect(&formbuilder, &FormBuilder::stopPressed, this, &MainWindow::stopMode);
    connect(&formbuilder, &FormBuilder::startPressed, this,
            &MainWindow::startMode);
    connect(&formbuilder, &FormBuilder::refreshPressed, this,
            &MainWindow::refreshComs);
    connect(&inputThread, &InputWorker::updateLastValUI, this,
            &MainWindow::onUpdateLastValUI);
    connect(&inputThread, &InputWorker::updateLastStatusUI, this,
            &MainWindow::onUpdateLastStatusUI);
    connect(&outputThread, SIGNAL(updateLastStatusUI(QString)),
            SLOT(onUpdateLastStatusUI(QString)));

    // IMPORTANT IMPROV SECTION

    formbuilder.loadComPortData();

    // INPUTS
    QVBoxLayout *inContainer = ui->inLayoutContainer;

    auto *shadow = new QGraphicsDropShadowEffect();
    QWidget *inWidget = ui->inWidgetContainer;
    shadow->setBlurRadius(20);
    shadow->setOffset(2, 2);
    inWidget->setGraphicsEffect(shadow);
    inContainer->setAlignment(Qt::AlignTop);


    inContainer->addWidget(formbuilder.generateComControls(1));
    inContainer->parentWidget()->setSizePolicy(QSizePolicy::Expanding,
                                               QSizePolicy::Expanding);


    QStringList *inputKeys = settingsHandler.retrieveKeys("inputComs");
    for (int i = 0; i < inputKeys->size(); i++) {
        QWidget *comSelector = formbuilder.generateComSelector(false, 1);
        inContainer->addWidget(comSelector);

        QString comName =
                settingsHandler.retrieveSetting("inputComs", inputKeys->at(i))
                        ->toString()
                        .mid(7);
        qDebug() << comName;
        auto *comboBox = comSelector->findChild<QComboBox *>();
        int index = getComboxIndex(comboBox, comName);
        qDebug() << index;
        comboBox->setCurrentIndex(index);
    }

    if (inputKeys->empty()) {
        inContainer->addWidget(formbuilder.generateComSelector(false, 1));
    }

    auto inputWarningBox = new QVBoxLayout();
    inputWarningBox->setObjectName("inputWarningBox");
    inContainer->addLayout(inputWarningBox);

    // OUTPUTS
    QVBoxLayout *outContainer = ui->outLayoutContainer;
    QWidget *outWidget = ui->outWidgetContainer;
    auto *shadowOut = new QGraphicsDropShadowEffect();
    shadowOut->setBlurRadius(20);
    shadowOut->setOffset(2, 2);
    outWidget->setGraphicsEffect(shadowOut);
    outContainer->setAlignment(Qt::AlignTop);

    outContainer->addWidget(formbuilder.generateComControls(2));

    outContainer->parentWidget()->setSizePolicy(QSizePolicy::Expanding,
                                                QSizePolicy::Expanding);
    QStringList *outputKeys = settingsHandler.retrieveKeys("outputComs");

    for (int i = 0; i < outputKeys->size(); i++) {
        QWidget *comSelector = formbuilder.generateComSelector(true, 2);
        outContainer->addWidget(comSelector);

        QString comName =
                settingsHandler.retrieveSetting("outputComs", outputKeys->at(i))
                        ->toString()
                        .mid(7);
        qDebug() << comName;
        auto *comboBox = comSelector->findChild<QComboBox *>();
        int index = getComboxIndex(comboBox, comName);
        qDebug() << index;
        comboBox->setCurrentIndex(index);
    }

    if (outputKeys->empty()) {
        outContainer->addWidget(formbuilder.generateComSelector(true, 2));
    }

    auto outputWarningBox = new QVBoxLayout();
    outputWarningBox->setObjectName("outputWarningBox");
    outContainer->addLayout(outputWarningBox);

    // DUAL MODE
    QVBoxLayout *dualContainer = ui->dualLayoutContainer;
    QWidget *dualWidget = ui->dualWidgetContainer;
    auto *shadowDual = new QGraphicsDropShadowEffect();
    shadowDual->setBlurRadius(20);
    shadowDual->setOffset(2, 2);
    dualWidget->setGraphicsEffect(shadowDual);
    dualContainer->setAlignment(Qt::AlignTop);

    dualContainer->addWidget(formbuilder.generateComControls(3));

    dualContainer->parentWidget()->setSizePolicy(QSizePolicy::Expanding,
                                                 QSizePolicy::Expanding);
    QStringList *dualkKeys = settingsHandler.retrieveKeys("dualComs");

    for (int i = 0; i < dualkKeys->size(); i++) {
        QWidget *comSelector = formbuilder.generateComSelector(true, 3);
        dualContainer->addWidget(comSelector);

        QString comName =
                settingsHandler.retrieveSetting("dualComs", dualkKeys->at(i))
                        ->toString()
                        .mid(7);
        qDebug() << comName;
        auto *comboBox = comSelector->findChild<QComboBox *>();
        int index = getComboxIndex(comboBox, comName);
        qDebug() << index;
        comboBox->setCurrentIndex(index);
    }

    if (outputKeys->empty()) {
        dualContainer->addWidget(formbuilder.generateComSelector(true, 3));
    }

    auto dualWarningBox = new QVBoxLayout();
    dualWarningBox->setObjectName("dualWarningBox");
    dualContainer->addLayout(dualWarningBox);

    auto *mgr = new QNetworkAccessManager(this);
    connect(mgr, &QNetworkAccessManager::finished, this, &MainWindow::onfinish);
    connect(mgr, &QNetworkAccessManager::finished, mgr, &QObject::deleteLater);
    mgr->get(
            QNetworkRequest(QUrl("https://www.bitsanddroids.com/assets/"
                                 "downloads/connector/version.html")));

    this->adjustSize();
}

void MainWindow::onfinish(QNetworkReply *rep) {
    try {
        if (rep->error() == QNetworkReply::NoError) {
            QByteArray bts = rep->readAll();

            QString str(bts);

            QStringList versionCut = str.split("~");

            if (versionCut[1].toStdString() != version) {
                ui->updateButton->setEnabled(true);
                std::string nr = versionCut[1].toStdString();
                std::string buttonText = "New update available: " + nr;
                ui->updateButton->setText(QString::fromStdString(buttonText));
                url = versionCut[3].toStdString();
            } else {
                ui->updateButton->setText("Up to date v: " + versionCut[1]);
            }
        }
    } catch (...) {
    }
}

// SLOTS
void MainWindow::onUpdateLastValUI(const QString &lastVal) {
    ui->labelLastVal_2->setText(lastVal);
}

void MainWindow::onUpdateLastStatusUI(const QString &lastVal) {
    ui->labelLastStatus->setText(lastVal);
}

void MainWindow::startMode(int mode) {
    switch (mode) {
        case 1:
            startInputs();
            break;
        case 2:
            startOutputs();
            break;
        case 3:
            startDual();
            break;
        default:
            break;
    }
}

void MainWindow::startInputs() {
    auto *widget = new QWidget();
    inputThread.abortInput = false;
    widget = ui->inWidgetContainer;
    settingsHandler.clearKeys("inputComs");
    QRegularExpression search("comBox");
    QList<QComboBox *> comList = widget->findChildren<QComboBox *>(search);
    bool emptyInputCom = checkIfComboIsEmpty(comList);
    if(!emptyInputCom){
        QString comboBoxName;
        QString key;

        for (int i = 0; i < comList.size(); i++) {
            key = "inputCom" + QString::number(i);
            comboBoxName = "comboBoxRow" + QString::number(i);

            QString keyValue = comList[i]->currentText();
            settingsHandler.storeValue("inputComs", key,
                                       convertComPort(keyValue).c_str());
        }

        inputThread.start();
    } else{
        auto *startButton = ui->inWidgetContainer->findChild<QPushButton *>("2startButton");
        startButton->setChecked(false);
        startButton->setText("Start");
        startButton->setEnabled(true);

        auto *stopButton = ui->inWidgetContainer->findChild<QPushButton *>("2stopBtn");
        stopButton->setChecked(true);
        stopButton->setStyleSheet("background-color:#0F4C5C");

        auto outWarningBox = ui->inWidgetContainer->findChild<QLayout *>("outputWarningBox");
        clearChildrenFromLayout(outWarningBox);

        //always true, check is in here for if I ever add more error messages
        if (emptyInputCom) {
            outWarningBox->addWidget(returnWarningString(NOCOMPORT));
        }
    }
}

void MainWindow::startOutputs() {
    auto *widget = new QWidget();

    widget = ui->outWidgetContainer;

    settingsHandler.clearKeys("outputcoms");
    settingsHandler.clearKeys("outputComSet");

    QRegularExpression search("comBox");
    QList<QComboBox *> comList = widget->findChildren<QComboBox *>(search);

    QRegularExpression searchSets("setBox");
    QList<QComboBox *> setList = widget->findChildren<QComboBox *>(searchSets);
    bool emptyDualSet = checkIfComboIsEmpty(setList);
    bool emptyDualCom = checkIfComboIsEmpty(comList);
    if (!emptyDualSet && !emptyDualCom) {
        QString key;
        QString setKey;
        QList<Output *> outputsToMap;

        for (int i = 0; i < comList.size(); i++) {
            key = "outputCom" + QString::number(i);
            setKey = "outputSet" + QString::number(i);

            QString keyValue = comList[i]->currentText();
            QString setKeyValue = setList[i]->currentText();

            std::cout << keyValue.toStdString() << std::endl;
            settingsHandler.storeValue("outputcoms", key,
                                       convertComPort(keyValue).c_str());

            int index = setList[i]->currentIndex();

            int id = availableSets->at(index).getID();

            auto *bundle = new outputBundle();
            outputThread.clearBundles();
            set active = setHandler->getSetById(QString::number(id));
            bundle->setSet(active);

            auto *outputs = new QMap<int, Output *>();
            *outputs = active.getOutputs();
            QMap<int, Output *>::iterator j;
            for (j = outputs->begin(); j != outputs->end(); j++) {
                outputsToMap.append(j.value());
            }
            outputThread.setOutputsToMap(outputsToMap);

            bundle->setOutputsInSet(*outputs);
            outputThread.addBundle(bundle);
            settingsHandler.storeValue("outputComSet", setKey, id);
        }
        outputThread.abort = false;
        outputThread.start();
    } else {
        auto *startButton = ui->outWidgetContainer->findChild<QPushButton *>("2startButton");
        startButton->setChecked(false);
        startButton->setText("Start");
        startButton->setEnabled(true);

        auto *stopButton = ui->outWidgetContainer->findChild<QPushButton *>("2stopBtn");
        stopButton->setChecked(true);
        stopButton->setStyleSheet("background-color:#0F4C5C");

        auto outWarningBox = ui->outWidgetContainer->findChild<QLayout *>("outputWarningBox");
        clearChildrenFromLayout(outWarningBox);
        if (emptyDualCom) {
            outWarningBox->addWidget(returnWarningString(NOCOMPORT));
        }
        if (emptyDualSet) {
            outWarningBox->addWidget(returnWarningString(NOSET));
        }
    }
}

void MainWindow::startDual() {
    auto *widget = new QWidget();


    widget = ui->dualWidgetContainer;
    settingsHandler.clearKeys("dualComs");
    settingsHandler.clearKeys("dualComSet");

    QRegularExpression search("comBox");
    QList<QComboBox *> comList = widget->findChildren<QComboBox *>(search);

    QRegularExpression searchSets("setBox");
    QList<QComboBox *> setList = widget->findChildren<QComboBox *>(searchSets);

    bool emptyDualSet = checkIfComboIsEmpty(setList);
    bool emptyDualCom = checkIfComboIsEmpty(comList);
    if (!emptyDualSet && !emptyDualCom) {
        QString key;
        QString setKey;
        QList<Output *> outputsToMap;
        qDebug() << "sets available" << comList.size();
        for (int i = 0; i < comList.size(); i++) {
            key = "dualCom" + QString::number(i);
            setKey = "dualSet" + QString::number(i);

            QString keyValue = comList[i]->currentText();
            QString setKeyValue = setList[i]->currentText();
            qDebug() << setKeyValue;
            std::cout << keyValue.toStdString() << std::endl;
            settingsHandler.storeValue("dualComs", key,
                                       convertComPort(keyValue).c_str());

            qDebug() << " SET INDEX " << setList.at(i)->currentIndex();
            int index = setList[i]->currentIndex();

            int id = availableSets->at(index).getID();
            dualThread.clearBundles();
            auto *bundle = new outputBundle();

            set active = setHandler->getSetById(QString::number(id));
            bundle->setSet(active);
            //    const char *spString = convertComPort(keyValue).c_str();
            //    SerialPort *port = new SerialPort(spString);
            //    bundle->setSerialPortString(&spString);
            //    bundle->setSerialPort(*port);

            auto *outputs = new QMap<int, Output *>();
            *outputs = active.getOutputs();
            QMap<int, Output *>::iterator j;
            for (j = outputs->begin(); j != outputs->end(); j++) {
                outputsToMap.append(j.value());
            }
            dualThread.setOutputsToMap(outputsToMap);
            bundle->setOutputsInSet(*outputs);
            dualThread.addBundle(bundle);
            settingsHandler.storeValue("dualComSet", setKey, id);
        }

        //    ui->stopButton->setVisible(true);
        //    QString comText = ui->outputComboBoxBase->currentText();
        // std::string comNr
        //    R"(\\.\COM)" + comText.toStdString().std::string::substr(3, 2);

        dualThread.abortDual = false;
        dualThread.start();
    } else {
        auto *startButton = ui->dualWidgetContainer->findChild<QPushButton *>("3startButton");
        startButton->setChecked(false);
        startButton->setText("Start");
        startButton->setEnabled(true);

        auto *stopButton = ui->dualWidgetContainer->findChild<QPushButton *>("3stopBtn");
        stopButton->setChecked(true);
        stopButton->setStyleSheet("background-color:#0F4C5C");

        auto dualWarningBox = ui->dualWidgetContainer->findChild<QLayout *>("dualWarningBox");
        clearChildrenFromLayout(dualWarningBox);
        if (emptyDualCom) {
            dualWarningBox->addWidget(returnWarningString(NOCOMPORT));
        }
        if (emptyDualSet) {
            dualWarningBox->addWidget(returnWarningString(NOSET));
        }


    }
}

bool MainWindow::checkIfComboIsEmpty(QList<QComboBox *> toCheck) {
    for (auto &i: toCheck) {
        if (i->currentIndex() == -1) {
            return true;
        } else return false;
    }
}

void MainWindow::clearChildrenFromLayout(QLayout *toClear) {
    if (toClear != nullptr) {
        QLayoutItem *item;
        while ((item = toClear->takeAt(0)) != nullptr) {
            delete item->widget();
            delete item;
        }
    }
}

QLabel *MainWindow::returnWarningString(int warningType) {
    auto warningLabel = new QLabel();
    warningLabel->setStyleSheet("color:#B33A3A");
    switch (warningType) {
        case NOCOMPORT:
            warningLabel->setText("Please select a com port before pressing start");
            break;
        case NOSET:
            warningLabel->setText("Please create or select a set before pressing start");
            break;
    }
    return warningLabel;
}

void MainWindow::refreshComs(int mode) {
    auto *widget = new QWidget();

    switch (mode) {
        case 1:
            widget = ui->inWidgetContainer;
            break;
        case 2:
            widget = ui->outWidgetContainer;
            break;
        case 3:
            widget = ui->dualWidgetContainer;
            break;
        default:
            break;
    }
    QRegularExpression search("comBox");
    QList<QComboBox *> comList = widget->findChildren<QComboBox *>(search);
    formbuilder.loadComPortData();
    auto sets = *formbuilder.getAvailableSets();
    QList<QString> coms = formbuilder.getAvailableComPorts();

    for (int i = 0; i < comList.size(); i++) {
        qDebug() << "hit size:" << comList.size();

        comList[i]->clear();
        for (auto &com : coms) {
            comList[i]->addItem(com);
            qDebug() << com;
        }
    }
    qDebug() << "refresh" << mode;
}

void MainWindow::stopMode(int mode) {
    switch (mode) {
        case 1:
            stopInput();
            break;
        case 2:
            stopOutput();
            break;
        case 3:
            stopDual();
            break;
        default:
            break;
    }
}

void MainWindow::addCom(int mode) {
    auto *layout = new QVBoxLayout();
    bool set = false;
    switch (mode) {
        case 1:
            layout = ui->inLayoutContainer;
            break;
        case 2:
            layout = ui->outLayoutContainer;
            set = true;
            break;
        case 3:
            layout = ui->dualLayoutContainer;
            set = true;
            break;
        default:
            break;
    }

    layout->addWidget(formbuilder.generateComSelector(set, mode));
    qDebug() << "addCom" << mode;
}

void MainWindow::stopInput() {
    inputThread.abortInput = true;

    inputThread.quit();
}

void MainWindow::stopOutput() { outputThread.abort = true; }

void MainWindow::stopDual() { dualThread.abortDual = true; }

void MainWindow::on_updateButton_clicked() {
    QString qUrl = QString::fromStdString(url);
    QDesktopServices::openUrl(QUrl(qUrl));
}

int MainWindow::getComboxIndex(QComboBox *comboBox, QString value) {
    int index = 0;
    qDebug() << value;
    for (int i = 0; i < comboBox->count(); i++) {
        QString text = comboBox->itemText(i);
        if (text.contains(value)) {
            index = i;
        }
    }
    return index;
}

MainWindow::~MainWindow() { delete ui; }
