#ifndef FORMBUILDER_H
#define FORMBUILDER_H

#include <qstring.h>

#include <QHBoxLayout>
#include <QLabel>
#include <qtabwidget.h>

#include <outputs/set.h>
#include <outputs/sethandler.h>

using namespace std;

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

  QStringList comHeaders = {"Inputs", "Outputs", "Both"};
  QStringList rangeHeaders;
  QStringList engineLabels = {"Reverse", "Idle cutoff", "Max"};
  QStringList engineHeaders;

  int getAmountOfEngines() { return engineHeaders.size(); };

  QTabWidget *generateOutputTabs();
  QVBoxLayout *generateOutputSetList();
  QGridLayout *generateOutputControls();
  //QVBoxLayout *generateActiveSet();

  QWidget *generateSetRow(set setForRow);
  QHBoxLayout *generateOutputRow(Output *output);
  QWidget *generateActiveSet(set *selectedSet);
  QLabel *generateHeader(QString text);
  QWidget *generateComSelector(bool setsNeeded, int mode);
  QWidget *generateComControls(int mode);
  QList<set> getAvailableSets() { return availableSets;};
  QList<QString> getAvailableComPorts(){ return availableComPorts;};

private slots:
  void localRemove();
  void localEdit();

  void localStart();
  void localRefreshed();
  void localStop();
  void localAdd();
  void removeComWidget();
signals:
  void addSet();
  void setEdited(QString id);
  void removeSet(QString id);
  void startPressed(int mode);
  void refreshPressed(int mode);
  void stopPressed(int mode);
  void addPressed(int mode);
private:
  SetHandler setHandler;
  QList<set> availableSets;
  QList<QString> availableComPorts;


};

#endif  // FORMBUILDER_H
