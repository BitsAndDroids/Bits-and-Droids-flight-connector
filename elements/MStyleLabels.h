//
// Created by DaveRiedel on 18-8-2022.
//

#ifndef BITSANDDROIDSCONNECTOR_MSTYLELABELS_H
#define BITSANDDROIDSCONNECTOR_MSTYLELABELS_H

#include <enums/StyleEnum.h>
#include <QLabel>

class MStyleLabels: public QLabel {
private:

public:
    MStyleLabels(QString text, StyleEnum style);

};


#endif //BITSANDDROIDSCONNECTOR_MSTYLELABELS_H
