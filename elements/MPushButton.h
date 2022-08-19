//
// Created by DaveRiedel on 18-8-2022.
//

#ifndef BITSANDDROIDSCONNECTOR_MPUSHBUTTON_H
#define BITSANDDROIDSCONNECTOR_MPUSHBUTTON_H


#include <QPushButton>
#include <QLayout>
class MPushButton: public QPushButton {
public:
    MPushButton(QString text, QLayout* layout);
    MPushButton(QLayout* layout);
    void setIconWithPath(QString path);
};


#endif //BITSANDDROIDSCONNECTOR_MPUSHBUTTON_H
