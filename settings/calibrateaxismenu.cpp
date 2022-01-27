#include "calibrateaxismenu.h"

#include <qboxlayout.h>
#include <stdio.h>

#include <iostream>

#include "formBuilder.h"
#include "ui_calibrateaxismenu.h"
using namespace std;
CalibrateAxisMenu::CalibrateAxisMenu(QWidget *parent)
    : QWidget(parent), ui(new Ui::CalibrateAxisMenu) {
  ui->setupUi(this);
  builder->loadPointsToPlot(curves);
  this->setLayout(GenerateCurveLayouts());
}

CalibrateAxisMenu::~CalibrateAxisMenu() { delete ui; }
void CalibrateAxisMenu::saveSettings() {
  for (int i = 0; i < curves.size(); i++) {
    QList<coordinates> *coords = builder->getCoordinates(i);
    cout << curves[i].toStdString() << endl;
    QStringList sliderNames = {
        QString::number(i) + curves[i] + "Deadzone",
        QString::number(i) + curves[i] + "MinSensitivity",
        QString::number(i) + curves[i] + "PlusSensitivity"};
    for (const auto &sliderName : sliderNames) {
      auto *sliderFound = findChild<QSlider *>(sliderName);
      cout << sliderFound->objectName().toStdString() << endl;
      settingsHandler.storeSubGroup(curves[i] + "Series", "sliders", sliderName,
                                    sliderFound->value());
    }
    cout << coords->size() << "size" << endl;
    for (int j = 0; j < coords->size(); j++) {
      cout << i << endl;
      settingsHandler.storeSubGroup(curves[i] + "Series", "axis",
                                    QString::number(j), coords->at(j).getX());
      settingsHandler.storeSubGroup(curves[i] + "Series", "value",
                                    QString::number(j), coords->at(j).getY());
    }
    QStringList rudderLineEdits = builder->getCalibrateLabels();
    for (auto &line : rudderLineEdits) {
      auto rudderValsFound =
          findChild<QLineEdit *>(curves[i] + line)->text().toInt();
      settingsHandler.storeSubGroup(curves[i] + "Series", "calibrations", line,
                                    rudderValsFound);
    }
  }
}
QGridLayout *CalibrateAxisMenu::GenerateCurveLayouts() {
  auto curveGrid = new QGridLayout();
  int counter = 0;
  qDebug() << "Yoah";

  builder->setCurves(curves);
  for (int i = 0; i < curves.size(); i++) {
    qDebug() << "loopie";
    auto *wdg = new QWidget();

    // builder->loadPointsToPlot(curves);

    curveGrid->addLayout(builder->createAxisRow(curves[i], i), counter / 2,
                         counter % 2);
    counter++;
  }
  for (auto &curve : curves) {
    if (!settingsHandler
             .retrieveSubSetting(curve + "Series", "sliders",
                                 curve + "Deadzone")
             ->isNull()) {
      this->findChild<QSlider *>(curve + "Deadzone")
          ->setValue(settingsHandler
                         .retrieveSubSetting(curve + "Series", "sliders",
                                             curve + "Deadzone")
                         ->toInt());
      this->findChild<QSlider *>(curve + "MinSensitivity")
          ->setValue(settingsHandler
                         .retrieveSubSetting(curve + "Series", "sliders",
                                             curve + "MinSensitivity")
                         ->toInt());

      this->findChild<QSlider *>(curve + "PlusSensitivity")
          ->setValue(settingsHandler
                         .retrieveSubSetting(curve + "Series", "sliders",
                                             curve + "PlusSensitivity")
                         ->toInt());
      // Range handling
    }
  }
  auto saveBtn = new QPushButton("Save");
  connect(saveBtn, &QPushButton::clicked, this,
          &CalibrateAxisMenu::saveSettings);
  curveGrid->addWidget(saveBtn, 2, 0);
  return curveGrid;
}
