#ifndef CALIBRATEAXISMENU_H
#define CALIBRATEAXISMENU_H

#include "formBuilder.h"

#include <QWidget>
#include <qgridlayout.h>

namespace Ui {
class CalibrateAxisMenu;
}

class CalibrateAxisMenu : public QWidget
{
    Q_OBJECT

public:
    explicit CalibrateAxisMenu(QWidget *parent = nullptr);
    ~CalibrateAxisMenu();

private:
    Ui::CalibrateAxisMenu *ui;
    QStringList curves = {"Rudder", "Toe brakes", "Aileron","Elevator"};
    QGridLayout *GenerateCurveLayouts();
    void saveSettings();
    FormBuilder* builder = new FormBuilder();
    SettingsHandler settingsHandler = SettingsHandler();
};

#endif // CALIBRATEAXISMENU_H
