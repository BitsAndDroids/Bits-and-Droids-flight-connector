#include "settings/formbuilder.h"

#include <qcombobox.h>
#include <qlabel.h>
#include <qserialportinfo.h>
#include <settings/settingsranges.h>

#include <QLineEdit>
#include <iostream>
using namespace std;
FormBuilder::FormBuilder() {
  //    lights[make_pair(<QString, QString,bool>(("Test","Test"),true));)
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
  auto maxLE = new QLineEdit();
  maxLE->setObjectName(header + "Max");
  editTextRow->addWidget(minLE);
  editTextRow->addWidget(maxLE);
  rangeBlock->addLayout(editTextRow);

  return rangeBlock;
}

QVBoxLayout* FormBuilder::RangeBuilder() {
  QVBoxLayout* rangeHLayout = new QVBoxLayout();

  QVBoxLayout* engineRanges = new QVBoxLayout();
  engineRanges->setObjectName("Engine ranges");
  QStringList engineLabels = {"Reverse", "Idle cutoff", "Max"};
  QStringList headers = {"Engine 1", "Engine 2", "Engine 3", "Engine 4"};
  for (int i = 0; i < headers.size(); i++) {
    cout << "test " << engineLabels.size() << endl;
    SettingsRanges* engineRange =
        new SettingsRanges(engineLabels.size(), engineLabels, headers[i]);
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

void FormBuilder::loadComPortData() {
  availableComPorts.clear();
  availableComPorts.append("");
  foreach (const QSerialPortInfo& serialPortInfo,
           QSerialPortInfo::availablePorts()) {
    availableComPorts.append(serialPortInfo.portName() + " | " +
                             serialPortInfo.description());
  }
}
