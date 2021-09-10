#include "outputmenu.h"
#include "formbuilder.h"
#include<headers/mainwindow.h>
#include "ui_outputmenu.h"

#include <QLabel>

FormBuilder formBuilder;
outputHandler outputHandler;
SetHandler setHandler;


OutputMenu::OutputMenu(QWidget *parent)
    : QWidget(parent), ui(new Ui::OutputMenu) {
  ui->setupUi(this);
  auto *foundSets = new QList<set>();
  foundSets = setHandler.getSets();

  connect(&formBuilder, &FormBuilder::addSet, this, &OutputMenu::addNewSet);
  connect(&formBuilder, &FormBuilder::setEdited, this, &OutputMenu::editSet);

  ui->containerLayout->setAlignment(Qt::AlignLeft);
  ui->containerLayout->addLayout(formBuilder.generateOutputSetList());
  ui->containerLayout->addLayout(formBuilder.generateOutputControls());

  auto *activeLayout = new QVBoxLayout();
  activeLayout->setObjectName("activeContainer");

  ui->containerLayout->addLayout(activeLayout);

  QStringList *keys = settingsHandler.retrieveKeys("sets");
  for (const auto & foundSet : *foundSets) {
    // qDebug()<<foundSets->at(i).getSetName()<< "wuttie";
    ui->widget->findChild<QVBoxLayout *>("outputSetList")
        ->addWidget(formBuilder.generateSetRow(foundSet));
  }

  ui->containerLayout->addWidget(formBuilder.generateOutputTabs());

  auto *saveEdit = new QPushButton("Save edit");
  saveEdit->setObjectName("btnSsaveEdit");
  connect(saveEdit, &QAbstractButton::clicked, this, &OutputMenu::saveEdit);
  ui->containerLayout->addWidget(saveEdit);
  ui->widget->findChild<QTabWidget *>("outputTabWidget")->setVisible(false);
  ui->widget->adjustSize();
  this->adjustSize();

  connect(&formBuilder, &FormBuilder::removeSet, this,
          &OutputMenu::removeSetAction);
}

OutputMenu::~OutputMenu() {
    emit OutputMenu::closedOutputMenu();
    qDebug()<<"closed here";
    delete ui;
}
void OutputMenu::closeEvent(QCloseEvent *event) {
    qDebug()<<'clEvent';
    delete this;
}

void OutputMenu::addNewSet() {
  auto *lineEditName = ui->widget->findChild<QLineEdit *>("leSetName");
  set *newSet = new set(lineEditName->text());

  set *setSaved = setHandler.saveSet(newSet);
  ui->widget->findChild<QVBoxLayout *>("outputSetList")
      ->addWidget(formBuilder.generateSetRow(*setSaved));
}

void OutputMenu::removeSetAction(QString id) {
  // QPushButton *btn = qobject_cast<QPushButton*>(test);
  qDebug() << id.toInt();
  auto removeWidget = ui->widget->findChild<QWidget *>(id);
  removeWidget->hide();
  setHandler.removeSet(id);
  // delete removeWidget;
}

void OutputMenu::editSet(QString id) {
  set setFound = setHandler.getSetById(id);

  qDebug() << "a";
  ui->widget->findChild<QTabWidget *>("outputTabWidget")->setVisible(true);
  auto *widgetFound = ui->widget->findChild<QWidget *>("activeWidget");
  qDebug() << "deleted";
  delete widgetFound;

  ui->widget->adjustSize();

  auto *container =
      ui->widget->findChild<QVBoxLayout *>("activeContainer");
  QWidget *setActiveWidget = formBuilder.generateActiveSet(&setFound);
  container->addWidget(setActiveWidget);

  auto *setHeader = ui->widget->findChild<QLabel *>("setNameHeader");

  setHeader->setText(setFound.getSetName());

  qDebug() << "b";
  //    QVBoxLayout *outputList =
  //    ui->widget->findChild<QVBoxLayout*>("savedOutputs");

  //    QMap<int, Output*> map = setFound.getOutputs();
  //    QMap<int, Output*>::Iterator i;
  //    for(i = map.begin(); i != map.end(); i ++){
  //       outputList->addLayout(formBuilder.generateOutputRow(i.value()));
  //       qDebug()<<"outputs"<<i.value()->getCbText();
  //    }

  activeSet = id.toInt();
  QMap<int, Output *> outputsToToggle = setFound.getOutputs();
  QMap<int, Output *>::iterator i;
  for (i = outputsToToggle.begin(); i != outputsToToggle.end(); i++) {
    ui->widget
        ->findChild<QCheckBox *>("cb" + QString::number(i.value()->getId()))
        ->setChecked(true);
  }
  ui->widget->findChild<QTabWidget *>("outputTabWidget")->adjustSize();
  ui->widget->adjustSize();
  this->adjustSize();
}
void OutputMenu::saveEdit() {
  QList<QCheckBox *> cbList = ui->widget->findChildren<QCheckBox *>();
  set setToEdit = setHandler.getSetById(QString::number(activeSet));
  setToEdit.clearOutputs();
  for (auto & i : cbList) {
    if (i->isChecked()) {
      QString cbName = i->objectName();
      qDebug() << cbName;
      Output *outputSelected =
          outputHandler.findOutputById(cbName.midRef(2).toInt());
      ;

      qDebug() << outputSelected->getCbText();
      setToEdit.addOutput(outputSelected);
    }
  }

  ui->widget->findChild<QTabWidget *>("outputTabWidget")->setVisible(false);
  ui->widget->adjustSize();
  this->adjustSize();
  setHandler.saveSet(&setToEdit);
}
