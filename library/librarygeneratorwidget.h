#ifndef LIBRARYGENERATORWIDGET_H
#define LIBRARYGENERATORWIDGET_H

#include <QWidget>
#include <qgridlayout.h>

#include "ui_libraryGeneratorWindow.h"
namespace Ui {
class LibraryGeneratorWidget;
}
class LibraryGeneratorWidget : public QWidget
{
    Q_OBJECT
public:
    explicit LibraryGeneratorWidget(QWidget *parent = nullptr);
    QGridLayout* widgetGridLayout = new QGridLayout();
private:
    Ui::LibraryGeneratorWidget *libraryGeneratorWidget;

signals:

};

#endif // LIBRARYGENERATORWIDGET_H
