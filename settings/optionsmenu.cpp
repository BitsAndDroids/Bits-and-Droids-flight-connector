#include "optionsmenu.h"

#include <elements/mcheckbox.h>
#include <qstandardpaths.h>

#include <iostream>
#include <string>

#include "ui_optionsmenu.h"

using namespace std;

void optionsMenu::closeEvent(QCloseEvent *event) { delete this; }

void optionsMenu::selectFile() {
    QFileDialog dialog(this);
    dialog.setFileMode(QFileDialog::Directory);
    QString communityFolderPath = dialog.getExistingDirectory();
    cout << communityFolderPath.toStdString().c_str() << endl;
    if (!communityFolderPath.isNull()) {
        auto pathLabel = this->findChild<QLabel *>("communityFolderPathLabel");

        pathLabel->setText(communityFolderPath);
        pathLabel->adjustSize();
    }
}

optionsMenu::optionsMenu(QWidget *parent)
        : QWidget(parent), uiOptions(new Ui::optionsMenu) {
    uiOptions->setupUi(this);
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

    auto cbCloseToTray = new mCheckBox("Close to tray", "cbCloseToTray", true);
    uiOptions->vlOptions->addWidget(cbCloseToTray->generateCheckbox());

    auto cbStartupMenu = new mCheckBox("Run on startup", "cbRunOnStartup", false);
    uiOptions->vlOptions->addWidget(cbStartupMenu->generateCheckbox());

    auto cbAutoRun = new mCheckBox("Autorun in background", "cbAutorun", false);
    uiOptions->vlOptions->addWidget(cbAutoRun->generateCheckbox());

    // Loading the saved checkbox states
    if (!settingsHandler.retrieveSetting("Settings", "cbCloseToTray")->isNull()) {
        this->findChild<QCheckBox *>("cbCloseToTray")
                ->setChecked(
                        settingsHandler.retrieveSetting("Settings", "cbCloseToTray")
                                ->toBool());
        this->findChild<QCheckBox *>("cbAutorun")
                ->setChecked(
                        settingsHandler.retrieveSetting("Settings", "cbAutorun")
                                ->toBool());
        this->findChild<QCheckBox *>("cbRunOnStartup")
                ->setChecked(
                        settingsHandler.retrieveSetting("Settings", "cbRunOnStartup")
                                ->toBool());
    }

    auto communityFolderPathLabel = new QLabel();
    auto communityFolderFileBtn = new QPushButton("Select community folder");
    connect(communityFolderFileBtn, &QPushButton::clicked, this,
            &optionsMenu::selectFile);
    communityFolderFileBtn->setMaximumWidth(150);
    communityFolderPathLabel->setMaximumWidth(250);

    communityFolderPathLabel->setWordWrap(true);
    communityFolderPathLabel->setSizePolicy(QSizePolicy::Preferred,
                                            QSizePolicy::MinimumExpanding);
    uiOptions->vlOptions->addWidget(communityFolderFileBtn);
    communityFolderPathLabel->setObjectName("communityFolderPathLabel");
    uiOptions->vlOptions->addWidget(communityFolderPathLabel);
    uiOptions->vlOptions->setSizeConstraint(QLayout::SetMinimumSize);
    uiOptions->vlOptions->setAlignment(Qt::AlignTop);
    // uiOptions->vlEngineRange->addLayout(builder->createRudderRow());
    uiOptions->vlEngineRange->addLayout(builder->RangeBuilder());
    auto sensLayout = new QVBoxLayout();
    uiOptions->sensitivityWidget->setLayout(sensLayout);
    uiOptions->sensitivityWidget->layout()->setAlignment(Qt::AlignTop);

    if (!settingsHandler
            .retrieveSubSetting("rudderSeries", "sliders", "rudderDeadzone")
            ->isNull()) {
        uiOptions->sensitivityWidget->findChild<QSlider *>("rudderDeadzone")
                ->setValue(
                        settingsHandler
                                .retrieveSubSetting("rudderSeries", "sliders", "rudderDeadzone")
                                ->toInt());
        uiOptions->sensitivityWidget->findChild<QSlider *>("rudderMinSensitivity")
                ->setValue(settingsHandler
                                   .retrieveSubSetting("rudderSeries", "sliders",
                                                       "rudderMinSensitivity")
                                   ->toInt());

        uiOptions->sensitivityWidget->findChild<QSlider *>("rudderPlusSensitivity")
                ->setValue(settingsHandler
                                   .retrieveSubSetting("rudderSeries", "sliders",
                                                       "rudderPlusSensitivity")
                                   ->toInt());
        // Range handling
    }
    uiOptions->sensitivityWidget->adjustSize();
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
    if (!settingsHandler.retrieveSetting("Settings", "communityFolderPathLabel")
            ->isNull()) {
        communityFolderPathLabel->setText(
                settingsHandler.retrieveSetting("Settings", "communityFolderPathLabel")
                        ->toString());
        communityFolderPathLabel->adjustSize();
    }
}

optionsMenu::~optionsMenu() {
    emit closedOptionsMenu();
    delete uiOptions;
}

void optionsMenu::on_saveSettingsBtn_clicked() {
    QLabel *communityFolderPath =
            this->findChild<QLabel *>("communityFolderPathLabel");
    settingsHandler.storeValue("Settings", communityFolderPath->objectName(),
                               communityFolderPath->text());
    PathHandler pathHandler;
    pathHandler.setCommunityFolderPath(communityFolderPath->text());

    // Checkboxes that affect the application behavior on closing / startup
    auto cbCloseToTray = this->findChild<QCheckBox *>("cbCloseToTray");
    settingsHandler.storeValue("Settings", "cbCloseToTray",
                               cbCloseToTray->isChecked());

    auto cbAutoRun = this->findChild<QCheckBox *>("cbAutorun");
    settingsHandler.storeValue("Settings", "cbAutorun",
                               cbAutoRun->isChecked());

    auto cbStartup = this->findChild<QCheckBox *>("cbRunOnStartup");
    settingsHandler.storeValue("Settings", "cbRunOnStartup",
                               cbStartup->isChecked());

    // The default startup path is
    // (C:\Users\<USER>\AppData\Roaming\Microsoft\Windows\Start
    // Menu\Programs\Startup
    auto startupPath =
            QStandardPaths::writableLocation(QStandardPaths::ApplicationsLocation) +
            "/Startup/";

    if (cbStartup->isChecked()) {
        // This will create a shortcut in the startup menu
        // This will also overwrite any existing links(of the connector) already
        // pressent.
        QFile::link("Bitsanddroidsgui.exe",
                    startupPath + "Bitsanddroidsgui.exe.lnk");

    } else {
        QFile file(startupPath + "Bitsanddroidsgui.exe.lnk");

        if (file.exists()) {
            file.remove();
        }
    }

    QList<QLineEdit *> allLabels =
            uiOptions->formLayoutWidget->findChildren<QLineEdit *>();
    qDebug() << "size" << allLabels.size();
    for (auto allLabel: allLabels) {
        QString name = allLabel->objectName();
        qDebug() << "Clicked" << name;
        settingsHandler.storeValue("Settings", name, allLabel->text());
    }
    settingsHandler.storeValue("Settings", "CBR",
                               uiOptions->baudComboBox->currentText());

    QList<QLineEdit *> rangeLineEdits =
            uiOptions->widgetRanges->findChildren<QLineEdit *>();

    for (auto &rangeLineEdit: rangeLineEdits) {
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
        for (auto allLabel: allLabels) {
            allLabel->setEnabled(true);
        }
        uiOptions->baudComboBox->setEnabled(true);

    } else {
        for (auto allLabel: allLabels) {
            allLabel->setEnabled(false);
        }
        uiOptions->baudComboBox->setEnabled(false);
    }
}

void optionsMenu::on_baudComboBox_currentTextChanged(const QString &arg1) {}
