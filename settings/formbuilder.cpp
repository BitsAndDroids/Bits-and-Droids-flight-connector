#include "settings/formbuilder.h"

#include "outputmenu.h"

#include <headers/constants.h>
#include <outputs/outputhandler.h>
#include <qcombobox.h>
#include <qlabel.h>
#include <qserialportinfo.h>
#include <settings/settingsranges.h>

#include <QCheckBox>
#include <QFormLayout>
#include <QLineEdit>
#include <QPushButton>
#include <outputs/set.h>
#include <QSizePolicy>
#include <iostream>
#include <outputs/output.h>
using namespace std;
FormBuilder::FormBuilder() {
  for (int i = 0; i < constants::supportedEngines; i++) {
    engineHeaders.append("Engine " + QString::number(i + 1));
  }
  for (int i = 0; i < constants::supportedMixtureLevers; i++) {
    rangeHeaders.append("Mixture " + QString::number(i + 1));
  }
  for (int i = 0; i < constants::supportedPropellerLevers; i++) {
    rangeHeaders.append("Propeller " + QString::number(i + 1));
  }
  rangeHeaders.append("Flaps");


}

QVBoxLayout* FormBuilder::generateRange(QString header) {
  auto rangeBlock = new QVBoxLayout();
  auto headerRow = new QHBoxLayout();
  auto headerLabel = new QLabel();
  headerLabel->setText(header);
  headerRow->addWidget(headerLabel);
  rangeBlock->addLayout(headerRow);

  auto labelRow = new QHBoxLayout();
  auto minLabel = new QLabel();
  auto maxLabel = new QLabel();
  minLabel->setText("Min");
  maxLabel->setText("Max");
  labelRow->addWidget(minLabel);
  labelRow->addWidget(maxLabel);
  rangeBlock->addLayout(labelRow);

  auto editTextRow = new QHBoxLayout();
  auto minLE = new QLineEdit();
  minLE->setObjectName(header + "Min");
  minLE->setText("0");
  auto maxLE = new QLineEdit();
  maxLE->setObjectName(header + "Max");
  maxLE->setText("1023");
  editTextRow->addWidget(minLE);
  editTextRow->addWidget(maxLE);
  rangeBlock->addLayout(editTextRow);

  return rangeBlock;
}

QVBoxLayout* FormBuilder::RangeBuilder() {
  QVBoxLayout* rangeHLayout = new QVBoxLayout();
  rangeHLayout->setObjectName("rangeLayout");

  QLabel* rangeHeader = new QLabel();
  rangeHeader->setText("Ranges");
  rangeHLayout->addWidget(rangeHeader);

  QVBoxLayout* engineRanges = new QVBoxLayout();
  engineRanges->setObjectName("engineRangesLayout");

  for (int i = 0; i < engineHeaders.size(); i++) {
    SettingsRanges* engineRange =
        new SettingsRanges(engineLabels.size(), engineLabels, engineHeaders[i]);
    QVBoxLayout* layout = new QVBoxLayout();
    layout = engineRange->CreateRangeRow();
    engineRanges->addLayout(layout);
  }
  rangeHLayout->addLayout(engineRanges);
  rangeHLayout->setContentsMargins(0, 0, 0, 0);

  for (int i = 0; i < rangeHeaders.size(); i++) {
    rangeHLayout->addLayout(generateRange(rangeHeaders[i]));
  }

  return rangeHLayout;
}

QHBoxLayout* FormBuilder::generateComBlock() {
  QHBoxLayout* comContainer = new QHBoxLayout();

  for (int i = 0; i < comHeaders.size(); i++) {
    comContainer->addLayout(generateComColumn(i));
  }

  return comContainer;
}
QVBoxLayout* FormBuilder::generateComColumn(int index) {
  QVBoxLayout* comColumn = new QVBoxLayout();
  QGridLayout* grid = new QGridLayout();
  QLabel* columnHeader = new QLabel();
  columnHeader->setText(comHeaders[index]);
  QLabel* portLabel = new QLabel();
  portLabel->setText("Select your comport: ");
  grid->addWidget(portLabel, 1, 0);
  QComboBox* comBoxes = new QComboBox();
  grid->addWidget(comBoxes, 1, 1);

  return comColumn;
}
QGridLayout* FormBuilder::generateOutputControls(){
    QGridLayout *outputControls = new QGridLayout();
    outputControls->setAlignment(Qt::AlignLeft);
    QLineEdit *setName = new QLineEdit();
    setName->setMaximumWidth(150);
    setName->setObjectName("leSetName");
    QLabel *setNameLabel = new QLabel("Set name");

    QPushButton *saveSet = new QPushButton("Save set");
    connect(saveSet, &QAbstractButton::clicked,this, &FormBuilder::addSet);

    outputControls->addWidget(setNameLabel, 0,0);
    outputControls->addWidget(setName,0,1,Qt::AlignLeft);
    outputControls->addWidget(saveSet, 2,0);
    return outputControls;
}
QVBoxLayout* FormBuilder::generateOutputSetList(){
   QVBoxLayout *outputSetList = new QVBoxLayout();
   outputSetList->setObjectName("outputSetList");
   QLabel *listHeader = new QLabel("Saved sets");
   QFont font = listHeader->font();
   font.setBold(true);
   font.setPointSize(24);
   listHeader->setFont(font);

   outputSetList->addWidget(listHeader);

   return outputSetList;
}
QWidget* FormBuilder::generateActiveSet(set *selectedSet){

    QWidget *activeWidget = new QWidget();
    activeWidget->setObjectName("activeWidget");
    QVBoxLayout *activeSet = new QVBoxLayout();

    QLabel *setNameHeader = new QLabel(selectedSet->getSetName());
    setNameHeader->setObjectName("setNameHeader");
    QFont headerFont = setNameHeader->font();
    headerFont.setPointSize(18);
    headerFont.setBold(true);
    setNameHeader->setFont(headerFont);
    activeSet->addWidget(setNameHeader);


    QGridLayout *outputGrid = new QGridLayout();
    int columnCounter = 0;
    int rowCounter = 0;
    QMap<int,Output*> outputsInSet = selectedSet->getOutputs();
    QMap<int,Output*>::Iterator i;
    for(i = outputsInSet.begin(); i != outputsInSet.end(); i++){
        if(rowCounter % 5 == 0){
            columnCounter++;
            rowCounter = 0;
        }
        rowCounter++;
        QLabel *outputName = new QLabel(i.value()->getCbText());
        outputGrid->addWidget(outputName,rowCounter,columnCounter);
    }
     qDebug()<<"we made it";
    QVBoxLayout* outputList = new QVBoxLayout();
    outputList->setObjectName("savedOutputs");

    activeSet->addLayout(outputList);
    activeSet->addLayout(outputGrid);
    activeWidget->setLayout(activeSet);

    return activeWidget;
}

QWidget* FormBuilder::generateSetRow(set setForRow){
    QWidget *setRowContainer = new QWidget();
    setRowContainer->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
    QHBoxLayout* setRow = new QHBoxLayout();
    QLabel* setRowLabel = new QLabel(setForRow.getSetName());

    QPushButton* editButton = new QPushButton("Edit");
    connect(editButton, &QAbstractButton::clicked,this, &FormBuilder::localEdit);

    QPushButton* deleteButton = new QPushButton("Delete");
    connect(deleteButton, SIGNAL(clicked()),SLOT(localRemove()));

    editButton->setMaximumWidth(150);
    deleteButton->setMaximumWidth(150);
    setRow->addWidget(setRowLabel);
    setRow->addWidget(editButton);
    setRow->addWidget(deleteButton);
    setRowContainer->setLayout(setRow);
    setRowContainer->setAutoFillBackground(true);
    setRowContainer->setPalette(Qt::white);
    setRowContainer->setObjectName(QString::number(setForRow.getID()));
    return setRowContainer;
}


QTabWidget* FormBuilder::generateOutputTabs(){
    QTabWidget *outputTabs = new QTabWidget();
    outputTabs->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    outputTabs->setObjectName("outputTabWidget");
    outputHandler outputHandler;
    auto categorieList = outputHandler.getCategoryStrings();
    auto categorizedOutputs = outputHandler.getOutputsCategorized();

    for(int i = 0; i < categorieList.size(); i++){
        QWidget *newTab = new QWidget();
        QGridLayout *cbGridLayout = new QGridLayout();
        cbGridLayout->setAlignment(Qt::AlignTop);
        for(int j = 0; j < categorizedOutputs[i].size(); j++){

            QCheckBox *checkbox = new QCheckBox();
            checkbox->setMinimumHeight(30);
            QString cbText = categorizedOutputs[i][j].getCbText();
            checkbox->setText(QString(cbText));
            checkbox->setObjectName("cb"+QString::number(categorizedOutputs[i][j].getId()));

            if(j < 15){
            cbGridLayout->addWidget(checkbox,j,0);
            } else if (j < 30) {
                cbGridLayout->addWidget(checkbox,j-15,1);
            } else if(j < 45){
                cbGridLayout->addWidget(checkbox,j-30,2);
            }

        }
        newTab->setLayout(cbGridLayout);
        outputTabs->addTab(newTab,categorieList[i]);
    }
    outputTabs->setMinimumWidth(800);
    outputTabs->adjustSize();
    return outputTabs;
}
void FormBuilder::loadComPortData() {
  availableComPorts.clear();
  availableComPorts.append("");
  foreach (const QSerialPortInfo& serialPortInfo,
           QSerialPortInfo::availablePorts()) {
    availableComPorts.append(serialPortInfo.portName() + " | " +
                             serialPortInfo.description());
  }
}

QHBoxLayout* FormBuilder::generateOutputRow(Output *output) {
    QHBoxLayout *row = new QHBoxLayout();
    QLabel *outputName = new QLabel(output->getCbText());
    row->addWidget(outputName);
    return row;

}


void FormBuilder::localRemove(){
qDebug()<<"apple";
QPushButton* button = qobject_cast<QPushButton*>(sender());
emit removeSet(button->parentWidget()->objectName());
}

void FormBuilder::localEdit(){
    QPushButton* button = qobject_cast<QPushButton*>(sender());
    emit setEdited(button->parentWidget()->objectName());
}
QLabel* FormBuilder::generateHeader(QString text){
    QLabel* header = new QLabel(text);
    QFont font = header->font();
    font.setPointSize(16);
    header->setFont(font);
    return header;
}
QWidget* FormBuilder::generateComSelector(bool setsNeeded,int mode){
    QWidget *comSelector = new QWidget();
    QHBoxLayout* comRow = new QHBoxLayout();
    comSelector->setLayout(comRow);
    QComboBox* comPortComboBox = new QComboBox();
    for (int i = 0; i < availableComPorts.size() ;i++ ) {
        comPortComboBox->addItem(availableComPorts[i]);
    }
    comRow->addWidget(comPortComboBox);
    return comSelector;
}
QWidget* FormBuilder::generateComControls(int mode){
    QWidget* comControls = new QWidget();
    QHBoxLayout* comControlRow = new QHBoxLayout();
    comControls->setLayout(comControlRow);

    //REFRESH BTN
    QPushButton* refreshButton = new QPushButton("Refresh");
    connect(refreshButton, &QAbstractButton::clicked, this, &FormBuilder::refreshPressed);

    //START BTN
    QPushButton* startButton = new QPushButton("Start");
    connect(startButton, &QAbstractButton::clicked, this, &FormBuilder::startPressed);

    //STOP BTN
    QPushButton* stopButton = new QPushButton("Stop");
    connect(stopButton, &QAbstractButton::clicked, this, &FormBuilder::stopPressed);

    //ADD BTN
    QPushButton* addButton = new QPushButton("+");
    connect(addButton, &QAbstractButton::clicked, this, &FormBuilder::addPressed);


    return comControls;
}


void FormBuilder::localStart(){
    emit startPressed(1);
}
void FormBuilder::localRefreshed(){
    emit refreshPressed(1);
}
void FormBuilder::localStop(){
    emit stopPressed(1);
}
void FormBuilder::localAdd(){
    emit addPressed(1);
}
