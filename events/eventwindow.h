#ifndef EVENTWINDOW_H
#define EVENTWINDOW_H

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
};

#endif  // EVENTWINDOW_H
