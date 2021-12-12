#ifndef CALIBRATEAXISMENU_H
#define CALIBRATEAXISMENU_H

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
};

#endif // CALIBRATEAXISMENU_H
