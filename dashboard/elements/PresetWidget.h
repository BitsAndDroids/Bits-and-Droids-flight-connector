//
// Created by dave- on 20-11-2023.
//

#ifndef PRESETWIDGET_H
#define PRESETWIDGET_H
#include <qboxlayout.h>
#include <QMainWindow>
#include <qtmetamacros.h>
#include <QWidget>

#include "dashboard/controller/PresetWidgetController.h"
#include "elements/Element.h"


class PresetWidget : public QWidget, public Element {
    Q_OBJECT
public:
    PresetWidget(const QWidget* parent);

    PresetWidget(QWidget* parent);

    QWidget* generateElement();

    std::vector<Preset> loadPresets();

    void generatePresetRows(QVBoxLayout* layout);

private:
    const PresetWidgetController * controller = nullptr;
    QWidget * parent = nullptr;


};


#endif //PRESETWIDGET_H
