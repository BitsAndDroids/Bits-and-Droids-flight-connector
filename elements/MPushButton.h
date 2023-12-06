//
// Created by DaveRiedel on 18-8-2022.
//

#ifndef BITSANDDROIDSCONNECTOR_MPUSHBUTTON_H
#define BITSANDDROIDSCONNECTOR_MPUSHBUTTON_H


#include <QPushButton>
#include <QLayout>

#include "enums/ButtonStyleEnum.h"

class MPushButton: public QPushButton {
public:
    MPushButton(QString text, QLayout* layout, ButtonStyleEnum style);
    MPushButton(QLayout* layout, ButtonStyleEnum style);
    void setStyle(ButtonStyleEnum style);
    void setStyle();
    void setDefaulStyle();
    void setAnunciatorStyle();
    void setIconWithPath(const QString& path);
};


#endif //BITSANDDROIDSCONNECTOR_MPUSHBUTTON_H
