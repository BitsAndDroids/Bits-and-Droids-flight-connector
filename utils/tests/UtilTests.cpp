//
// Created by DaveRiedel on 22-8-2022.
//
#include <gtest/gtest.h>
#include <QString>
#include <utils/OutputConverters.h>
#include <models/commands/output.h>

TEST(UtilsTests, com_to_format) {
    OutputConverters converters = OutputConverters();
    float rawCom = 123456;

    Output output = Output();
    output.setPrefix(900);
    output.setType(0);

    std::string received = converters.formatOutgoingString(rawCom, output);
    std::string expectedChar = "900 123456\n";
    EXPECT_EQ(received, expectedChar);
}

TEST(UtilsTests, percentage_to_format) {
    OutputConverters converters = OutputConverters();
    float incValue = 0.8345;

//    {
//        "id": 79,
//                "outputName": "GEAR TOTAL PCT EXTENDED",
//                "metric": "Percentage",
//                "updateEvery": 0.01,
//                "dataType": 0,
//                "cbText":"Gear total extended %",
//                "prefix": 536,
//                "type":5
//    }
    Output output = Output();
    output.setPrefix(536);
    output.setType(5);

    std::string received = converters.formatOutgoingString(incValue, output);
    std::string expectedChar = "536 83\n";
    EXPECT_EQ(received, expectedChar);
}
