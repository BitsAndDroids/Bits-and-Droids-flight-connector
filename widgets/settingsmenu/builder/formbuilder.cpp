#include <QDebug>
#include "formbuilder.h"
#include "constants.h"
#include <qcombobox.h>
#include "models/settings/settingsranges.h"
#include <QtCharts>
#include <QtSerialPort>
#include <iostream>
#include <utility>
#include "elements/ModeIndexCheckbox.h"


using namespace std;

FormBuilder::FormBuilder() {
    for (int i = 0; i < constants::supportedEngines; i++) {
        engineHeaders.append("Engine " + QString::number(i + 1));
    }
    for (int i = 0; i < constants::supportedMixtureLevers; i++) {
        rangeHeaders.append("Mixture " + QString::number(i + 1));
    }
    for (int i = 0; i < constants::supportedPropellerLevers; i++) {
        rangeHeaders.append("Propeller " + QString::number(i + 1));
    }
    rangeHeaders.append("Flaps");

    qDebug() << "bye";
}

void FormBuilder::loadPointsToPlot(QStringList axis) {
    pointsToPlot.clear();

    for (int i = 0; i < axis.size(); i++) {
        auto coordsToPlot = new QList<coordinates>();
        pointsToPlot.append(*coordsToPlot);
        auto keys = settingsHandler.retrieveSubKeys(axis[i] + "Series", "axis");
        if (!keys->isEmpty()) {
            auto axisKeys =
                    settingsHandler.retrieveSubKeys(axis[i] + "Series", "axis");
            auto valueKeys =
                    settingsHandler.retrieveSubKeys(axis[i] + "Series", "value");
            for (int j = 0; j < axisKeys->size(); j++) {
                float x =
                        settingsHandler
                        .retrieveSubSetting(axis[i] + "Series", "axis", axisKeys->at(j))
                        ->toFloat();
                float y = settingsHandler
                        .retrieveSubSetting(axis[i] + "Series", "value",
                                            valueKeys->at(j))
                        ->toFloat();
                auto* coord = new coordinates(x, y);
                pointsToPlot[i].append(*coord);
            }
        }
    }
}

QVBoxLayout* FormBuilder::createAxisRow(QString name, int number) {
    auto* layout = new QVBoxLayout();
    layout->setObjectName(QString::number(number) + name + "CalibrateLayout");
    auto serie = new QLineSeries();
    serie->setName(QString::number(number) + "spline");
    series.append(serie);

    //*series << QPointF(11, 1) << QPointF(13, 3);

    charts.append(new QChart());
    charts[number]->legend()->hide();
    charts[number]->addSeries(serie);
    charts[number]->setTitle(name + " curve");

    // chart->createDefaultAxes();

    auto* xAxis = new QValueAxis();

    for (int i = 0; i < objectNames.size(); i++) {
        if (!settingsHandler
            .retrieveSubSetting(name + "Series", "calibrations",
                                objectNames.at(i))
            ->isNull()) {
            int valFound = settingsHandler
                    .retrieveSubSetting(name + "Series", "calibrations",
                                        objectNames.at(i))
                    ->toInt();
            switch (i) {
                case 0:
                    minValue[number] = valFound;
                    break;
                case 1:
                    neutralValue[number] = valFound;
                    break;
                case 2:
                    maxValue[number] = valFound;
                    break;
                default:
                    break;
            }
        }
    }
    xAxis->setRange(minValue[number], maxValue[number]);
    xAxis->setLabelFormat("%i");

    auto* yAxis = new QValueAxis();
    yAxis->setRange(-16383, 16383);
    yAxis->setLabelFormat("%i");
    auto newChart = new QChart();

    charts.append(newChart);

    charts[number]->addAxis(xAxis, Qt::AlignBottom);

    charts[number]->addAxis(yAxis, Qt::AlignLeft);
    auto newChartView = new QChartView(charts[number]);
    chartViews.append(newChartView);
    newChartView->setObjectName(QString::number(number) + name + "ChartView");
    newChartView->setMinimumSize(350, 250);
    newChartView->setMaximumSize(350, 250);
    newChartView->adjustSize();

    newChartView->setRenderHint(QPainter::Antialiasing);
    layout->addWidget(newChartView);
    int sliders = 3;
    QStringList sliderLabels = {"Deadzone", "Sensitivity -", "Sensitivity +"};
    QStringList sliderNames = {
        QString::number(number) + name + "Deadzone",
        QString::number(number) + name + "MinSensitivity",
        QString::number(number) + name + "PlusSensitivity"
    };
    for (int i = 0; i < sliders; i++) {
        auto layoutRow = new QHBoxLayout();

        auto slider = new QSlider(Qt::Horizontal);
        slider->setObjectName(sliderNames.at(i));
        connect(slider, &QAbstractSlider::valueChanged, this,
                &FormBuilder::changeSlider);
        slider->setRange(0, 100);
        auto sliderLabel = new QLabel(sliderLabels.at(i));
        layoutRow->addWidget(sliderLabel);
        layoutRow->addWidget(slider);
        layout->addLayout(layoutRow);
        layoutRow->setAlignment(Qt::AlignLeft);
    }
    QStringList rudderValueLabels = {"Min", "Neutral", "Max"};

    auto rudderValueRow = new QHBoxLayout();

    for (int i = 0; i < rudderValueLabels.size(); i++) {
        auto label = new QLabel(rudderValueLabels.at(i));
        auto lineEdit = new QLineEdit();
        switch (i) {
            case 0:
                lineEdit->setText(QString::number(minValue[number]));
                break;
            case 1:
                lineEdit->setText(QString::number(neutralValue[number]));
                break;
            case 2:
                lineEdit->setText(QString::number(maxValue[number]));
                break;
            default:
                break;
        }

        lineEdit->setObjectName(QString::number(number) + QString::number(i) +
                                name + objectNames.at(i));
        lineEdit->setMaximumWidth(50);
        connect(lineEdit, &QLineEdit::textEdited, this,
                &FormBuilder::rudderTextChanged);
        rudderValueRow->addWidget(label, 0, Qt::AlignLeft);
        rudderValueRow->addWidget(lineEdit, 0, Qt::AlignLeft);
    }
    auto reversedLabel = new QLabel("Reversed");
    auto rudderReversedCb = new QCheckBox();
    rudderReversedCb->setObjectName(QString::number(number) + name + "Reversed");
    connect(rudderReversedCb, &QCheckBox::clicked, this,
            &FormBuilder::reverseClicked);
    rudderValueRow->addWidget(reversedLabel);
    rudderValueRow->addWidget(rudderReversedCb);
    layout->addLayout(rudderValueRow);
    rudderValueRow->setAlignment(Qt::AlignLeft);
    int const min = -16383;
    int const max = 16383;

    if (pointsToPlot[number].isEmpty()) {
        auto* coords = new QList<coordinates>{
            {coordinates(static_cast<float>(minValue[number]), min)},
            {
                coordinates(static_cast<float>(neutralValue[number]) -
                            static_cast<float>(
                                (maxValue[number] - neutralValue[number]) / 2),
                            axisValues[1])
            },
            {coordinates(500, axisValues[2])},
            {coordinates(static_cast<float>(neutralValue[number]), axisValues[2])},
            {coordinates(522, axisValues[2])},
            {
                coordinates(static_cast<float>(neutralValue[number]) +
                            static_cast<float>(
                                (maxValue[number] - neutralValue[number]) / 2),
                            axisValues[5])
            },
            {coordinates(static_cast<float>(maxValue[number]), max)}
        };

        for (auto&i: *coords) {
            pointsToPlot[number].append(i);
            series[number]->append(i.getX(), i.getY());
        }
    }
    else {
        for (auto&i: pointsToPlot[number]) {
            series[number]->append(i.getX(), i.getY());
        }
    }
    // layout->addLayout(curveControls);
    series[number]->attachAxis(yAxis);
    series[number]->attachAxis(xAxis);
    return layout;
}

void FormBuilder::rudderTextChanged() {
    auto senderLineEdit = qobject_cast<QLineEdit *>(sender());
    int valueToChange = senderLineEdit->text().toInt();
    int index = QString(senderLineEdit->objectName().at(1)).toInt();
    int table = senderLineEdit->objectName().first(1).toInt();
    qDebug() << table << " table + "
            << "Got" << index << " " << senderLineEdit->objectName();
    switch (index) {
        case 0:
            pointsToPlot[table][0].setX(static_cast<float>(valueToChange));
            minValue[table] = valueToChange;
            break;
        case 1:
            pointsToPlot[table][3].setX(static_cast<float>(valueToChange));
            neutralValue[table] = valueToChange;
            break;
        case 2:
            pointsToPlot[table][6].setX(static_cast<float>(valueToChange));
            maxValue[table] = valueToChange;
            break;
        default:
            break;
    }
    updateChart(index);
}

void FormBuilder::reverseClicked() {
    auto sendCb = qobject_cast<QCheckBox *>(sender());
    int number = sendCb->objectName().first(1).toInt();
    for (int i = 0; i < pointsToPlot[number].size(); i++) {
        if (sendCb->isChecked()) {
            pointsToPlot[number][i].setY(axisValues[axisValues.size() - 1 - i]);
        }
        else {
            pointsToPlot[number][i].setY(axisValues[i]);
        }
    }
    updateChart(number);
}

QList<coordinates>* FormBuilder::getCoordinates(int number) {
    return &pointsToPlot[number];
}

void FormBuilder::updateY(int number, int index, int value) {
    pointsToPlot[number][index].setY((float)value);
    updateChart(number);
}

void FormBuilder::updateXCall() {
    auto* senderLE = qobject_cast<QLineEdit *>(sender());
    auto index = senderLE->objectName().right(1).toInt();
    emit updateXSignal(senderLE->objectName().first(1).toInt(), index,
                       senderLE->text().toInt());
}

void FormBuilder::updateYCall() {
    auto* senderLE = qobject_cast<QLineEdit *>(sender());
    auto index = senderLE->objectName().right(1).toInt();
    emit updateYSignal(senderLE->objectName().first(1).toInt(), index,
                       senderLE->text().toInt());
}

void FormBuilder::updateX(int number, int index, int value) {
    pointsToPlot[number][index].setX((float)value);
    updateChart(number);
}

void FormBuilder::setCurves(QStringList namesToSet) { curves = std::move(namesToSet); }

void FormBuilder::changeSlider() {
    auto slider = qobject_cast<QSlider *>(sender());
    int number = slider->objectName().first(1).toInt();
    float value = 0;
    QString name = curves.at(number);
    if (slider->objectName() ==
        QString::number(number) + name + "MinSensitivity") {
        value = static_cast<float>(slider->value() / 100.0) * 511.0f;
        pointsToPlot[number][1] = {
            static_cast<float>(neutralValue[number]) - value,
            pointsToPlot[number][1].getY()
        };
    }
    if (slider->objectName() == QString::number(number) + name + "Deadzone") {
        value = 1023.0f * static_cast<float>(slider->value() / 100.0);

        pointsToPlot[number][2] = {
            static_cast<float>(neutralValue[number]) - (value / 2), 0
        };
        pointsToPlot[number][4] = {
            static_cast<float>(neutralValue[number]) + (value / 2), 0
        };
    }
    if (slider->objectName() ==
        QString::number(number) + name + "PlusSensitivity") {
        value = static_cast<float>((slider->value() / 100.0) * 511.0f);
        pointsToPlot[number][5] = {
            (float)neutralValue[number] + value,
            pointsToPlot[number][pointsToPlot[number].size() - 2].getY()
        };
    }

    updateChart(number);
}

void FormBuilder::updateChart(int number) {
    series.at(number)->clear();
    for (auto&i: pointsToPlot[number]) {
        series.at(number)->append(i.getX(), i.getY());
    }
    charts[number]->removeAxis(charts[number]->axes(Qt::Horizontal).back());
    auto* xAxis = new QValueAxis();
    xAxis->setRange(minValue[number], maxValue[number]);
    xAxis->setLabelFormat("%i");
    charts[number]->addAxis(xAxis, Qt::AlignBottom);
    series[number]->attachAxis(xAxis);
    chartViews.at(number)->update();
}

QVBoxLayout* FormBuilder::generateCurveCol(int number, int valAxis,
                                           int valRange) {
    auto* colEntries = new QVBoxLayout();
    auto* inputFieldRange = new QLineEdit();
    auto* inputFieldAxis = new QLineEdit();

    auto index = QString::number(pointsToPlot.size());
    inputFieldRange->setObjectName("x" + index);
    inputFieldAxis->setObjectName("y" + index);

    coordinates coords = {(float)valRange, (float)valAxis};

    colEntries->addWidget(inputFieldRange);
    colEntries->addWidget(inputFieldAxis);
    inputFieldRange->setText(QString::number(valRange));
    inputFieldAxis->setText(QString::number(valAxis));
    connect(inputFieldRange, &QLineEdit::textEdited, this,
            &FormBuilder::updateXCall);
    connect(this, &FormBuilder::updateXSignal, this, &FormBuilder::updateXCall);
    connect(this, &FormBuilder::updateYSignal, this, &FormBuilder::updateYCall);
    connect(inputFieldAxis, &QLineEdit::textEdited, this,
            &FormBuilder::updateYCall);
    return colEntries;
}

QVBoxLayout* FormBuilder::generateRange(const QString&header) {
    auto rangeBlock = new QVBoxLayout();
    auto headerRow = new QHBoxLayout();
    auto headerLabel = new QLabel();
    headerLabel->setText(header);
    headerRow->addWidget(headerLabel);
    rangeBlock->addLayout(headerRow);

    auto labelRow = new QHBoxLayout();
    auto minLabel = new QLabel();
    auto maxLabel = new QLabel();
    minLabel->setText("Min");
    maxLabel->setText("Max");
    labelRow->addWidget(minLabel);
    labelRow->addWidget(maxLabel);
    rangeBlock->addLayout(labelRow);

    auto editTextRow = new QHBoxLayout();
    auto minLE = new QLineEdit();
    minLE->setObjectName(header + "Min");
    minLE->setText("0");
    auto maxLE = new QLineEdit();
    maxLE->setObjectName(header + "Max");
    maxLE->setText("1023");
    editTextRow->addWidget(minLE);
    editTextRow->addWidget(maxLE);
    rangeBlock->addLayout(editTextRow);
    qDebug() << "Done";
    return rangeBlock;
}

QVBoxLayout* FormBuilder::RangeBuilder() {
    auto* rangeHLayout = new QVBoxLayout();
    rangeHLayout->setObjectName("rangeLayout");
    // rangeHLayout->addLayout(createRudderRow());
    auto* rangeHeader = new QLabel();
    rangeHeader->setText("Ranges");
    rangeHLayout->addWidget(rangeHeader);

    auto* engineRanges = new QVBoxLayout();
    engineRanges->setObjectName("engineRangesLayout");

    for (int i = 0; i < engineHeaders.size(); i++) {
        auto* engineRange =
                new SettingsRanges(engineLabels.size(), engineLabels, engineHeaders[i]);
        auto* layout = new QVBoxLayout();
        layout = engineRange->CreateRangeRow();
        engineRanges->addLayout(layout);
    }
    rangeHLayout->addLayout(engineRanges);
    rangeHLayout->setContentsMargins(0, 0, 0, 0);

    for (int i = 0; i < rangeHeaders.size(); i++) {
        rangeHLayout->addLayout(generateRange(rangeHeaders[i]));
    }

    return rangeHLayout;
}


QVBoxLayout* FormBuilder::generateComColumn(int index) {
    auto* comColumn = new QVBoxLayout();
    auto* grid = new QGridLayout();
    auto* columnHeader = new QLabel();
    columnHeader->setText(comHeaders[index]);
    auto* portLabel = new QLabel();
    portLabel->setText("Select your comport: ");
    grid->addWidget(portLabel, 1, 0);
    auto* comBoxes = new QComboBox();
    grid->addWidget(comBoxes, 1, 1);

    return comColumn;
}

QLabel* FormBuilder::generateHeader(const QString&text) {
    auto* header = new QLabel(text);
    header->setObjectName("header" + text);
    QFont font = header->font();
    font.setPointSize(16);
    header->setFont(font);
    return header;
}

void FormBuilder::autoRunChanged() {
    auto senderCB = qobject_cast<ModeIndexCheckbox *>(sender());

    int mode = senderCB->getMode();
    QString index = QString::number(senderCB->getIndex());
    QString group;
    switch (mode) {
        //        case INPUTMODE:  {
        //            group = "inputARIndex";
        //            break;
        //        }
        //
        //        case OUTPUTMODE: {
        //            group = "outputARIndex";
        //            break;
        //        }
        //
        //        case DUALMODE:   {
        //            group = "dualARIndex";
        //            break;
        //        }
        default: break;
    }
    if (!settingsHandler.retrieveSetting(group, index)->isNull()) {
        settingsHandler.storeValue(group, index, senderCB->isChecked());
    }
}
