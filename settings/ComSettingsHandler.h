//
// Created by DaveRiedel on 11-10-2022.
//
#ifndef BITSANDDROIDSCONNECTOR_COMSETTINGSHANDLER_H
#define BITSANDDROIDSCONNECTOR_COMSETTINGSHANDLER_H
#ifdef QT_DEBUG
#include "_deps/googletest-src/googletest/include/gtest/gtest.h"
#endif
#include "settings/settingshandler.h"

    class ComSettingsHandler : public SettingsHandler {
    public:
        ComSettingsHandler();

        QList<QPair<QString, int>> loadComs();

        void saveComs(QList<QPair<QString, int>> coms);

        void adjustIndexes();

        void removeComRowSettings(QString index);

    private:
#ifdef QT_DEBUG
        FRIEND_TEST(ComSettingsHandlerTests, test_serialize_qlist_to_json);
        FRIEND_TEST(ComSettingsHandlerTests, test_serialize_qpair_to_json);
        FRIEND_TEST(ComSettingsHandlerTests, test_serialize_json_to_qlist);
#endif

        QJsonObject comPairToJson(const QPair<QString, int> &pair);

        QPair<QString, int> comPairFromJson(const QJsonObject &object);


        QJsonObject comListToJsonObject(QList<QPair<QString, int>> coms);

        QList<QPair<QString, int>> jsonObjectToComList(QJsonObject obj);
    };


#endif //BITSANDDROIDSCONNECTOR_COMSETTINGSHANDLER_H
