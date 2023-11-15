#include "optionsmenu.h"
#include "elements/mcheckbox.h"
#include <iostream>
#include <string>
#include "ui_optionsmenu.h"

using namespace std;

void OptionsMenu::closeEvent(QCloseEvent* event) { delete this; }

void OptionsMenu::selectFile() {
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

OptionsMenu::OptionsMenu(QWidget* parent)
    : QWidget(parent), uiOptions(new Ui::optionsMenu) {
    uiOptions->setupUi(this);
    QStringList keys = *settingsHandler.retrieveKeys("Settings");
    if (!keys.empty()) {
        foreach(const QString &key, keys) {
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

    auto cbStartupMenu = new mCheckBox("Run on MFS2020 launch", "cbRunOnMFSStartup", false);
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
        this->findChild<QCheckBox *>("cbRunOnMFSStartup")
                ->setChecked(
                    settingsHandler.retrieveSetting("Settings", "cbRunOnMFSStartup")
                    ->toBool());
    }

    auto communityFolderPathLabel = new QLabel();
    auto communityFolderFileBtn = new QPushButton("Select community folder");
    connect(communityFolderFileBtn, &QPushButton::clicked, this,
            &OptionsMenu::selectFile);
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
    // uiOptions->vlEngineRange->addLayout(controller->createRudderRow());
    uiOptions->vlEngineRange->addLayout(builder->RangeBuilder());
    auto sensLayout = new QVBoxLayout();
    uiOptions->sensitivityWidget->setLayout(sensLayout);
    uiOptions->sensitivityWidget->layout()->setAlignment(Qt::AlignTop);


    uiOptions->sensitivityWidget->adjustSize();
    QStringList rangeKeys = *settingsHandler.retrieveKeys("Ranges");
    if (!rangeKeys.empty()) {
        foreach(const QString &key, rangeKeys) {
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

OptionsMenu::~OptionsMenu() {
    emit closedOptionsMenu();
    delete uiOptions;
}

void OptionsMenu::save_cbs() {
    QList<QCheckBox *> allCheckBoxes = this->findChildren<QCheckBox *>();
    for (auto&allCheckBox: allCheckBoxes) {
        settingsHandler.storeValue("Settings", allCheckBox->objectName(),
                                   allCheckBox->isChecked());
    }
}

void OptionsMenu::save_labels() {
    QList<QLineEdit *> allLabels =
            uiOptions->formLayoutWidget->findChildren<QLineEdit *>();
    qDebug() << "size" << allLabels.size();
    for (auto allLabel: allLabels) {
        QString name = allLabel->objectName();
        qDebug() << "Clicked" << name;
        settingsHandler.storeValue("Settings", name, allLabel->text());
    }
}

void OptionsMenu::save_ranges() {
    QList<QLineEdit *> rangeLineEdits =
            uiOptions->widgetRanges->findChildren<QLineEdit *>();

    for (auto&rangeLineEdit: rangeLineEdits) {
        settingsHandler.storeValue("Ranges", rangeLineEdit->objectName(),
                                   rangeLineEdit->text());
    }

    QString idleStr = "Engine " + QString::number(1) + "Min";
    int idleCutoff = settingsHandler.retrieveSetting("Ranges", idleStr)->toInt();
    qDebug() << "cut" << idleCutoff;

    int value;
    auto id = uiOptions->buttonGroup->checkedId();
    // map values of the checkboxes to ingame value that represent a negative value from the idle cutoff
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

void OptionsMenu::save_communityfolder_path() {
    const auto* communityFolderPath =
            this->findChild<QLabel *>("communityFolderPathLabel");

    settingsHandler.storeValue("Settings", communityFolderPath->objectName(),
                               communityFolderPath->text());
    PathHandler::setCommunityFolderPath(communityFolderPath->text());
}

void OptionsMenu::save_com_settings() {
    settingsHandler.storeValue("Settings", "CBR",
                               uiOptions->baudComboBox->currentText());
}


void OptionsMenu::on_saveSettingsBtn_clicked() {
    // find all checkboxes and save their state
    save_cbs();
    save_labels();
    save_ranges();
    save_communityfolder_path();
    save_com_settings();
}

void OptionsMenu::on_checkBox_stateChanged(int checked) {
    QList<QLineEdit *> allLabels =
            uiOptions->formLayoutWidget->findChildren<QLineEdit *>();
    if (uiOptions->checkBox->isChecked()) {
        for (auto allLabel: allLabels) {
            allLabel->setEnabled(true);
        }
        uiOptions->baudComboBox->setEnabled(true);
    }
    else {
        for (auto allLabel: allLabels) {
            allLabel->setEnabled(false);
        }
        uiOptions->baudComboBox->setEnabled(false);
    }
}

void OptionsMenu::on_baudComboBox_currentTextChanged(const QString&arg1) {
}
