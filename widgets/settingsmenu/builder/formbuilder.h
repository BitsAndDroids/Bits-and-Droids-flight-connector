#ifndef FORMBUILDER_H
#define FORMBUILDER_H

#include "handlers/outputhandler.h"
#include "models/commands/set.h"
#include "handlers/sethandler.h"
#include <qstring.h>
#include <qtabwidget.h>

#include <QHBoxLayout>
#include <QLabel>
#include <QtCharts>

#include "models/settings/coordinates.h"
#include "handlers/settingshandler.h"

class FormBuilder : public QObject {
Q_OBJECT

public:
    FormBuilder();

    //~FormBuilder();
    QStringList getCalibrateLabels() { return objectNames; };

    QVBoxLayout *RangeBuilder();

    void loadComPortData();

    QVBoxLayout *generateComColumn(int index);

    QHBoxLayout *generateComBlock();

    static QVBoxLayout *generateRange(const QString &header);

    QVBoxLayout *createAxisRow(QString name, int number);

    QStringList comHeaders = {"Inputs", "Outputs", "Both"};
    QStringList rangeHeaders;
    QStringList engineLabels = {"Reverse", "Idle cutoff", "Max"};
    QStringList engineHeaders;

    void setCurves(QStringList);

    int getAmountOfEngines() { return engineHeaders.size(); };

    static QTabWidget *generateOutputTabs();

    static QVBoxLayout *generateOutputSetList();

    QGridLayout *generateOutputControls() const;
    // QVBoxLayout *generateActiveSet();

    QWidget *generateSetRow(const set &setForRow);

    QHBoxLayout *generateOutputRow(Output *output);

    static QWidget *generateActiveSet(set *selectedSet);

    static QLabel *generateHeader(const QString &text);

    QWidget *generateComSelector(bool setsNeeded, int mode);

    QWidget *generateComControls(int mode);

    QList<set> *getAvailableSets() { return availableSets; };

    QList<QString> getAvailableComPorts() { return availableComPorts; };

    QList<struct coordinates> *getCoordinates(int number);

    QWidget *generateComSelector(bool setsNeeded, int mode, int index);

    void loadPointsToPlot(QStringList axis);

private slots:

    void localRemove();

    void localEdit();

    void localStart();

    void localRefreshed();

    void localStop();

    void localAdd();

    void autoRunChanged();

    void removeComWidget();

    void rudderTextChanged();

    void updateXCall();

    void updateYCall();

    void updateX(int number, int index, int value);

    void reverseClicked();

    void updateY(int number, int index, int value);

signals:

    void updateXSignal(int number, int index, int value);

    void updateYSignal(int number, int index, int value);

    void addSet();

    void setEdited(QString id);

    void removeSet(QString id);

    void startPressed(int mode);

    void refreshPressed(int mode);

    void stopPressed(int mode);

    void addPressed(int mode);

private:
    QStringList curves;
    QStringList objectNames = {"MinLE", "NeutralLE", "MaxLE"};
    QList<int> axisValues = {-16383, -10000, 0, 0, 0, 10000, 16383};
    QList<QChart *> charts = QList<QChart *>();

    QList<QLineSeries *> series = QList<QLineSeries *>();
    QList<QChartView *> chartViews = QList<QChartView *>();
    QStringList mainHeaders = {"", "INPUT", "OUTPUT", "DUAL"};
    SetHandler setHandler;
    SettingsHandler settingsHandler;
    QList<set> *availableSets;
    QList<QString> availableComPorts;

    void updateChart(int number);

    QList<QList<coordinates>> pointsToPlot = QList<QList<coordinates>>();

    QVBoxLayout *generateCurveCol(int number, int valAxis, int valRange);

    void changeSlider();

    int minValue[4] = {0, 0, 0, 0};

    int maxValue[4] = {1023, 1023, 1023, 1023};

    int neutralValue[4] = {((1023 - 0) / 2), ((1023 - 0) / 2), ((1023 - 0) / 2),
                           ((1023 - 0) / 2)};

    void adjustIndexes();

    void adjustIndexes(int index);

    void adjustIndexes(int mode, int index);


};

#endif  // FORMBUILDER_H
