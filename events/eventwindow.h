#ifndef EVENTWINDOW_H
#define EVENTWINDOW_H
#include <handlers/pathhandler.h>
#include <qstandardpaths.h>

#include <QApplication>
#include <QTableWidgetItem>
#include <QWidget>

namespace Ui {
class EventWindow;
}

class EventWindow : public QWidget {
  Q_OBJECT

 public:
  explicit EventWindow(QWidget *parent = nullptr);
  ~EventWindow();

 private:
  PathHandler pathHandler;
  QString applicationEventsPath = pathHandler.getWritableEventPath();
  Ui::EventWindow *ui;
  void readFile();
  bool eventFilter(QObject *obj, QEvent *e);
  bool checkIfRowChanged(int index);
  QTableWidget *eventTable = new QTableWidget();

  void writeFile();
  void fillRow(int index);
  QString checkSpaces(std::string stringToCheck);
  void closeEvent(QCloseEvent *event) override;
 private slots:
  void cellTextChanged(QTableWidgetItem *);
  void comboBoxChanged();
  void saveBtnPressed();
  void addEventBtnPressed();
  void delChanged();

 signals:
  void closedEventWindow();
};

#endif  // EVENTWINDOW_H
