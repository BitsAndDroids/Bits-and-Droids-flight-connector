#include <iostream>
#include "outputmenu.h"
#include "outputmenu/builder/SetrowBuilder.h"
#include "outputmenu/builder/OutputTabBuilder.h"
#include "outputmenu/elements/SetDetails.h"

OutputMenu::OutputMenu(QWidget *parent)
        : QWidget(parent) {

    this->setWindowTitle("Output menu");

    auto outputMenuVLayout = new QVBoxLayout(this);
    auto outputMenuLayout = new QHBoxLayout();
    //objectname is used to identify the widget when displaying ui components
    outputMenuLayout->setObjectName("outputMenuLayout");

    addMenuBar();

    //outputMenuVLayout->addItem(new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum));
    outputMenuVLayout->addLayout(outputMenuLayout);
    outputMenuLayout->setAlignment({Qt::AlignTop, Qt::AlignLeft});
    //We pass this OutputMenu to the builder so it can connect showSetDetails when a setrow is clicked
    auto setrowBuilder = SetrowBuilder(this);
    outputMenuLayout->addWidget(setrowBuilder.buildSetrowContainer());
    outputMenuLayout->addWidget(
            setDetaisBuilder.buildOutputDetailsContainer(QString::number(setHandler.getSets()->first().getID())));
    outputMenuLayout->addWidget(outputTabBuilder.buildOutputTabContainer());

    this->findChild<QWidget *>("outputTabWidget")->setVisible(false);
    this->setMinimumHeight(500);
    this->setStyleSheet("background-color:#487f94;");
    this->show();
}

OutputMenu::~OutputMenu() {
    emit OutputMenu::closedOutputMenu();
    delete this;
}

void OutputMenu::addMenuBar() {
    auto *menuBar = new QMenuBar();
    menuBar->setStyleSheet("QMenuBar {"
                           "    background-color: #fff;"
                           "    spacing: 3px; /* spacing between menu bar items */\n"
                           "}");
    auto *setMenu = new QMenu("Sets");
    setMenu->addAction("Create set");
    setMenu->addAction("Import Set");
    menuBar->addMenu(setMenu);
    //display menuBar full width of window
    menuBar->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    this->layout()->setMenuBar(menuBar);
}

void OutputMenu::closeEvent(QCloseEvent *event) { delete this; }

void OutputMenu::editSet(QString id) {
    outputTabBuilder.setCheckboxes(id);
    this->findChild<QWidget *>("outputTabWidget")->setVisible(true);
}

void OutputMenu::addNewSet() {
//    auto *lineEditName = ui->widget->findChild<QLineEdit *>("leSetName");
//
//    bool ok;
//    QString name =
//            QInputDialog::getText(this, tr("Name"), tr("Enter a name:"),
//                                  QLineEdit::Normal, QDir::home().dirName(), &ok);
//    if (ok && !name.isEmpty()) {
//        auto *newSet = new Set(name);
//
//        Set *setSaved = setHandler.saveSet(newSet);
//        ui->widget->findChild<QVBoxLayout *>("outputSetList")
//                ->addWidget(formBuilder.generateSetRow(*setSaved));
//    }
}

//void OutputMenu::removeSetAction(QString id) {
//    // QPushButton *btn = qobject_cast<QPushButton*>(test);
//    std::cout << id.toInt() << " : SET TO DELETE" << std::endl;
//    auto removeWidget = ui->widget->findChild<QWidget *>(id);
//    removeWidget->hide();
//    setHandler.removeSet(id);
//    // delete removeWidget;
//}
//
//void OutputMenu::editSet(QString id) {
//    Set setFound = setHandler.getSetById(id);
//
//    qDebug() << "a";
//    ui->widget->findChild<QTabWidget *>("outputTabWidget")->setVisible(true);
//    ui->widget->findChild<QTabWidget *>("outputTabWidget")->setMaximumHeight(450);
//    auto *widgetFound = ui->widget->findChild<QWidget *>("activeWidget");
//
//    delete widgetFound;
//
//    ui->widget->adjustSize();
//
//    auto *container = ui->widget->findChild<QGridLayout *>("rightGrid");
//    QWidget * setActiveWidget = formBuilder.generateActiveSet(&setFound);
//    container->setAlignment(Qt::AlignTop);
//
//    container->parentWidget()->setMinimumHeight(650);
//    container->parentWidget()->setMaximumHeight(650);
//    container->addWidget(setActiveWidget, 1, 0);
//    auto *setHeader = ui->widget->findChild<QLabel *>("setNameHeader");
//
//    setHeader->setText(setFound.getSetName());
//
//    activeSet = id.toInt();
//
//    QMap<int, Output *> outputsToToggle = setFound.getOutputs();
//    QMap<int, Output *>::iterator i;
//
//    for (i = outputsToToggle.begin(); i != outputsToToggle.end(); i++) {
//        if (this->findChild<QCheckBox *>("cb" +
//                                         QString::number(i.value()->getId()))) {
//            ui->widget
//                    ->findChild<QCheckBox *>("cb" + QString::number(i.value()->getId()))
//                    ->setChecked(true);
//        }
//    }
//
//    this->resize(1200, 650);
//}

void OutputMenu::showSetDetails(QString id) {
    emit displaySetDetails(id);

    auto *widgetFound = this->findChild<SetDetails *>("setDetailsContainer");
    this->layout()->replaceWidget(widgetFound, setDetaisBuilder.buildOutputDetailsContainer(id));



    this->findChild<QWidget *>("outputTabWidget")->setVisible(false);
    widgetFound->deleteLater();
    this->adjustSize();
    this->update();
    this->adjustSize();
}

void OutputMenu::saveEdit() {
//    QList<QCheckBox *> cbList = ui->widget->findChildren<QCheckBox *>();
//    Set setToEdit = setHandler.getSetById(QString::number(activeSet));
//    setToEdit.clearOutputs();
//    for (auto &i: cbList) {
//        if (i->isChecked()) {
//            QString cbName = i->objectName();
//            if (outputHandler->findOutputById(cbName.mid(2).toInt())) {
//                Output *outputSelected =
//                        outputHandler->findOutputById(cbName.mid(2).toInt());
//                setToEdit.addOutput(outputSelected);
//            }
//        }
//    }
//
//    ui->widget->findChild<QTabWidget *>("outputTabWidget")->setVisible(false);
//    ui->widget->adjustSize();
//    this->adjustSize();
//    setHandler.saveSet(&setToEdit);
}
