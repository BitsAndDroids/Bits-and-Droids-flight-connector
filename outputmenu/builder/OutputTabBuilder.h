//
// Created by DaveRiedel on 18-8-2022.
//

#ifndef BITSANDDROIDSCONNECTOR_OUTPUTTABBUILDER_H
#define BITSANDDROIDSCONNECTOR_OUTPUTTABBUILDER_H


#include <QWidget>

class OutputTabBuilder: public QWidget {
Q_OBJECT
private:
public:
    OutputTabBuilder(QWidget *parent = nullptr);
    QWidget* buildOutputTabContainer();
};


#endif //BITSANDDROIDSCONNECTOR_OUTPUTTABBUILDER_H
