#include "eventwindow.h"

#include <qcombobox.h>
#include <qstring.h>

#include <QGridLayout>
#include <QGroupBox>
#include <QHeaderView>
#include <QLabel>
#include <QPushButton>
#include <QString>
#include <QTableWidget>
#include <fstream>
#include <iostream>

#include "ui_eventwindow.h"

using namespace std;

struct tableRow {
  std::string prefix;
  std::string event;
  std::string type;
  std::string updateEvery;
  std::string comment;
};

QList<tableRow> tableRows = QList<tableRow>();

EventWindow::EventWindow(QWidget *parent)
    : QWidget(parent), ui(new Ui::EventWindow) {
  ui->setupUi(this);
  QStringList headers = {"Prefix", "Event", "Type", "Update every", "Comment"};
  auto eventGrid = new QGridLayout();
  auto eventTable = new QTableWidget();
  readFile();

  eventTable->setRowCount(tableRows.size() + 1);
  eventTable->setColumnCount(headers.size());
  eventTable->setHorizontalHeaderLabels(headers);
  eventTable->verticalHeader()->setVisible(false);

  int rowCounter = 0;

  for (auto &row : tableRows) {
    auto prefixCell = new QTableWidgetItem(
        QString::fromStdString(row.prefix).toStdString().c_str());
    auto eventCell = new QTableWidgetItem(QString::fromStdString(row.event));
    auto typeCell = new QTableWidgetItem(QString::fromStdString(row.type));
    auto updateEveryCell =
        new QTableWidgetItem(QString::fromStdString(row.updateEvery));
    auto commentCell =
        new QTableWidgetItem(QString::fromStdString(row.comment));
    auto typeComboBox = new QComboBox();
    typeComboBox->addItems({"Input", "Input with value", "Axis", "Output"});
    switch (stoi(row.type)) {
      case 0:
        typeComboBox->setCurrentIndex(0);
        break;
      case 1:
        typeComboBox->setCurrentIndex(1);
        break;
      case 3:
        typeComboBox->setCurrentIndex(3);
        break;
      case 9:
        typeComboBox->setCurrentIndex(2);
        break;
    }
    typeComboBox->setFocusPolicy(Qt::StrongFocus);
    typeComboBox->installEventFilter(this);
    eventTable->setItem(rowCounter, 0, prefixCell);
    eventTable->setItem(rowCounter, 1, eventCell);
    eventTable->setCellWidget(rowCounter, 2, typeComboBox);
    eventTable->setItem(rowCounter, 3, updateEveryCell);
    eventTable->setItem(rowCounter, 4, commentCell);

    rowCounter++;
  }
  eventGrid->addWidget(eventTable);
  this->setLayout(eventGrid);
}
bool EventWindow::eventFilter(QObject *obj, QEvent *e) {
  if (e->type() == QEvent::Wheel) {
    QComboBox *combo = qobject_cast<QComboBox *>(obj);
    if (combo && !combo->hasFocus()) return true;
  }

  return false;
}
EventWindow::~EventWindow() { delete ui; }
void EventWindow::readFile() {
  QString applicationPath =
      qApp->applicationDirPath() + "/BitsAndDroidsModule/modules/events.txt";
  qDebug() << applicationPath;
  std::ifstream file(applicationPath.toStdString());
  std::string row;

  while (std::getline(file, row)) {
    int modeDelimiter = row.find("^");
    int prefixDelimiter = row.find("#");
    int updateEveryDelimiter = row.find("$");
    int commentDelimiter = row.find("//");
    if (row.front() == ' ') {
      row.erase(0, 1);
    }
    if (row.size() > 25 && row.at(0) != '/') {
      auto newRow = new tableRow();
      newRow->prefix = row.substr(prefixDelimiter + 1, 4);
      newRow->event = row.substr(0, modeDelimiter);
      newRow->type = row.substr(modeDelimiter + 1, 1);

      newRow->updateEvery =
          row.substr(updateEveryDelimiter + 1,
                     commentDelimiter - updateEveryDelimiter - 1);
      newRow->comment = row.substr(commentDelimiter + 2);

      tableRows.append(*newRow);
      qDebug() << newRow->comment.c_str();
    }
  }
  file.close();
}
