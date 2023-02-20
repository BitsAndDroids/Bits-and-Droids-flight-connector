//
// Created by DaveRiedel on 18-8-2022.
//

#ifndef BITSANDDROIDSCONNECTOR_OUTPUTTABS_H
#define BITSANDDROIDSCONNECTOR_OUTPUTTABS_H


#include <QWidget>
#include <QTabWidget>

class OutputTabs: public QWidget{
Q_OBJECT
private:
    QTabWidget* outputTabsWidget = new QTabWidget();
    QWidget* parent;
public:
    explicit OutputTabs();
    ~OutputTabs() override;
};




#endif //BITSANDDROIDSCONNECTOR_OUTPUTTABS_H
