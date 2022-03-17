#ifndef LIBRARYGENERATORWINDOW_H
#define LIBRARYGENERATORWINDOW_H

#include <QWidget>
#include <qgridlayout.h>

namespace Ui {
class LibraryGeneratowWindow;
}

class LibraryGeneratowWindow : public QWidget
{
    Q_OBJECT

public:
    explicit LibraryGeneratowWindow(QWidget *parent = nullptr);
    ~LibraryGeneratowWindow();

private:
    Ui::LibraryGeneratowWindow *ui;
    QGridLayout* windowGridLayout = new QGridLayout();
};

#endif // LIBRARYGENERATORWINDOW_H
