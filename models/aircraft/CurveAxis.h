//
// Created by DaveRiedel on 13-Jun-22.
//

#ifndef BITSANDDROIDSCONNECTOR_CURVEAXIS_H
#define BITSANDDROIDSCONNECTOR_CURVEAXIS_H


#include <QList>
#include "models/settings/coordinates.h"
#include "Axis.h"

class CurveAxis: public Axis {


    QList<coordinates> curve = {
            {coordinates(0, -16383)},
            {coordinates(250, -10000)},
            {coordinates(500, 0)},
            {coordinates(511, 0)},
            {coordinates(522, 0)},
            {coordinates(750, 10000)},
            {coordinates(1023, 16383)}};
public:
    CurveAxis(InputEnum::DATA_DEFINE_ID_INPUT event, const QList<coordinates> &curve);
    CurveAxis(InputEnum::DATA_DEFINE_ID_INPUT event);
    void clearCurve();
    void setCurve(QList<coordinates> &);
    coordinates getCoordinates(int index);
};


#endif //BITSANDDROIDSCONNECTOR_CURVEAXIS_H
