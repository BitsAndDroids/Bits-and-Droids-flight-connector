//
// Created by DaveRiedel on 26-9-2022.
//

#ifndef BITSANDDROIDSCONNECTOR_2022092601_H
#define BITSANDDROIDSCONNECTOR_2022092601_H

#include "Migration.h"
#include "settings/settingshandler.h"

class Migration2022092601 : public Migration {
public:
    /*
     * This migration moves the triple mode(dual-, input-, output-) comports to the comports group
     * This is implemented to merge  three separate comPort groups to a single group
     */
    explicit Migration2022092601(int migrationId) : Migration(migrationId) {}

    void migrate() override {
        Migration::moveGroup("dualComs", "comPorts");
        Migration::moveGroup("dualSets", "comSets");
        Migration::moveGroup("outputComs", "comPorts");
        Migration::moveGroup("outputSets", "comSets");
        Migration::moveGroup("inputComs", "comPorts");
    }
};
#endif //BITSANDDROIDSCONNECTOR_2022092601_H
