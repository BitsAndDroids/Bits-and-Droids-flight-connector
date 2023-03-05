//
// Created by DaveRiedel on 18-8-2022.
//

#ifndef BITSANDDROIDSCONNECTOR_OUTPUTTABS_H
#define BITSANDDROIDSCONNECTOR_OUTPUTTABS_H


#include <QWidget>
#include <QTabWidget>
#include <qwidget.h>

class OutputTabs: public QWidget{
Q_OBJECT
private:
    QTabWidget* outputTabsWidget = new QTabWidget();
    QWidget* parent;
public:
    explicit OutputTabs(QWidget* parent);
    ~OutputTabs() override;
};




#endif //BITSANDDROIDSCONNECTOR_OUTPUTTABS_H
