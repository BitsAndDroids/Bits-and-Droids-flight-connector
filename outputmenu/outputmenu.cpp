#include <iostream>
#include "outputmenu.h"
#include "outputmenu/builder/OutputTabBuilder.h"
#include "outputmenu/elements/SetDetails.h"
#include "logging/MessageCaster.h"

OutputMenu::OutputMenu(ServiceWorker *pWorker)
{
    this->setWindowTitle("Output menu");
    this->setObjectName("outputMenu");
    auto outputMenuVLayout = new QVBoxLayout(this);
    auto outputMenuLayout = new QHBoxLayout();
    //objectname is used to identify the widget when displaying ui components
    outputMenuLayout->setObjectName("outputMenuLayout");

    addMenuBar();

    //outputMenuVLayout->addItem(new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum));
    outputMenuVLayout->addLayout(outputMenuLayout);
    outputMenuLayout->setAlignment({Qt::AlignTop, Qt::AlignLeft});
    //We pass this OutputMenu to the controller so it can connect showSetDetails when a setrow is clicked
    outputMenuLayout->addWidget(setrowBuilder.buildSetrowContainer());

    if (!setHandler.getSets()->empty()) {
        outputMenuLayout->addWidget(
                setDetaisBuilder.buildOutputDetailsContainer(QString::number(setHandler.getSets()->first().getID())));
    } else{
        outputMenuLayout->addWidget(setDetaisBuilder.buildOutputDetailsContainer());
    }

    outputMenuLayout->addWidget(outputTabBuilder.buildOutputTabContainer());

    this->findChild<QWidget *>("outputTabWidget")->setVisible(false);
    this->setMinimumHeight(500);
    this->setStyleSheet("QWidget#outputMenu{background-color:#487f94;}");

    QObject::connect(&outputTabBuilder, &OutputTabBuilder::setEdited, this, &OutputMenu::showSetDetails);

    connect(&setrowBuilder, &SetrowBuilder::showSetDetailsSignal, this, &OutputMenu::showSetDetails);
    connect(&setrowBuilder, &SetrowBuilder::editSetSignal, this, &OutputMenu::editSet);
    connect(&setrowBuilder, &SetrowBuilder::deleteSetSignal, this, &OutputMenu::deleteSet);
    connect(&setrowBuilder, &SetrowBuilder::createSetSignal, this, &OutputMenu::createSet);
}

OutputMenu::~OutputMenu() {
    emit closedOutputMenu();
}

void OutputMenu::addMenuBar() {
    auto *menuBar = new QMenuBar();
    menuBar->setStyleSheet("QMenuBar {"
                           "    background-color: #fff;"
                           "    spacing: 3px; /* spacing between menu bar items */\n"
                           "}");
    auto *setMenu = new QMenu("Sets");

    auto createSetAction = new QAction("Create set");
    setMenu->addAction(createSetAction);

    auto importSetAction = new QAction("Import set");
    setMenu->addAction(importSetAction);

    menuBar->addMenu(setMenu);
    //display menuBar full width of window
    menuBar->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    this->layout()->setMenuBar(menuBar);

    connect(createSetAction, &QAction::triggered, this, &OutputMenu::createSet);
    connect(importSetAction, &QAction::triggered, this, &OutputMenu::importSet);

}

void OutputMenu::closeEvent(QCloseEvent *event) {
    emit closedOutputMenu();
    delete this;
}

void OutputMenu::editSet(const QString& id) {
    this->findChild<QWidget *>("outputTabWidget")->setVisible(true);
    outputTabBuilder.setCheckboxes(id);
}

void OutputMenu::createSet() {
    bool ok;
    QString name =
            QInputDialog::getText(this, tr("Create a new set"), tr("Enter a descriptive name:"),
                                  QLineEdit::Normal, QDir::home().dirName(), &ok);
    if (ok && !name.isEmpty()) {
        auto *newSet = new Set(name);

        Set *setSaved = setHandler.saveSet(newSet);
        setrowBuilder.createSet(QString::number(setSaved->getID()));
        auto newSetsButton = this->findChild<QPushButton *>("newSetButton");
        if(this->findChild<QPushButton *>("newSetButton")){
            newSetsButton->setVisible(false);
            showSetDetails(QString::number(setSaved->getID()));
            this->adjustSize();
            this->adjustSize();
        }
    }
}

void OutputMenu::deleteSet(const QString& id) {

    try {
        setHandler.removeSet(id);
        auto widget = this->findChild<QWidget *>(id);
        widget->setVisible(false);
        widget->deleteLater();
        if (setDetaisBuilder.getCurrentSetID() == id) {
            if (!setHandler.getSets()->empty()) {
                showSetDetails(QString::number(setHandler.getSets()->first().getID()));
            } else {
                this->findChild<QWidget *>("setDetailsContainer")->setVisible(false);
            }
        }
    }
    catch (std::exception &e) {
        MessageCaster::showWarningMessage("Could not delete set\n" + QString::fromStdString(e.what()));
    }
    this->findChild<QWidget *>("outputTabWidget")->setVisible(false);
    //TODO find out why i have to call this twice
    this->adjustSize();
    this->adjustSize();
}


void OutputMenu::showSetDetails(const QString& id) {
    emit displaySetDetails(id);

    auto *widgetFound = this->findChild<SetDetails *>("setDetailsContainer");
    this->layout()->replaceWidget(widgetFound, setDetaisBuilder.buildOutputDetailsContainer(id));


    this->findChild<QWidget *>("outputTabWidget")->setVisible(false);
    widgetFound->deleteLater();
    //TODO find out why i have to call this twice
    this->adjustSize();
    this->update();
    this->adjustSize();
}

void OutputMenu::importSet(){
    auto filePath = QFileDialog::getOpenFileName(this, "Import set", "", "*.json");
    QString fileName = QInputDialog::getText(this, "Import set", "Enter a name for the set");
    setrowBuilder.createSet(QString::number(setHandler.importSet(filePath, fileName)));
}
