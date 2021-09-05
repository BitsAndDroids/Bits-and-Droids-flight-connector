#include "optionsmenu.h"

#include <qstandardpaths.h>

#include <QLineEdit>
#include <string>

#include "formbuilder.h"

#include "ui_optionsmenu.h"

FormBuilder *builder = new FormBuilder();


using namespace std;

optionsMenu::optionsMenu(QWidget *parent)
        : QWidget(parent), uiOptions(new Ui::optionsMenu) {
    uiOptions->setupUi(this);

    QList<QLineEdit *> allLabels =
            uiOptions->formLayoutWidget->findChildren<QLineEdit *>();

    QStringList keys = *settingsHandler.retrieveKeys("Settings");
    if (!keys.empty()) {
                foreach (const QString &key, keys) {
                if (uiOptions->formLayoutWidget->findChild<QLineEdit *>(key)) {
                    uiOptions->formLayoutWidget->findChild<QLineEdit *>(key)->setText(
                            settingsHandler.retrieveSetting("Settings", key)->toString());
                }
            }
        if (!settingsHandler.retrieveSetting("Settings", "CBR")->isNull()) {
            uiOptions->baudComboBox->setCurrentText(
                    settingsHandler.retrieveSetting("Setting", "CBR")->toString());
        }
    }

    // Range handling
    uiOptions->vlOptions->setAlignment(Qt::AlignTop);
    // uiOptions->vlEngineRange->addLayout(builder->createRudderRow());
    uiOptions->vlEngineRange->addLayout(builder->RangeBuilder());

    QList<QLineEdit *> allRangeLabels =
            uiOptions->widgetRanges->findChildren<QLineEdit *>();
    QStringList rangeKeys = *settingsHandler.retrieveKeys("Ranges");
    if (!rangeKeys.empty()) {
                foreach (const QString &key, rangeKeys) {
                if (uiOptions->widgetRanges->findChild<QLineEdit *>(key)) {
                    uiOptions->widgetRanges->findChild<QLineEdit *>(key)->setText(
                            settingsHandler.retrieveSetting("Ranges", key)->toString());
                }
            }
    }
    if (!settingsHandler.retrieveSetting("Ranges", "maxReverseId")->isNull()) {
        int value =
                settingsHandler.retrieveSetting("Ranges", "maxReverseId")->toInt();
        if (value != -1) {
            uiOptions->buttonGroup->button(value)->click();
        }
    }
}

optionsMenu::~optionsMenu() { delete uiOptions; }

void optionsMenu::on_saveSettingsBtn_clicked() {
    QList<QLineEdit *> allLabels =
            uiOptions->formLayoutWidget->findChildren<QLineEdit *>();
    qDebug() << "size" << allLabels.size();
    for (auto allLabel : allLabels) {
        QString name = allLabel->objectName();
        qDebug() << "Clicked" << name;
        settingsHandler.storeValue("Settings", name, allLabel->text());
    }
    settingsHandler.storeValue("Settings", "CBR",
                               uiOptions->baudComboBox->currentText());

    auto qChartView = uiOptions->
            vlEngineRange->
            findChild<QLayout *>("rudderCalibrateLayout")->
            findChild<QChartView *>("rudderChartView");
    qDebug()<<"iop";
    auto series = qChartView->chart()->series();



    settingsHandler.storeValue("Ranges","rudderSeries", QVariant::fromValue(series));
    QList<QLineEdit *> rangeLineEdits =
            uiOptions->widgetRanges->findChildren<QLineEdit *>();

    for (auto &rangeLineEdit : rangeLineEdits) {
        settingsHandler.storeValue("Ranges", rangeLineEdit->objectName(),
                                   rangeLineEdit->text());
    }

    QString idleStr = "Engine " + QString::number(1) + "Min";
    int idleCutoff = settingsHandler.retrieveSetting("Ranges", idleStr)->toInt();
    qDebug() << "cut" << idleCutoff;

    int value;
    auto id = uiOptions->buttonGroup->checkedId();
    qDebug() << "id " << id;
    switch (id) {
        case -2:
            value = -21400;
            break;
        case -3:
            value = -23000;
            break;
        case -4:
            value = -25000;
            break;
        default:
            value = -23000;
            break;
    }
    settingsHandler.storeValue("Ranges", "maxReverseRange", value);
    settingsHandler.storeValue("Ranges", "maxReverseId", id);
}

void optionsMenu::on_checkBox_stateChanged(int checked) {
    QList<QLineEdit *> allLabels =
            uiOptions->formLayoutWidget->findChildren<QLineEdit *>();
    if (uiOptions->checkBox->isChecked()) {
        for (auto allLabel : allLabels) {
            allLabel->setEnabled(true);
        }
        uiOptions->baudComboBox->setEnabled(true);

    } else {
        for (auto allLabel : allLabels) {
            allLabel->setEnabled(false);
        }
        uiOptions->baudComboBox->setEnabled(false);
    }
}

void optionsMenu::on_baudComboBox_currentTextChanged(const QString &arg1) {}
