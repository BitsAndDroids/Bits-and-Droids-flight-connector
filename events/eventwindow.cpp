
#include "eventwindow.h"

#include <events/Event.h>
#include <events/EventDataType.h>
#include <events/EventType.h>
#include <qcheckbox.h>
#include <qcombobox.h>
#include <qfile.h>
#include <qstring.h>

#include <QFontDatabase>
#include <QGridLayout>
#include <QHeaderView>
#include <QLabel>
#include <QMessageBox>
#include <QPushButton>
#include <fstream>
#include <iostream>

#include "logging/MessageCaster.h"
#include "ui_eventwindow.h"

using namespace std;
namespace Utils {
std::string padLeft(std::string source, std::size_t totalWidth,
                    char paddingChar = ' ') {
  if (totalWidth > source.size()) {
    source.insert(0, totalWidth - source.size(), paddingChar);
  }
  return source;
}
}  // namespace Utils

QList<Event> tableRows = QList<Event>();
QList<int> tableRowsToDelete = QList<int>();
QList<int> rowsChanged = QList<int>();

EventWindow::EventWindow(QWidget *parent)
    : QWidget(parent), ui(new Ui::EventWindow) {
  ui->setupUi(this);
  this->setWindowTitle("Event editor");
  QStringList headers = {"Prefix",       "Event",   "Type",  "Datatype",
                         "Update every", "Comment", "Remove"};
  auto eventGrid = new QGridLayout();
  tableRows.clear();
  tableRowsToDelete.clear();
  rowsChanged.clear();
  readFile();

  QFont Triforce("Roboto Black", 11, 900);
  eventTable->horizontalHeader()->setFont(Triforce);
  eventTable->setRowCount((int)tableRows.size());
  eventTable->setColumnCount((int)headers.size());
  eventTable->setHorizontalHeaderLabels(headers);

  eventTable->verticalHeader()->setVisible(false);
  eventTable->setColumnWidth(0, 50);
  eventTable->setColumnWidth(1, 450);
  eventTable->setColumnWidth(5, 250);
  eventTable->setColumnWidth(6, 80);
  eventTable->setMinimumWidth(1150);
  eventTable->setMinimumHeight(490);
  eventTable->setSortingEnabled(true);

  int rowCounter = 0;
  this->setStyleSheet("background:white;");

  for (auto &row : tableRows) {
    fillRow(rowCounter);
    rowCounter++;
  }
  connect(eventTable, &QTableWidget::itemChanged, this,
          &EventWindow::cellTextChanged);

  eventGrid->addWidget(eventTable, 0, 0, 1, 2);

  auto saveBtn = new QPushButton("Save");
  auto addBtn = new QPushButton("Add event");
  connect(addBtn, &QPushButton::pressed, this,
          &EventWindow::addEventBtnPressed);
  connect(saveBtn, &QPushButton::pressed, this, &EventWindow::saveBtnPressed);

  addBtn->setFont(Triforce);

  auto *btnRow = new QHBoxLayout();
  eventGrid->addLayout(btnRow, 1, 1);
  btnRow->addWidget(addBtn);
  addBtn->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
  saveBtn->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

  btnRow->addWidget(saveBtn);
  addBtn->setStyleSheet(
      "background-color:#0f4c5c;"
      "border-radius:3px;"
      "color:white;"
      "font-size: 2.44em;"
      "line-height: 1.4;"
      "font-weight:bold;"
      "min-height:24px;"
      "padding:5px;");
  saveBtn->setFont(Triforce);
  saveBtn->setStyleSheet(
      "background-color:#509402;"
      "border-radius:3px;"
      "color:white;"
      "font-size: 2.44em;"
      "line-height: 1.4;"
      "font-weight:bold;"
      "min-height:24px;"
      "padding:5px;");
  this->setLayout(eventGrid);
  eventTable->setFrameStyle(QFrame::NoFrame);

  this->adjustSize();
}

void EventWindow::fillRow(int index) {
  eventTable->blockSignals(true);

  QFont Triforce("Roboto", 10, 400);
  auto prefixCell =
      new QTableWidgetItem(QString::number(tableRows[index].getPrefix()));

  auto eventCell =
      new QTableWidgetItem(QString::fromStdString(tableRows[index].getEvent()));
  auto updateEveryCell = new QTableWidgetItem();
  if (tableRows[index].getType().getTypeIndex() == 3) {
    updateEveryCell->setText(
        QString::number(tableRows[index].getUpdateEvery()));
  } else {
    updateEveryCell->setFlags(Qt::ItemIsSelectable);
    updateEveryCell->setText("n/a");
  }
  auto commentCell = new QTableWidgetItem(
      QString::fromStdString(tableRows[index].getComment()));
  auto typeComboBox = new QComboBox();
  typeComboBox->setObjectName("cbt" + QString::number(index));

  typeComboBox->addItems(EventType::getAvailableTypes());
  typeComboBox->setCurrentIndex(tableRows[index].getType().getTypeIndex());

  auto dataTypeCombobox = new QComboBox();
  dataTypeCombobox->setObjectName("cbdt" + QString::number(index));

  dataTypeCombobox->addItems(EventDatatype::getTypeLibraryParse());
  dataTypeCombobox->setCurrentIndex(
      tableRows[index]
          .getDatatype()
          .getDatatypeIndex());  // NOLINT(cppcoreguidelines-narrowing-conversions)

  dataTypeCombobox->setObjectName("cbdt" + QString::number(index));

  auto *naDatatype = new QTableWidgetItem("n/a");
  naDatatype->setFlags(Qt::ItemIsSelectable);
  prefixCell->setFont(Triforce);
  eventCell->setFont(Triforce);
  commentCell->setFont(Triforce);
  updateEveryCell->setFont(Triforce);

  typeComboBox->setFocusPolicy(Qt::StrongFocus);
  typeComboBox->installEventFilter(this);
  auto *removeToggle = new QCheckBox();
  removeToggle->setStyleSheet("margin-left:50%; margin-right:50%;");
  removeToggle->setObjectName("del" + QString::number(index));
  connect(removeToggle, &QCheckBox::stateChanged, this,
          &EventWindow::delChanged);
  connect(typeComboBox, &QComboBox::currentIndexChanged, this,
          &EventWindow::comboBoxChanged);
  eventTable->setItem(index, 0, prefixCell);
  eventTable->setItem(index, 1, eventCell);
  eventTable->setCellWidget(index, 2, typeComboBox);
  if (tableRows[index].getType().getTypeIndex() == 3) {
    eventTable->setCellWidget(index, 3, dataTypeCombobox);
  } else {
    eventTable->setItem(index, 3, naDatatype);
  }

  eventTable->setItem(index, 4, updateEveryCell);
  eventTable->setItem(index, 5, commentCell);
  eventTable->setCellWidget(index, 6, removeToggle);
  eventTable->blockSignals(false);
}

void EventWindow::delChanged() {
  auto sendCb = qobject_cast<QCheckBox *>(sender());

  int index = sendCb->objectName().mid(3).toInt();
  if (sendCb->checkState() == Qt::Checked) {
    tableRowsToDelete.append(index);
  } else {
    tableRowsToDelete.remove(tableRowsToDelete.indexOf(index));
  }
}

void EventWindow::cellTextChanged(QTableWidgetItem *changedItem) {
  std::string textToCompare;
  changedItem->tableWidget()->blockSignals(true);
  switch (changedItem->column()) {
    case 0:
      textToCompare = std::to_string(tableRows[changedItem->row()].getPrefix());
      break;
    case 1:
      textToCompare = tableRows[changedItem->row()].getEvent();
      break;

    case 3:
      textToCompare =
          std::to_string(tableRows[changedItem->row()].getUpdateEvery());
      break;
    case 4:
      textToCompare = tableRows[changedItem->row()].getComment();
      break;
  }

  QFont font;
  if (textToCompare != changedItem->text().toStdString()) {
    changedItem->setForeground(QColor(1, 150, 11));

    font.setBold(true);

  } else {
    font.setBold(false);
    changedItem->setForeground(QColor(0, 0, 0));
  }

  changedItem->setFont(font);
  changedItem->tableWidget()->blockSignals(false);
  if (checkIfRowChanged(changedItem->row())) {
    if (rowsChanged.indexOf(changedItem->row()) == -1) {
      rowsChanged.append(changedItem->row());
      cout << "Changed" << changedItem->row() << endl;
    }
  }
  cout << changedItem->text().toStdString() << "CHANGED" << endl;
}

void EventWindow::addEventBtnPressed() {
  auto *newEvent = new Event();
  eventTable->setRowCount(eventTable->rowCount() + 1);
  newEvent->setPrefix(0);
  newEvent->setEvent("Your event goes here");
  newEvent->setComment("Comments go here");
  newEvent->setUpdateEvery(0);
  newEvent->setType(EventType(0));
  rowsChanged.append((int)tableRows.size());
  tableRows.append(*newEvent);
  fillRow((int)tableRows.size() - 1);
}

void EventWindow::saveBtnPressed() {
  // A row gets flagged for change when the data != the first found state
  // If the value changes from default -> changed value -> the default value
  // We want to remove the changed flag
  for (auto &changed : rowsChanged) {
    if (!checkIfRowChanged(changed)) {
      rowsChanged.removeAt(rowsChanged.indexOf(changed));
      cout << "ROW REMOVED" << endl;
    }
  }
  auto saveMessageBox = new QMessageBox();
  if (!rowsChanged.empty() || !tableRowsToDelete.empty()) {
    saveMessageBox->setText(
        QString::number(rowsChanged.size() + tableRowsToDelete.size()) +
        " Changes have been found");
    saveMessageBox->setInformativeText("Do you want to save these changes?");
    QString detailedText = "";

    for (auto &toRemove : tableRowsToDelete) {
      detailedText += "DELETE: ";
      detailedText += QString::number(tableRows[toRemove].getPrefix()) + " ";
      detailedText +=
          QString::fromStdString(tableRows[toRemove].getEvent()) + "\n";
    }

    for (auto &changed : rowsChanged) {
      detailedText +=
          "OLD VALUE: " + QString::number(tableRows[changed].getPrefix());
      detailedText +=
          (" " + QString::fromStdString(tableRows[changed].getEvent()));
      detailedText +=
          (" " + QString::fromStdString(tableRows[changed].getComment()));
      detailedText += " -> NEW VALUE: " + eventTable->item(changed, 0)->text();
      detailedText += (" " + eventTable->item(changed, 1)->text());
      detailedText += (" " + eventTable->item(changed, 5)->text() + "\n");
    }

    saveMessageBox->setDetailedText(detailedText);
    saveMessageBox->setStandardButtons(QMessageBox::Save | QMessageBox::Cancel);

    auto *horizontalSpacer =
        new QSpacerItem(600, 0, QSizePolicy::Minimum, QSizePolicy::Expanding);
    auto *layout = (QGridLayout *)saveMessageBox->layout();
    layout->addItem(horizontalSpacer, layout->rowCount(), 0, 1,
                    layout->columnCount());
  } else {
    saveMessageBox->setText("No changes have been made");
  }

  int ret = saveMessageBox->exec();

  if (ret == QMessageBox::Save) {
    for (auto &changedRow : rowsChanged) {
      auto row = tableRows[changedRow];

      tableRows[changedRow].setPrefix(
          eventTable->item(changedRow, 0)->text().toInt());
      tableRows[changedRow].setEvent(
          eventTable->item(changedRow, 1)->text().toStdString());
      tableRows[changedRow].setComment(
          eventTable->item(changedRow, 5)->text().toStdString());

      tableRows[changedRow].setUpdateEvery(
          eventTable->item(changedRow, 4)->text().toFloat());
      tableRows[changedRow].setType(EventType(
          this->findChild<QComboBox *>("cbt" + QString::number(changedRow))
              ->currentIndex()));

      if (tableRows[changedRow].getType().getTypeIndex() == 3) {
        tableRows[changedRow].setDatatype(EventDatatype(
            this->findChild<QComboBox *>("cbdt" + QString::number(changedRow))
                ->currentIndex()));
      }
    }
    for (auto &del : tableRowsToDelete) {
      eventTable->removeRow(del);
      tableRows.removeAt(del);
    }
    writeFile();
  }
}

void EventWindow::writeFile() {
  QFile newEventsFile(applicationEventsPath);
  cout << "WRITEFILE " << newEventsFile.fileName().toStdString().c_str()
       << endl;
  newEventsFile.open(QIODevice::ReadWrite);
  newEventsFile.resize(0);
  QTextStream out(&newEventsFile);
  for (auto &row : tableRows) {
    if (row.getUpdateEvery() < 0) {
      row.setUpdateEvery(0);
    }
    QString typeString;
    int typeIndex = row.getType().getTypeIndex();
    if (typeIndex == 3) {
      QString eventType =
          QString::number(EventType::getEventCodes().at(typeIndex));
      QString dataType = EventDatatype::getTypeLibraryParseShorthand().at(
          row.getDatatype().getDatatypeIndex());
      typeString = eventType + dataType;

    } else {
      typeString = QString::number(row.getType().getTypeIndex());
    }
    QString formattedAxis = "";
    if (row.getType().getTypeIndex() == 9) {
      formattedAxis = "-0+1023";
    }
    QString formattedEvent = checkSpaces(row.getEvent());
    out << formattedEvent << "^" << typeString
        << "#" + QString::fromStdString(
                     Utils::padLeft(std::to_string(row.getPrefix()), 4))
        << formattedAxis << "$" << QString::number(row.getUpdateEvery()) << "//"
        << QString::fromStdString(row.getComment())

        << "\n";
  }
  newEventsFile.close();
  QFile::remove(pathHandler.getCommunityFolderPath() +
                "/BitsAndDroidsModule/modules/events.txt");
  QFile::copy(applicationEventsPath,
              pathHandler.getCommunityFolderPath() +
                  "/BitsAndDroidsModule/modules/events.txt");
}

QString EventWindow::checkSpaces(std::string stringToCheck) {
  std::string charsToFind[] = {"+", "-", "/", "*", "(", ")"};
  for (auto &c : charsToFind) {
    size_t pos = stringToCheck.find(c);
    while (pos != std::string::npos) {
      if (pos < stringToCheck.size() - 1 && stringToCheck[pos + 1] != ' ' &&
          c != "(") {
        stringToCheck.insert(pos + 1, " ");
      }
      if (pos > 0 && stringToCheck[pos - 1] != ' ' && c != ")") {
        stringToCheck.insert(pos, " ");
      }

      pos = stringToCheck.find(c, pos + c.size());
    }
  }

  return QString::fromStdString(stringToCheck);
}

void EventWindow::comboBoxChanged() {
  auto senderComboBox = qobject_cast<QComboBox *>(sender());
  int index = stoi(senderComboBox->objectName().toStdString().substr(3));
  if (checkIfRowChanged(index)) {
    if (rowsChanged.indexOf(index) == -1) {
      rowsChanged.append(index);
    }
  }
  if (senderComboBox->currentIndex() == 3) {
    auto dataTypeCombobox = new QComboBox();
    dataTypeCombobox->setObjectName("cbt" + QString::number(index));

    dataTypeCombobox->addItems(EventDatatype::getTypeLibrary());
    dataTypeCombobox->setCurrentIndex(
        tableRows[index].getDatatype().getDatatypeIndex());

    dataTypeCombobox->setObjectName("cbdt" + QString::number(index));
    eventTable->setCellWidget(index, 3, dataTypeCombobox);
    eventTable->removeCellWidget(index, 4);
    auto *emptyUpdateEvery = new QTableWidgetItem("0");
    eventTable->setItem(index, 4, emptyUpdateEvery);

  } else {
    eventTable->removeCellWidget(index, 3);
    auto *naTextWidget = new QTableWidgetItem("n/a");
    naTextWidget->setFlags(Qt::ItemIsSelectable);
    eventTable->setItem(index, 3, naTextWidget);
    eventTable->setItem(index, 4, naTextWidget);
  }
}

bool EventWindow::eventFilter(QObject *obj, QEvent *e) {
  if (e->type() == QEvent::Wheel) {
    auto *combo = qobject_cast<QComboBox *>(obj);
    if (combo && !combo->hasFocus()) return true;
  }

  return false;
}

bool EventWindow::checkIfRowChanged(int index) {
  std::array<bool, 3> checks = {
      (tableRows[index].getPrefix() ==
       eventTable->item(index, 0)->text().toInt()),
      tableRows[index].getEvent() ==
          eventTable->item(index, 1)->text().toStdString(),
      //      tableRows[index].updateEvery ==
      //        //                eventTable->item(index,
      //        3)->text().toStdString()== 0,
      tableRows[index].getComment() ==
          eventTable->item(index, 4)->text().toStdString()};

  int typeIndex = this->findChild<QComboBox *>("cbt" + QString::number(index))
                      ->currentIndex();

  if (tableRows[index].getType().getTypeIndex() != typeIndex) {
    return true;
  }

  return !(std::all_of(checks.begin(), checks.end(), [](bool b) {
    std::cout << "oneFalse" << std::endl;
    return !b;
  }));
}

EventWindow::~EventWindow() {
  emit EventWindow::closedEventWindow();
  delete ui;
}

void EventWindow::closeEvent(QCloseEvent *event) {
  emit EventWindow::closedEventWindow();
  delete ui;
}

void EventWindow::readFile() {
  std::ifstream file(applicationEventsPath.toStdString());
  std::string row;
  int index = 0;
  while (std::getline(file, row)) {
    index++;
    try {
      if (row.size() > 25 && row.at(0) != '/') {
        int modeDelimiter = (int)row.find('^');
        int prefixDelimiter = (int)row.find('#');
        int updateEveryDelimiter = (int)row.find('$');
        int commentDelimiter = (int)row.find("//");
        if (row.front() == ' ') {
          row.erase(0, 1);
        }

        auto newRow = new Event();
        newRow->setPrefix(stoi(row.substr(prefixDelimiter + 1, 4)));
        newRow->setEvent(row.substr(0, modeDelimiter));
        newRow->setType(EventType(stoi(row.substr(modeDelimiter + 1, 1))));
        if (newRow->getType().getTypeIndex() == 3)
          newRow->setDatatype(EventDatatype(
              (int)EventDatatype::getTypeLibraryParseShorthand().indexOf(
                  QString::fromStdString(row.substr(modeDelimiter + 2, 1)))));

        newRow->setUpdateEvery(
            stof(row.substr(updateEveryDelimiter + 1,
                            commentDelimiter - updateEveryDelimiter - 1)));
        newRow->setComment(row.substr(commentDelimiter + 2));

        tableRows.append(*newRow);
        qDebug() << newRow->getComment().c_str();
      }
    } catch (const std::exception &e) {
      //#TODO write to logger
      auto errorString = "Event file error in line " + QString::number(index) +
                         "\n" + QString::fromStdString(row) + "\nReason(" +
                         e.what() + " failed)";
      MessageCaster::showWarningMessage(errorString);
    }
  }
  file.close();
}
