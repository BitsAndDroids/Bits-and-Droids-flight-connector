#include "outputmenu.h"
#include "ui_outputmenu.h"
#include "formbuilder.h"
FormBuilder formBuilder;
OutputMenu::OutputMenu(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::OutputMenu)
{
    ui->setupUi(this);
    ui->containerLayout->addWidget(formBuilder.generateOutputTabs());
    ui->outputWidgetContainer->adjustSize();
    this->adjustSize();

}

OutputMenu::~OutputMenu()
{
    delete ui;
}
