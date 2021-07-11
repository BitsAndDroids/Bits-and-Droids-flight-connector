#ifndef OUTPUTMENU_H
#define OUTPUTMENU_H

#include <QSettings>
#include <QStandardPaths>
#include <QWidget>

namespace Ui {
class OutputMenu;
}

class OutputMenu : public QWidget
{
    Q_OBJECT

public:
    explicit OutputMenu(QWidget *parent = nullptr);
    ~OutputMenu();


public slots:
 void addNewSet();
 void removeSetAction(QString id);
 void editSet(QString id);
 void saveEdit();
signals:
 void addSet();
 void setEdited(QString id);
 void saveEdited();
 Q_SIGNAL void removeSet(QString id);
private:
    int activeSet;
    Ui::OutputMenu *ui;
    QString path =
        QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QSettings *settings =
        new QSettings(path + "/Bits and Droids/settings.ini", QSettings::IniFormat);
};

#endif // OUTPUTMENU_H
