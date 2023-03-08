#include "settingsranges.h"

#include <qlabel.h>
#include <qlineedit.h>

#include <QStringList>
#include <QtCharts>
#include <iostream>


SettingsRanges::SettingsRanges(int amntOfRanges, QStringList labels,
                               QString headerText) {
  this->amntOfRanges = amntOfRanges;
  this->labels = labels;
  this->headerText = headerText;
}

QVBoxLayout* SettingsRanges::CreateRangeRow() {
  auto* rangeRow = new QVBoxLayout();

  auto* headerRow = new QHBoxLayout();
  auto* labelRow = new QHBoxLayout();
  auto* lineEditRow = new QHBoxLayout();

  auto* header = new QLabel();
  header->setText(this->headerText);
  headerRow->addWidget(header);

  for (int i = 0; i < this->amntOfRanges; i++) {
    auto* label = new QLabel();
    label->setText(labels[i]);
    labelRow->addWidget(label);
  }

  for (int i = 0; i < this->amntOfRanges; i++) {
    QString lineEditName = headerText + labels[i];
    auto* line = new QLineEdit();
    switch (i) {
      case 0:
        line->setText("0");
        break;
      case 1:
        line->setText("0");
        break;
      case 2:
        line->setText("1023");
        break;
    }
    line->setObjectName(lineEditName);
    lineEditRow->addWidget(line);
  }

  rangeRow->addLayout(headerRow);
  rangeRow->addLayout(labelRow);
  rangeRow->addLayout(lineEditRow);

  return rangeRow;
}


