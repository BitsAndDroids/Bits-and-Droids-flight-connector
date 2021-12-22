#include "formbuilder.h"

#include <headers/constants.h>
#include <qcombobox.h>
#include <settings/settingsranges.h>

#include <QtCharts>
#include <QtSerialPort>
#include <iostream>

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

  cout << "bye" << endl;
  availableSets = setHandler.getSets();
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
        coordinates *coord = new coordinates(x, y);
        cout << coord->getY() << " : " << coord->getX() << endl;

        pointsToPlot[i].append(*coord);
      }
    }
  }
}

QVBoxLayout *FormBuilder::createAxisRow(QString name, int number) {
  auto *layout = new QVBoxLayout();
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

  auto *xAxis = new QValueAxis();

  for (int i = 0; i < objectNames.size(); i++) {
    if (!settingsHandler
             .retrieveSubSetting(objectNames[i] + "Series", "calibrations",
                                 name + objectNames.at(i))
             ->isNull()) {
      int valFound = settingsHandler
                         .retrieveSubSetting(name + "Series", "calibrations",
                                             name + objectNames.at(i))
                         ->toInt();
      switch (i) {
        case 0:
          minRudderValue = valFound;
          break;
        case 1:
          neutralRudderValue = valFound;
          break;
        case 2:
          maxRudderValue = valFound;
          break;
        default:
          break;
      }
    }
  }
  xAxis->setRange(minRudderValue, maxRudderValue);
  xAxis->setLabelFormat("%i");

  auto *yAxis = new QValueAxis();
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
      QString::number(number) + name + "PlusSensitivity"};
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
        lineEdit->setText(QString::number(minRudderValue));
        break;
      case 1:
        lineEdit->setText(QString::number(neutralRudderValue));
        break;
      case 2:
        lineEdit->setText(QString::number(maxRudderValue));
        break;
      default:
        break;
    }

    lineEdit->setObjectName(name + objectNames.at(i));
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
    cout << "empty" << endl;
    QList<coordinates> *coords = new QList<coordinates>{
        {coordinates(static_cast<float>(minRudderValue), min)},
        {coordinates(
            static_cast<float>(neutralRudderValue) -
                static_cast<float>((maxRudderValue - neutralRudderValue) / 2),
            axisValues[1])},
        {coordinates(500, axisValues[2])},
        {coordinates(static_cast<float>(neutralRudderValue), axisValues[2])},
        {coordinates(522, axisValues[2])},
        {coordinates(
            static_cast<float>(neutralRudderValue) +
                static_cast<float>((maxRudderValue - neutralRudderValue) / 2),
            axisValues[5])},
        {coordinates(static_cast<float>(maxRudderValue), max)}};

    for (auto &i : *coords) {
      pointsToPlot[number].append(i);
      series[number]->append(i.getX(), i.getY());
    }
  } else {
    for (auto &i : pointsToPlot[number]) {
      series[number]->append(i.getX(), i.getY());
    }
  }
  // layout->addLayout(curveControls);
  series[number]->attachAxis(yAxis);
  series[number]->attachAxis(xAxis);
  return layout;
}

void FormBuilder::rudderTextChanged() {
  qDebug() << "Got";
  auto senderLineEdit = qobject_cast<QLineEdit *>(sender());
  int valueToChange = senderLineEdit->text().toInt();
  int index = senderLineEdit->objectName().first(1).toInt();
  switch (index) {
    case 0:
      pointsToPlot[index][0].setX(static_cast<float>(valueToChange));
      minRudderValue = valueToChange;
      break;
    case 1:
      pointsToPlot[index][3].setX(static_cast<float>(valueToChange));
      neutralRudderValue = valueToChange;
      break;
    case 2:
      pointsToPlot[index][6].setX(static_cast<float>(valueToChange));
      maxRudderValue = valueToChange;
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
    } else {
      pointsToPlot[number][i].setY(axisValues[i]);
    }
  }
  updateChart(number);
}
QList<coordinates> *FormBuilder::getCoordinates(int number) {
  return &pointsToPlot[number];
}

void FormBuilder::updateY(int number, int index, int value) {
  pointsToPlot[number][index].setY(value);
  // cout << "Y" << index << " val" << pointsToPlot[index].y << endl;
  updateChart(number);
}
void FormBuilder::updateXCall() {
  auto *senderLE = qobject_cast<QLineEdit *>(sender());
  auto index = senderLE->objectName().right(1).toInt();
  emit updateXSignal(senderLE->objectName().first(1).toInt(), index,
                     senderLE->text().toInt());
}
void FormBuilder::updateYCall() {
  auto *senderLE = qobject_cast<QLineEdit *>(sender());
  auto index = senderLE->objectName().right(1).toInt();
  emit updateYSignal(senderLE->objectName().first(1).toInt(), index,
                     senderLE->text().toInt());
}
void FormBuilder::updateX(int number, int index, int value) {
  pointsToPlot[number][index].setX(value);
  // cout << "X" << index << " val" << pointsToPlot[index].x << endl;
  updateChart(number);
}
void FormBuilder::setCurves(QStringList namesToSet) { curves = namesToSet; }
void FormBuilder::changeSlider() {
  auto slider = qobject_cast<QSlider *>(sender());
  int number = slider->objectName().first(1).toInt();
  float value = 0;
  QString name = curves.at(number);
  if (slider->objectName() ==
      QString::number(number) + name + "MinSensitivity") {
    value = static_cast<float>(slider->value() / 100.0) * 511.0f;
    pointsToPlot[number][1] = {static_cast<float>(neutralRudderValue) - value,
                               pointsToPlot[number][1].getY()};
  }
  if (slider->objectName() == QString::number(number) + name + "Deadzone") {
    value = 1023.0f * static_cast<float>(slider->value() / 100.0);

    pointsToPlot[number][2] = {
        static_cast<float>(neutralRudderValue) - (value / 2), 0};
    pointsToPlot[number][4] = {
        static_cast<float>(neutralRudderValue) + (value / 2), 0};
  }
  if (slider->objectName() ==
      QString::number(number) + name + "PlusSensitivity") {
    value = static_cast<float>((slider->value() / 100.0) * 511.0f);
    pointsToPlot[number][5] = {
        (float)neutralRudderValue + value,
        pointsToPlot[number][pointsToPlot[number].size() - 2].getY()};
  }

  updateChart(number);
}

void FormBuilder::updateChart(int number) {
  series.at(number)->clear();
  for (auto &i : pointsToPlot[number]) {
    series.at(number)->append(i.getX(), i.getY());
  }
  charts[number]->removeAxis(charts[number]->axes(Qt::Horizontal).back());
  auto *xAxis = new QValueAxis();
  xAxis->setRange(minRudderValue, maxRudderValue);
  xAxis->setLabelFormat("%i");
  charts[number]->addAxis(xAxis, Qt::AlignBottom);
  series[number]->attachAxis(xAxis);
  chartViews.at(number)->update();
}

QVBoxLayout *FormBuilder::generateCurveCol(int number, int valAxis,
                                           int valRange) {
  auto *colEntries = new QVBoxLayout();
  auto *inputFieldRange = new QLineEdit();
  auto *inputFieldAxis = new QLineEdit();

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

QVBoxLayout *FormBuilder::generateRange(const QString &header) {
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

QVBoxLayout *FormBuilder::RangeBuilder() {
  auto *rangeHLayout = new QVBoxLayout();
  rangeHLayout->setObjectName("rangeLayout");
  // rangeHLayout->addLayout(createRudderRow());
  auto *rangeHeader = new QLabel();
  rangeHeader->setText("Ranges");
  rangeHLayout->addWidget(rangeHeader);

  auto *engineRanges = new QVBoxLayout();
  engineRanges->setObjectName("engineRangesLayout");

  for (int i = 0; i < engineHeaders.size(); i++) {
    auto *engineRange =
        new SettingsRanges(engineLabels.size(), engineLabels, engineHeaders[i]);
    auto *layout = new QVBoxLayout();
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

QHBoxLayout *FormBuilder::generateComBlock() {
  auto *comContainer = new QHBoxLayout();

  for (int i = 0; i < comHeaders.size(); i++) {
    comContainer->addLayout(generateComColumn(i));
  }

  return comContainer;
}

QVBoxLayout *FormBuilder::generateComColumn(int index) {
  auto *comColumn = new QVBoxLayout();
  auto *grid = new QGridLayout();
  auto *columnHeader = new QLabel();
  columnHeader->setText(comHeaders[index]);
  auto *portLabel = new QLabel();
  portLabel->setText("Select your comport: ");
  grid->addWidget(portLabel, 1, 0);
  auto *comBoxes = new QComboBox();
  grid->addWidget(comBoxes, 1, 1);

  return comColumn;
}

QGridLayout *FormBuilder::generateOutputControls() const {
  auto *outputControls = new QGridLayout();
  outputControls->setAlignment(Qt::AlignLeft);
  auto *setName = new QLineEdit();
  setName->setMaximumWidth(150);
  setName->setObjectName("leSetName");
  auto *setNameLabel = new QLabel("Set name");

  auto *saveSet = new QPushButton("Save set");
  connect(saveSet, &QAbstractButton::clicked, this, &FormBuilder::addSet);

  outputControls->addWidget(setNameLabel, 0, 0);
  outputControls->addWidget(setName, 0, 1, Qt::AlignLeft);
  outputControls->addWidget(saveSet, 2, 0);
  return outputControls;
}

QVBoxLayout *FormBuilder::generateOutputSetList() {
  auto *outputSetList = new QVBoxLayout();
  outputSetList->setObjectName("outputSetList");
  auto *listHeader = new QLabel("Saved sets");
  QFont font = listHeader->font();
  font.setBold(true);
  font.setPointSize(24);
  listHeader->setFont(font);

  outputSetList->addWidget(listHeader);

  return outputSetList;
}

QWidget *FormBuilder::generateActiveSet(set *selectedSet) {
  auto *activeWidget = new QWidget();
  activeWidget->setObjectName("activeWidget");
  auto *activeSet = new QVBoxLayout();

  auto *setNameHeader = new QLabel(selectedSet->getSetName());
  setNameHeader->setObjectName("setNameHeader");
  QFont headerFont = setNameHeader->font();
  headerFont.setPointSize(18);
  headerFont.setBold(true);
  setNameHeader->setFont(headerFont);
  activeSet->addWidget(setNameHeader);

  auto *outputGrid = new QGridLayout();
  int columnCounter = 0;
  int rowCounter = 0;
  QMap<int, Output *> outputsInSet = selectedSet->getOutputs();
  QMap<int, Output *>::Iterator i;
  for (i = outputsInSet.begin(); i != outputsInSet.end(); i++) {
    if (rowCounter % 5 == 0) {
      columnCounter++;
      rowCounter = 0;
    }
    rowCounter++;
    auto *outputName = new QLabel(i.value()->getCbText());
    outputGrid->addWidget(outputName, rowCounter, columnCounter);
  }

  auto *outputList = new QVBoxLayout();
  outputList->setObjectName("savedOutputs");

  activeSet->addLayout(outputList);
  activeSet->addLayout(outputGrid);
  activeWidget->setLayout(activeSet);

  return activeWidget;
}

QWidget *FormBuilder::generateSetRow(const set &setForRow) {
  auto *setRowContainer = new QWidget();
  setRowContainer->setSizePolicy(QSizePolicy::Preferred,
                                 QSizePolicy::Expanding);
  setRowContainer->setMinimumHeight(20);
  auto *setRow = new QHBoxLayout();
  auto *setRowLabel = new QLabel(setForRow.getSetName());

  auto *editButton = new QPushButton("Edit");
  connect(editButton, &QAbstractButton::clicked, this, &FormBuilder::localEdit);

  auto *deleteButton = new QPushButton("Delete");
  connect(deleteButton, SIGNAL(clicked()), SLOT(localRemove()));
  editButton->setMaximumWidth(150);
  deleteButton->setMaximumWidth(150);

  setRow->addWidget(setRowLabel);

  setRow->addWidget(editButton);
  setRow->addWidget(deleteButton);
  setRowContainer->setLayout(setRow);
  setRowContainer->setAutoFillBackground(true);
  setRowContainer->setPalette(Qt::white);
  setRowContainer->setObjectName(QString::number(setForRow.getID()));
  setRowContainer->setMinimumSize(600, 50);
  setRowContainer->setMaximumSize(3000, 100);
  return setRowContainer;
}

QTabWidget *FormBuilder::generateOutputTabs() {
  auto *outputTabs = new QTabWidget();
  outputTabs->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  outputTabs->setObjectName("outputTabWidget");
  outputHandler outputHandler;
  auto categorieList = outputHandler.getCategoryStrings();
  auto categorizedOutputs = outputHandler.getOutputsCategorized();

  for (int i = 0; i < categorieList.size(); i++) {
    auto *newTab = new QWidget();
    auto *cbGridLayout = new QGridLayout();
    cbGridLayout->setAlignment(Qt::AlignTop);
    for (int j = 0; j < categorizedOutputs[i].size(); j++) {
      auto *checkbox = new QCheckBox();
      checkbox->setMinimumHeight(30);
      QString cbText = categorizedOutputs[i][j].getCbText();
      checkbox->setText(QString(cbText));
      checkbox->setObjectName(
          "cb" + QString::number(categorizedOutputs[i][j].getId()));

      if (j < 15) {
        cbGridLayout->addWidget(checkbox, j, 0);
      } else if (j < 30) {
        cbGridLayout->addWidget(checkbox, j - 15, 1);
      } else if (j < 45) {
        cbGridLayout->addWidget(checkbox, j - 30, 2);
      }
    }
    newTab->setLayout(cbGridLayout);
    outputTabs->addTab(newTab, categorieList[i]);
  }

  outputTabs->setMinimumSize(600, 400);
  outputTabs->adjustSize();
  return outputTabs;
}

void FormBuilder::loadComPortData() {
  availableComPorts.clear();
  foreach (const QSerialPortInfo &serialPortInfo,
           QSerialPortInfo::availablePorts()) {
    availableComPorts.append(serialPortInfo.portName() + " | " +
                             serialPortInfo.description());
  }
}

QHBoxLayout *FormBuilder::generateOutputRow(Output *output) {
  auto *row = new QHBoxLayout();
  auto *outputName = new QLabel(output->getCbText());
  row->addWidget(outputName);
  return row;
}

void FormBuilder::localRemove() {
  qDebug() << "apple";
  auto *button = qobject_cast<QPushButton *>(sender());
  emit removeSet(button->parentWidget()->objectName());
}

void FormBuilder::localEdit() {
  auto *button = qobject_cast<QPushButton *>(sender());
  emit setEdited(button->parentWidget()->objectName());
}

QLabel *FormBuilder::generateHeader(const QString &text) {
  auto *header = new QLabel(text);
  QFont font = header->font();
  font.setPointSize(16);
  header->setFont(font);
  return header;
}

QWidget *FormBuilder::generateComSelector(bool setsNeeded, int mode,
                                          int index) {
  auto *comSelector = new QWidget();
  auto *comRow = new QHBoxLayout();
  comSelector->setLayout(comRow);
  auto *comPortComboBox = new QComboBox();
  comPortComboBox->setObjectName("comBox" + QString::number(index));
  for (auto &availableComPort : availableComPorts) {
    comPortComboBox->addItem(availableComPort);
  }
  comRow->addWidget(comPortComboBox);
  comPortComboBox->setMinimumWidth(150);
  if (setsNeeded) {
    auto *setComboBox = new QComboBox();
    for (const auto &availableSet : *availableSets) {
      setComboBox->addItem(availableSet.getSetName());
    }
    setComboBox->setMinimumWidth(150);
    setComboBox->setObjectName("setBox" + QString::number(index));
    comRow->addWidget(setComboBox);
    qDebug() << setComboBox->objectName();
  }

  auto *removeButton = new QPushButton("-");
  removeButton->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
  removeButton->setMinimumSize(20, 20);
  removeButton->setMaximumSize(20, 20);
  connect(removeButton, &QAbstractButton::clicked, this,
          &FormBuilder::removeComWidget);
  comRow->addWidget(removeButton);
  return comSelector;
}

QWidget *FormBuilder::generateComControls(int mode) {
  auto *comControls = new QWidget();
  auto *comControlRow = new QHBoxLayout();
  comControls->setLayout(comControlRow);
  comControlRow->addWidget(generateHeader(mainHeaders[mode]));
  comControlRow->setAlignment(Qt::AlignLeft);
  // REFRESH BTN
  auto *refreshButton = new QPushButton();

  refreshButton->setIcon(QIcon("images/refreshicon.png"));
  refreshButton->setStyleSheet(
      "border-image:url(:/images/refreshicon.png); background-color:#fff;");
  refreshButton->setObjectName(QString::number(mode) + "refreshBtn");

  refreshButton->setMinimumSize(15, 15);
  refreshButton->setMaximumSize(15, 15);
  connect(refreshButton, &QAbstractButton::clicked, this,
          &FormBuilder::localRefreshed);
  comControlRow->addWidget(refreshButton);

  // START BTN
  auto *startButton = new QPushButton("Start");
  startButton->setCheckable(true);
  //#2DE3A3
  startButton->setStyleSheet(

      "QPushButton {\
                      color:white;\
                      background-color:#509402;\
                  }   \
                  QPushButton:checked{\
                      background-color: #0F4C5C;\
                      border: none; \
                  }\
                  QPushButton:hover{  \
                      background-color: grey; \
                      border-style: outset;\
                  }");

  startButton->setMinimumSize(50, 25);
  startButton->setMaximumSize(50, 25);
  startButton->setObjectName(QString::number(mode) + "startButton");
  connect(startButton, &QAbstractButton::clicked, this,
          &FormBuilder::localStart);
  comControlRow->addWidget(startButton);

  // STOP BTN
  auto *stopButton = new QPushButton("Stop");
  stopButton->setMinimumSize(50, 25);
  stopButton->setMaximumSize(50, 25);
  stopButton->setObjectName(QString::number(mode) + "stopBtn");
  connect(stopButton, &QAbstractButton::clicked, this, &FormBuilder::localStop);
  comControlRow->addWidget(stopButton);

  // ADD BTN
  auto *addButton = new QPushButton("+");
  addButton->setMinimumSize(20, 20);
  addButton->setMaximumSize(20, 20);
  addButton->setObjectName(QString::number(mode) + "addBtn");
  connect(addButton, &QAbstractButton::clicked, this, &FormBuilder::localAdd);
  comControlRow->addWidget(addButton);

  return comControls;
}

void FormBuilder::removeComWidget() {
  QWidget *senderWidget = qobject_cast<QWidget *>(sender()->parent());
  delete senderWidget;
}

void FormBuilder::localStart() {
  auto *pressedBtn = qobject_cast<QPushButton *>(sender());
  pressedBtn->setEnabled(false);
  pressedBtn->setText("Running");

  int mode = pressedBtn->objectName().left(1).toInt();
  QString stopText = QString::number(mode) + "stopBtn";
  auto *stopButton = pressedBtn->parent()->findChild<QPushButton *>(stopText);
  stopButton->setStyleSheet("background-color:#E20303");

  emit startPressed(mode);
}

void FormBuilder::localRefreshed() {
  auto *pressedBtn = qobject_cast<QPushButton *>(sender());
  int mode = pressedBtn->objectName().left(1).toInt();
  emit refreshPressed(mode);
}

void FormBuilder::localStop() {
  auto *pressedBtn = qobject_cast<QPushButton *>(sender());
  int mode = pressedBtn->objectName().left(1).toInt();
  QString startBtnText = QString::number(mode) + "startButton";

  auto *startBtn = pressedBtn->parent()->findChild<QPushButton *>(startBtnText);
  startBtn->setEnabled(true);
  startBtn->setChecked(false);
  startBtn->setText("Start");
  pressedBtn->setStyleSheet("background-color:#0F4C5C");
  emit stopPressed(mode);
}

void FormBuilder::localAdd() {
  auto *pressedBtn = qobject_cast<QPushButton *>(sender());

  int mode = pressedBtn->objectName().left(1).toInt();
  emit addPressed(mode);
}
