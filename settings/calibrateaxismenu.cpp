#include "calibrateaxismenu.h"

#include <qboxlayout.h>

#include "formBuilder.h"
#include "ui_calibrateaxismenu.h"

CalibrateAxisMenu::CalibrateAxisMenu(QWidget *parent)
    : QWidget(parent), ui(new Ui::CalibrateAxisMenu) {
  ui->setupUi(this);
  this->setLayout(GenerateCurveLayouts());
}

CalibrateAxisMenu::~CalibrateAxisMenu() { delete ui; }

QGridLayout *CalibrateAxisMenu::GenerateCurveLayouts() {
  auto curveGrid = new QGridLayout();
  int counter = 0;
  qDebug() << "Yoah";
  auto builder = new FormBuilder();
  builder->setCurves(curves);
  for (int i = 0; i < curves.size(); i++) {
    qDebug() << "loopie";
    auto *wdg = new QWidget();

    // builder->loadPointsToPlot(curves);

    curveGrid->addLayout(builder->createAxisRow(curves[i], i), counter / 2,
                         counter % 2);
    counter++;
  }
  auto saveBtn = new QPushButton("Save");
  curveGrid->addWidget(saveBtn, 2, 0);
  return curveGrid;
}
