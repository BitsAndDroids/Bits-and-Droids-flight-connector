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
