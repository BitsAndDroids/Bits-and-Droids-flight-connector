#ifndef CALIBRATEAXISMENU_H
#define CALIBRATEAXISMENU_H

#include <qgridlayout.h>

#include <QWidget>

#include "formBuilder.h"

namespace Ui {
class CalibrateAxisMenu;
}

class CalibrateAxisMenu : public QWidget {
  Q_OBJECT

 public:
  explicit CalibrateAxisMenu(QWidget *parent = nullptr);
  ~CalibrateAxisMenu();

 private:
  Ui::CalibrateAxisMenu *ui;
  QStringList curves = {"Rudder", "Toe brakes", "Aileron", "Elevator"};
  QGridLayout *GenerateCurveLayouts();
  void saveSettings();
  FormBuilder *builder = new FormBuilder();
  SettingsHandler settingsHandler = SettingsHandler();

  void closeEvent(QCloseEvent *event) override;
 signals:
  void closedCalibrateAxisMenu();
};

#endif  // CALIBRATEAXISMENU_H
