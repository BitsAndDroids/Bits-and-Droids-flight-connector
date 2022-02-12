#include "outputmenu.h"

#include <headers/mainwindow.h>

#include <QLabel>

#include "ui_outputmenu.h"

OutputMenu::OutputMenu(QWidget *parent)
    : QWidget(parent), ui(new Ui::OutputMenu) {
  ui->setupUi(this);
  auto *foundSets = new QList<set>();
  foundSets = setHandler.getSets();
  this->setWindowTitle("Output menu");
  connect(&formBuilder, &FormBuilder::addSet, this, &OutputMenu::addNewSet);
  connect(&formBuilder, &FormBuilder::setEdited, this, &OutputMenu::editSet);
  auto gridLayout = new QGridLayout();
  gridLayout->setObjectName("gridFrame");
  auto leftCol = new QVBoxLayout();
  leftCol->setAlignment(Qt::AlignTop);
  auto rightCol = new QVBoxLayout();
  rightCol->setAlignment(Qt::AlignTop);
  rightCol->setObjectName("rightCol");
  leftCol->addLayout(formBuilder.generateOutputSetList());

  //  gridLayout->addLayout(formBuilder.generateOutputControls(), 2, 0);
  //  gridLayout->addLayout(formBuilder.generateOutputSetList(), 1, 0);
  gridLayout->addLayout(leftCol, 0, 0);

  // ui->containerLayout->setAlignment(Qt::AlignLeft);
  ui->containerLayout->addLayout(gridLayout);

  auto *activeLayout = new QVBoxLayout();
  activeLayout->setObjectName("activeContainer");

  // gridLayout->addLayout(activeLayout, 2, 0);
  // rightCol->addLayout(activeLayout);
  QStringList *keys = settingsHandler.retrieveKeys("sets");
  for (const auto &foundSet : *foundSets) {
    // qDebug()<<foundSets->at(i).getSetName()<< "wuttie";
    ui->widget->findChild<QVBoxLayout *>("outputSetList")
        ->addWidget(formBuilder.generateSetRow(foundSet));
  }

  auto *saveEdit = new QPushButton("Save edit");
  saveEdit->setObjectName("btnSsaveEdit");
  connect(saveEdit, &QAbstractButton::clicked, this, &OutputMenu::saveEdit);
  // gridLayout->addWidget(saveEdit, 4, 1);
  auto buttonRow = new QHBoxLayout();
  buttonRow->addLayout(formBuilder.generateOutputControls());
  buttonRow->addWidget(saveEdit);
  leftCol->addLayout(buttonRow);
  auto rightGrid = new QGridLayout();
  rightGrid->setObjectName("rightGrid");
  rightGrid->addLayout(activeLayout, 1, 0);
  rightCol->addLayout(rightGrid);
  rightGrid->addWidget(formBuilder.generateOutputTabs(), 0, 0);
  gridLayout->addLayout(rightCol, 0, 1);
  ui->widget->findChild<QTabWidget *>("outputTabWidget")->setVisible(false);
  ui->widget->adjustSize();
  this->adjustSize();

  connect(&formBuilder, &FormBuilder::removeSet, this,
          &OutputMenu::removeSetAction);
}

OutputMenu::~OutputMenu() {
  emit OutputMenu::closedOutputMenu();
  qDebug() << "closed here";
  delete ui;
}
void OutputMenu::closeEvent(QCloseEvent *event) { delete this; }

void OutputMenu::addNewSet() {
  auto *lineEditName = ui->widget->findChild<QLineEdit *>("leSetName");

  bool ok;
  QString name =
      QInputDialog::getText(this, tr("Name"), tr("Enter a name:"),
                            QLineEdit::Normal, QDir::home().dirName(), &ok);
  if (ok && !name.isEmpty()) {
    auto *newSet = new set(name);

    set *setSaved = setHandler.saveSet(newSet);
    ui->widget->findChild<QVBoxLayout *>("outputSetList")
        ->addWidget(formBuilder.generateSetRow(*setSaved));
  }
}

void OutputMenu::removeSetAction(QString id) {
  // QPushButton *btn = qobject_cast<QPushButton*>(test);
  std::cout << id.toInt() << " : SET TO DELETE" << std::endl;
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

  auto *container = ui->widget->findChild<QGridLayout *>("rightGrid");
  QWidget *setActiveWidget = formBuilder.generateActiveSet(&setFound);
  container->setAlignment(Qt::AlignTop);
  // container->addWidget(setActiveWidget, 0, 0);
  container->addWidget(setActiveWidget, 1, 0);
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
    if (this->findChild<QCheckBox *>("cb" +
                                     QString::number(i.value()->getId()))) {
      ui->widget
          ->findChild<QCheckBox *>("cb" + QString::number(i.value()->getId()))
          ->setChecked(true);
    }
  }
  ui->widget->findChild<QTabWidget *>("outputTabWidget")->adjustSize();
  ui->widget->adjustSize();
  this->adjustSize();
}
void OutputMenu::saveEdit() {
  QList<QCheckBox *> cbList = ui->widget->findChildren<QCheckBox *>();
  set setToEdit = setHandler.getSetById(QString::number(activeSet));
  setToEdit.clearOutputs();
  for (auto &i : cbList) {
    if (i->isChecked()) {
      QString cbName = i->objectName();
      if (outputHandler->findOutputById(cbName.mid(2).toInt())) {
        Output *outputSelected =
            outputHandler->findOutputById(cbName.mid(2).toInt());
        setToEdit.addOutput(outputSelected);
      }
    }
  }

  ui->widget->findChild<QTabWidget *>("outputTabWidget")->setVisible(false);
  ui->widget->adjustSize();
  this->adjustSize();
  setHandler.saveSet(&setToEdit);
}
