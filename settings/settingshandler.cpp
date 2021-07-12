#include "settingshandler.h"

SettingsHandler::SettingsHandler()
{

}

void SettingsHandler::storeQStringToGroup(QString group, QString key ,QString value)
{
    settings->beginGroup(group);
    settings->setValue(key, value);
    settings->endGroup();
}
