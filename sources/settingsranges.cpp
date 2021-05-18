#include "headers/settingsranges.h"

#include <qlabel.h>
#include <qlineedit.h>

#include <QStringList>
#include <QVBoxLayout>
#include <iostream>

SettingsRanges::SettingsRanges(int amntOfRanges, QStringList labels,
                               QString headerText) {
  this->amntOfRanges = amntOfRanges;
  this->labels = labels;
  this->headerText = headerText;
}

QVBoxLayout* SettingsRanges::CreateRangeRow() {
  QVBoxLayout* rangeRow = new QVBoxLayout();

  QHBoxLayout* headerRow = new QHBoxLayout();
  QHBoxLayout* labelRow = new QHBoxLayout();
  QHBoxLayout* lineEditRow = new QHBoxLayout();

  QLabel* header = new QLabel();
  header->setText(this->headerText);
  headerRow->addWidget(header);
  std::cout << this->amntOfRanges << std::endl;
  for (int i = 0; i < this->amntOfRanges; i++) {
    QLabel* label = new QLabel();
    label->setText(labels[i]);
    labelRow->addWidget(label);
  }

  for (int i = 0; i < this->amntOfRanges; i++) {
    QString lineEditName = headerText + labels[i];
    QLineEdit* line = new QLineEdit();
    lineEditRow->addWidget(line);
  }

  rangeRow->addLayout(headerRow);
  rangeRow->addLayout(labelRow);
  rangeRow->addLayout(lineEditRow);

  return rangeRow;
}
