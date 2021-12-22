#include "eventwindow.h"

#include <qstring.h>

#include <QGridLayout>
#include <QLabel>
#include <QPushButton>
#include <QString>
#include <fstream>
#include <iostream>

#include "ui_eventwindow.h"

using namespace std;

struct tableRow {
  std::string prefix;
  std::string event;
  int type;
  float updateEvery;
  std::string comment;
};

QList<tableRow> tableRows = QList<tableRow>();

EventWindow::EventWindow(QWidget *parent)
    : QWidget(parent), ui(new Ui::EventWindow) {
  ui->setupUi(this);
  auto eventGrid = new QGridLayout();
  QStringList headers = {"Prefix", "Event", "Type", "Update every", "Comment"};
  for (int i = 0; i < headers.size(); i++) {
    auto headerLabel = new QLabel(headers[i]);
    eventGrid->addWidget(headerLabel, 0, i);
  }
  readFile();
  int rowCounter = 0;
  for (auto &row : tableRows) {
    auto prefixLabel = new QLabel(QString::fromStdString(row.prefix));
    auto eventLabel = new QLabel(QString::fromStdString(row.event));
    eventGrid->addWidget(prefixLabel, rowCounter, 0);
    eventGrid->addWidget(eventLabel, rowCounter, 0);
    rowCounter++;
  }
  this->setLayout(eventGrid);
}

EventWindow::~EventWindow() { delete ui; }
void EventWindow::readFile() {
  QString applicationPath =
      qApp->applicationDirPath() + "/BitsAndDroidsModule/modules/events.txt";
  qDebug() << applicationPath;
  std::ifstream file(applicationPath.toStdString());
  std::string row;
  std::string modeDelimiter = "^";
  std::string prefixDelimiter = "#";
  char updateEveryDelimiter = '$';
  char commentDelimiter = '/';
  while (std::getline(file, row)) {
    if (row.front() == ' ') {
      row.erase(0, 1);
    }
    if (row.size() > 25 && row.at(0) != '/') {
      auto newRow = new tableRow();
      newRow->prefix = row.substr(row.find(prefixDelimiter) + 1, 4);
      newRow->event = row.substr(0, row.find(modeDelimiter));
      tableRows.append(*newRow);
      qDebug() << row.c_str();
    }
  }
  file.close();
}
