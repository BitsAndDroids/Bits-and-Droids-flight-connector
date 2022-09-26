#ifndef OPTIONSMENU_H
#define OPTIONSMENU_H

#include "models/aircraft/Engine.h"

#include <QWidget>

#include "widgets/settingsmenu/builder/formbuilder.h"
#include "settings/settingshandler.h"

namespace Ui {
class optionsMenu;
}

class OptionsMenu : public QWidget {
  Q_OBJECT

 public:
  explicit OptionsMenu(QWidget *parent = nullptr);

  ~OptionsMenu() override;
 signals:

  void closedOptionsMenu();

 private slots:

  void on_checkBox_stateChanged(int arg1);

  void on_saveSettingsBtn_clicked();

  void on_baudComboBox_currentTextChanged(const QString &arg1);

  void selectFile();

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
