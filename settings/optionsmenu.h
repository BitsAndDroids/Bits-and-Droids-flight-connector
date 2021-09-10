#ifndef OPTIONSMENU_H
#define OPTIONSMENU_H

#include "settingshandler.h"
#include "formbuilder.h"

#include <headers/Engine.h>

#include <QWidget>

namespace Ui {
    class optionsMenu;
}

class optionsMenu : public QWidget {
Q_OBJECT

public:
    explicit optionsMenu(QWidget *parent = nullptr);

    ~optionsMenu() override;
    signals:

    void closedOptionsMenu();

private slots:

    void on_checkBox_stateChanged(int arg1);

    void on_saveSettingsBtn_clicked();

    void on_baudComboBox_currentTextChanged(const QString &arg1);

private:
    FormBuilder *builder = new FormBuilder();
    SettingsHandler settingsHandler;
    Ui::optionsMenu *uiOptions;
    const int defaultArduinoFrameUpdate = 15;
    const int defaultArduinoWaitMs = 100;

    int arduinoFrameUpdate = 15;
    int arduinoWaitMs = 100;
    int supportedAmntEngines = 4;

    // Engine engines[4];
    void closeEvent(QCloseEvent *event) override;
};

#endif  // OPTIONSMENU_H
