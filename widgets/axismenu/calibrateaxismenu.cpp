#include "calibrateaxismenu.h"

#include <qboxlayout.h>
#include <stdio.h>

#include <iostream>

#include "widgets/settingsmenu/builder/formbuilder.h"
#include "ui_calibrateaxismenu.h"
using namespace std;
CalibrateAxisMenu::CalibrateAxisMenu(QWidget *parent)
    : QWidget(parent), ui(new Ui::CalibrateAxisMenu) {
  ui->setupUi(this);
  builder->loadPointsToPlot(curves);
  this->setLayout(GenerateCurveLayouts());
  int counter = 0;
  for (auto &curve : curves) {
    if (!settingsHandler
             .retrieveSubSetting(curve + "Series", "sliders",
                                 QString::number(counter) + curve + "Deadzone")
             ->isNull()) {
      this->findChild<QSlider *>(QString::number(counter) + curve + "Deadzone")
          ->setValue(settingsHandler
                         .retrieveSubSetting(
                             curve + "Series", "sliders",
                             QString::number(counter) + curve + "Deadzone")
                         ->toInt());

      this->findChild<QSlider *>(QString::number(counter) + curve +
                                 "MinSensitivity")
          ->setValue(settingsHandler
                         .retrieveSubSetting(curve + "Series", "sliders",
                                             QString::number(counter) + curve +
                                                 "MinSensitivity")
                         ->toInt());

      this->findChild<QSlider *>(QString::number(counter) + curve +
                                 "PlusSensitivity")
          ->setValue(settingsHandler
                         .retrieveSubSetting(curve + "Series", "sliders",
                                             QString::number(counter) + curve +
                                                 "PlusSensitivity")
                         ->toInt());
    }
    counter++;
  }
}

CalibrateAxisMenu::~CalibrateAxisMenu() {
  emit CalibrateAxisMenu::closedCalibrateAxisMenu();
  delete ui;
}

void CalibrateAxisMenu::saveSettings() {
  for (int i = 0; i < curves.size(); i++) {
    QList<coordinates> *coords = builder->getCoordinates(i);
    QStringList sliderNames = {
        QString::number(i) + curves[i] + "Deadzone",
        QString::number(i) + curves[i] + "MinSensitivity",
        QString::number(i) + curves[i] + "PlusSensitivity"};
    for (const auto &sliderName : sliderNames) {
      auto *sliderFound = findChild<QSlider *>(sliderName);
      settingsHandler.storeSubGroup(curves[i] + "Series", "sliders", sliderName,
                                    sliderFound->value());
    }
    QStringList rudderLineEdits = builder->getCalibrateLabels();
    for (int j = 0; j < coords->size(); j++) {
      settingsHandler.storeSubGroup(curves[i] + "Series", "axis",
                                    QString::number(j), coords->at(j).getX());
      settingsHandler.storeSubGroup(curves[i] + "Series", "value",
                                    QString::number(j), coords->at(j).getY());
    }

    for (int j = 0; j < rudderLineEdits.size(); j++) {
      // Format of the lineEdit objectnames =
      // <NrOfCurve><Nr1-3><NameOfCurve><NameOfLineEditType>
      auto rudderValsFound =
          findChild<QLineEdit *>(QString::number(i) + QString::number(j) +
                                 curves[i] + rudderLineEdits[j])
              ->text()
              .toInt();

      settingsHandler.storeSubGroup(curves[i] + "Series", "calibrations",
                                    rudderLineEdits[j], rudderValsFound);
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

    // controller->loadPointsToPlot(curves);

    curveGrid->addLayout(builder->createAxisRow(curves[i], i), counter / 2,
                         counter % 2);
    counter++;
  }

  auto saveBtn = new QPushButton("Save");
  connect(saveBtn, &QPushButton::clicked, this,
          &CalibrateAxisMenu::saveSettings);
  curveGrid->addWidget(saveBtn, 2, 0);
  return curveGrid;
}
void CalibrateAxisMenu::closeEvent(QCloseEvent *event) { delete this; }
