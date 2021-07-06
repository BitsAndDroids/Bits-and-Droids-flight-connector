#ifndef OUTPUTMENU_H
#define OUTPUTMENU_H

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

private:
    Ui::OutputMenu *ui;
};

#endif // OUTPUTMENU_H
