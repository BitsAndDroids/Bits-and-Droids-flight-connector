#include "eventwindow.h"

#include <qstring.h>

#include <QGridLayout>
#include <QLabel>
#include <QPushButton>
#include <QString>

#include "ui_eventwindow.h"

EventWindow::EventWindow(QWidget *parent)
    : QWidget(parent), ui(new Ui::EventWindow) {
  ui->setupUi(this);
  auto eventGrid = new QGridLayout();
  QStringList headers = {"Prefix", "Event", "Type", "Update every", "Comment"};
  for (int i = 0; i < headers.size(); i++) {
    auto headerLabel = new QLabel(headers[i]);
    eventGrid->addWidget(headerLabel, 0, i);
  }
  this->setLayout(eventGrid);
}

EventWindow::~EventWindow() { delete ui; }
