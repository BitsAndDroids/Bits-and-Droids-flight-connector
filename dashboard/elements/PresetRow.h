//
// Created by dave- on 20-11-2023.
//

#ifndef PRESETROW_H
#define PRESETROW_H
#include "ComPortWidget.h"
#include "dashboard/models/Preset.h"
#include "elements/Element.h"


class PresetRow: public QWidget, public Element {
    Q_OBJECT
public:
    PresetRow(QMainWindow* parent, Preset preset);

    QWidget* generateElement();

    void setActive();

private:
    QMainWindow *parent;
    Preset preset;
    QWidget *widget;
};



#endif //PRESETROW_H
