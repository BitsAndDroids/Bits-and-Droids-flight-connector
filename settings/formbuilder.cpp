#include "settings/formbuilder.h"

#include <headers/constants.h>
#include <outputs/outputhandler.h>
#include <qcombobox.h>
#include <qlabel.h>
#include <qserialportinfo.h>
#include <settings/settingsranges.h>


#include <QCheckBox>
#include <QFormLayout>
#include <QLineEdit>
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
QTabWidget* FormBuilder::generateOutputTabs(){
    QTabWidget *outputTabs = new QTabWidget();
    outputHandler outputHandler;
    auto categorieList = outputHandler.getCategoryStrings();
    auto categorizedOutputs = outputHandler.getOutputsCategorized();

    for(int i = 0; i < categorieList.size(); i++){
        QWidget *newTab = new QWidget();
        QGridLayout *cbGridLayout = new QGridLayout();
        cbGridLayout->setAlignment(Qt::AlignTop);
        for(int j = 0; j < categorizedOutputs[i].size(); j++){

            QCheckBox *checkbox = new QCheckBox();
            QString cbText = categorizedOutputs[i][j].getCbText();
            checkbox->setText(QString(cbText));

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
