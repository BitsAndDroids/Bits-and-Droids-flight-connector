#ifndef EVENTWINDOW_H
#define EVENTWINDOW_H

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
  Ui::EventWindow *ui;
  void readFile();
  bool eventFilter(QObject *obj, QEvent *e);
  bool checkIfRowChanged(int index);
  QTableWidget *eventTable = new QTableWidget();

  void writeFile();
  void fillRow(int index);
 private slots:
  void cellTextChanged(QTableWidgetItem *);
  void comboBoxChanged();
  void saveBtnPressed();
  void addEventBtnPressed();
};

#endif  // EVENTWINDOW_H
