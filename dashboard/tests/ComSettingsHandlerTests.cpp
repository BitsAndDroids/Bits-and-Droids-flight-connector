//
// Created by DaveRiedel on 12-10-2022.
//
#include <sstream>
#include <QString>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <dashboard/utils/ComSettingsHandler.h>
#include <gtest/gtest.h>

TEST(ComSettingsHandlerTests, test_serialize_qpair_to_json) {
    QPair<QString, int> pair = QPair<QString, int>("COM1", 1);
    ComSettingsHandler handler;
    auto json = handler.comPairToJson(pair);

    QString expectedPort = "COM1";
    EXPECT_EQ(json["comPort"].toString(), expectedPort);

    int expectedSet = 1;
    EXPECT_EQ(json["comSet"].toInt(), expectedSet);
}

TEST(ComSettingsHandlerTests, test_serialize_qlist_to_json) {
    QList<QPair<QString, int>> coms = QList<QPair<QString, int>>();
    coms.append(QPair<QString, int>({"COM1", 1}));
    coms.append(QPair<QString, int>({"COM2", 2}));
    coms.append(QPair<QString, int>({"COM3", 3}));

    ComSettingsHandler handler;
    auto json = handler.comListToJsonObject(coms);

    int expectedKeySize = 3;
    EXPECT_EQ(json.keys().size(), expectedKeySize);

    QStringList expectedKeys = {"0", "1", "2"};
    EXPECT_EQ(json.keys(), expectedKeys);

    QString expectedPort = "COM2";
    EXPECT_EQ(json["1"].toObject()["comPort"].toString(), expectedPort);

    int expectedSet = 2;
    EXPECT_EQ(json["1"].toObject()["comSet"].toInt(), expectedSet);
}

TEST(ComSettingsHandlerTests, test_serialize_json_to_qlist) {
    QString val =  R"({"0":{"comPort":"COM1","comSet":1},"1":{"comPort":"COM2","comSet":2},"2":{"comPort":"COM3","comSet":3}})";
    QJsonDocument doc = QJsonDocument::fromJson(val.toUtf8());

    ComSettingsHandler handler;
    std::cout<<doc.object().keys().size()<<std::endl;
    auto listConverted = handler.jsonObjectToComList(doc.object());

    int expectedKeySize = 3;
    EXPECT_EQ(listConverted.size(), expectedKeySize);

    QString expectedPort = "COM2";
    EXPECT_EQ(listConverted[1].first, expectedPort);

    int expectedSet = 2;
    EXPECT_EQ(listConverted[1].second, expectedSet);
}



