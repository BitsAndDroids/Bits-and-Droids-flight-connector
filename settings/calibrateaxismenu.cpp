#include "calibrateaxismenu.h"
#include "formbuilder.h"
#include "ui_calibrateaxismenu.h"

#include <qboxlayout.h>

CalibrateAxisMenu::CalibrateAxisMenu(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CalibrateAxisMenu)
{
    ui->setupUi(this);
    this->setLayout(GenerateCurveLayouts());

}

CalibrateAxisMenu::~CalibrateAxisMenu()
{
    delete ui;
}

QGridLayout* CalibrateAxisMenu::GenerateCurveLayouts(){
    auto curveGrid = new QGridLayout();
    int counter = 0;
    for(int i = 0; i < curves.size(); i++){
        auto *wdg = new QWidget();
        auto builder = new FormBuilder();
        auto sensLayout = new QVBoxLayout();
        builder->loadPointsToPlot(curves);
        sensLayout->addLayout(builder->createAxisRow(curves[i],i));

        curveGrid->addLayout(builder->createAxisRow(curves[i], i),counter / 2, counter%2);
        counter++;
    }

    return curveGrid;
}
