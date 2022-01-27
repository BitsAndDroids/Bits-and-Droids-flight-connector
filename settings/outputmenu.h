#ifndef OUTPUTMENU_H
#define OUTPUTMENU_H

#include <outputs/outputhandler.h>
#include <outputs/sethandler.h>

#include <QSettings>
#include <QStandardPaths>
#include <QWidget>

#include "formBuilder.h"
#include "settingshandler.h"

namespace Ui {
class OutputMenu;
}

class OutputMenu : public QWidget {
  Q_OBJECT

 public:
  explicit OutputMenu(QWidget *parent = nullptr);

  ~OutputMenu();

  bool isOpen() { return open; }

 public slots:

  void addNewSet();

  void removeSetAction(QString id);

  void editSet(QString id);

  void saveEdit();

 signals:

  void closedOutputMenu();

  void addSet();

  void setEdited(QString id);

  void saveEdited();

  Q_SIGNAL void removeSet(QString id);

 private:
  bool open = false;
  int activeSet;
  SettingsHandler settingsHandler;
  SetHandler setHandler;
  FormBuilder formBuilder;

  outputHandler *outputHandler = new class outputHandler();
  Ui::OutputMenu *ui;
  QString path =
      QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
  QSettings *settings = new QSettings(path + "/Bits and Droids/settings.ini",
                                      QSettings::IniFormat);

  void closeEvent(QCloseEvent *event) override;
};

#endif  // OUTPUTMENU_H
