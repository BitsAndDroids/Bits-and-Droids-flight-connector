#ifndef OPTIONSMENU_H
#define OPTIONSMENU_H

#include <QWidget>

namespace Ui {
class optionsMenu;
}

class optionsMenu : public QWidget
{
    Q_OBJECT

public:
    explicit optionsMenu(QWidget *parent = nullptr);
    ~optionsMenu();

private slots:


    void on_checkBox_stateChanged(int arg1);

    void on_saveSettingsBtn_clicked();

    void on_baudComboBox_currentTextChanged(const QString &arg1);

private:
    Ui::optionsMenu *uiOptions;
    const int defaultArduinoFrameUpdate = 15;
    const int defaultArduinoWaitMs = 100;

    int arduinoFrameUpdate = 15;
    int arduinoWaitMs = 100;
};

#endif // OPTIONSMENU_H
