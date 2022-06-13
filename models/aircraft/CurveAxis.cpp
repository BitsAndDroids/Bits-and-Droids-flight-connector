//
// Created by DaveRiedel on 13-Jun-22.
//

#include "CurveAxis.h"

CurveAxis::CurveAxis(InputEnum::DATA_DEFINE_ID_INPUT event, const QList<coordinates> &curve)
        : Axis(event), curve(curve) {}

void CurveAxis::clearCurve() {
    this->curve.clear();
}

void CurveAxis::setCurve(QList<coordinates> &curveToSet) {
    this->curve = curveToSet;
}

coordinates CurveAxis::getCoordinates(int index) {
    return curve.at(index);
}

CurveAxis::CurveAxis(InputEnum::DATA_DEFINE_ID_INPUT event) : Axis(event) {
    QList<coordinates> defaultCurve = {
            {coordinates(0, -16383)},
            {coordinates(250, -10000)},
            {coordinates(500, 0)},
            {coordinates(511, 0)},
            {coordinates(522, 0)},
            {coordinates(750, 10000)},
            {coordinates(1023, 16383)}};
    this->curve = defaultCurve;
}
