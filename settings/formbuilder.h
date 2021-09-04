#ifndef FORMBUILDER_H
#define FORMBUILDER_H

#include <outputs/outputhandler.h>
#include <outputs/set.h>
#include <outputs/sethandler.h>
#include <qstring.h>
#include <qtabwidget.h>

#include <QHBoxLayout>
#include <QLabel>
#include <QtCharts>
#include <qcustomplot.h>
#include "settingshandler.h"

class FormBuilder : public QObject {
Q_OBJECT

public:
    FormBuilder();
    //~FormBuilder();

    QVBoxLayout *RangeBuilder();

    void loadComPortData();

    QVBoxLayout *generateComColumn(int index);

    QHBoxLayout *generateComBlock();

    QVBoxLayout *generateRange(QString header);

    QVBoxLayout *createRudderRow();

    QStringList comHeaders = {"Inputs", "Outputs", "Both"};
    QStringList rangeHeaders;
    QStringList engineLabels = {"Reverse", "Idle cutoff", "Max"};
    QStringList engineHeaders;

    int getAmountOfEngines() { return engineHeaders.size(); };

    QTabWidget *generateOutputTabs();

    QVBoxLayout *generateOutputSetList();

    QGridLayout *generateOutputControls();
    // QVBoxLayout *generateActiveSet();

    QWidget *generateSetRow(set setForRow);

    QHBoxLayout *generateOutputRow(Output *output);

    QWidget *generateActiveSet(set *selectedSet);

    QLabel *generateHeader(QString text);

    QWidget *generateComSelector(bool setsNeeded, int mode);

    QWidget *generateComControls(int mode);

    QList<set> *getAvailableSets() { return availableSets; };

    QList<QString> getAvailableComPorts() { return availableComPorts; };

private slots:

    void localRemove();

    void localEdit();

    void localStart();

    void localRefreshed();

    void localStop();

    void localAdd();

    void removeComWidget();


    void updateX();
//
    void updateY();

signals:

    void addSet();

    void setEdited(QString id);

    void removeSet(QString id);

    void startPressed(int mode);

    void refreshPressed(int mode);

    void stopPressed(int mode);

    void addPressed(int mode);

private:
    QChart *chart;
    QCustomPlot *cplot;
    QLineSeries *series;
    QChartView *chartView;
    QStringList mainHeaders = {"", "INPUT", "OUTPUT", "DUAL"};
    SetHandler setHandler;
    SettingsHandler settingsHandler;
    QList<set> *availableSets;
    QList<QString> availableComPorts;


    void updateChart();

    struct coordinates {
        float x;
        float y;
    };
    QList<coordinates> pointsToPlot;


    QVBoxLayout *generateCurveCol(int valAxis, int valRange);

    void changeSlider();
};

#endif  // FORMBUILDER_H
