//
// Created by DaveRiedel on 26-9-2022.
//

#ifndef BITSANDDROIDSCONNECTOR_MIGRATION_H
#define BITSANDDROIDSCONNECTOR_MIGRATION_H


#include <QString>
#include "settings/settingshandler.h"

class Migration {
public:
    explicit Migration(int migrationId){
        this->migrationId = migrationId;
    }

    virtual void migrate() = 0;

    //TODO add a rollback function when the migration fails
    //virtual void revert() = 0;

    static void moveGroup(const QString& group, const QString& targetGroup){
        auto settingsHandler = SettingsHandler();
        auto keys = settingsHandler.retrieveKeys(group);
        auto targetKeys = settingsHandler.retrieveKeys(targetGroup);
        int targetIndexCounter = (targetKeys->empty()) ? 0 : targetKeys->at(targetKeys->size()-1).toInt() + 1;
        for (auto &key : *keys) {
            auto value = settingsHandler.retrieveSetting(group, key);
            settingsHandler.removeSetting(group,key);
            settingsHandler.storeValue(targetGroup, QString::number(targetIndexCounter), *value);
            targetIndexCounter++;
        }
    };

    int getMigrationId(){
        return migrationId;
    };




private:
    int migrationId;

};


#endif //BITSANDDROIDSCONNECTOR_MIGRATION_H
