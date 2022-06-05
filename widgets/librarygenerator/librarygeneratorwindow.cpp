#include "librarygeneratorwindow.h"
#include "ui_librarygeneratorwindow.h"

#include <qlabel.h>

LibraryGeneratowWindow::LibraryGeneratowWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LibraryGeneratowWindow)
{
    ui->setupUi(this);
    auto testLabel = new QLabel("test");
    windowGridLayout->addWidget(testLabel,0,0);
    this->setLayout(windowGridLayout);


}

LibraryGeneratowWindow::~LibraryGeneratowWindow()
{
    delete ui;
}
