//
// Created by DaveRiedel on 22-8-2022.
//
#include <gtest/gtest.h>
#include <QString>
#include <QJsonDocument>
#include <outputmenu/handlers/sethandler.h>

TEST(setHandlerTests,assert_from_json_to_set){
    QString jsonData = "{\n"
                       "    \"outputs\": [\n"
                       "        {\n"
                       "            \"cbText\": \"Fuel tank center 3 level\",\n"
                       "            \"dataType\": 0,\n"
                       "            \"id\": 106,\n"
                       "            \"metric\": \"Percent Over 100\",\n"
                       "            \"offset\": -1163005939,\n"
                       "            \"outputName\": \"FUEL TANK CENTER3 LEVEL\",\n"
                       "            \"prefix\": 236,\n"
                       "            \"type\": 1,\n"
                       "            \"updateEvery\": 0.009999999776482582\n"
                       "        },\n"
                       "        {\n"
                       "            \"cbText\": \"Fuel tank left aux level\",\n"
                       "            \"dataType\": 0,\n"
                       "            \"id\": 108,\n"
                       "            \"metric\": \"Percent Over 100\",\n"
                       "            \"offset\": -1163005939,\n"
                       "            \"outputName\": \"FUEL TANK LEFT AUX LEVEL\",\n"
                       "            \"prefix\": 238,\n"
                       "            \"type\": 1,\n"
                       "            \"updateEvery\": 0.009999999776482582\n"
                       "        },\n"
                       "        {\n"
                       "            \"cbText\": \"AP airspeed var\",\n"
                       "            \"dataType\": 0,\n"
                       "            \"id\": 500,\n"
                       "            \"metric\": \"knots\",\n"
                       "            \"offset\": -1163005939,\n"
                       "            \"outputName\": \"AUTOPILOT AIRSPEED HOLD VAR\",\n"
                       "            \"prefix\": 700,\n"
                       "            \"type\": 0,\n"
                       "            \"updateEvery\": 0\n"
                       "        }\n"
                       "    ],\n"
                       "    \"setId\": 222,\n"
                       "    \"setName\": \"Fuel\"\n"
                       "}";
    SetHandler setHandler = SetHandler();
    QJsonDocument jsonDocument = QJsonDocument::fromJson(jsonData.toUtf8());
    auto setFromJson = setHandler.fromJson(&jsonDocument);
    EXPECT_EQ(setFromJson.getID(),222);
    EXPECT_EQ(setFromJson.getSetName(),"Fuel");
}